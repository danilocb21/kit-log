#include <iostream>
#include <chrono>

#include "BP.h"
#include "node.h"
#include "subproblem.h"
#include "gurobi_c++.h"

using namespace std;

void BP::solve() {
    try {
        // Limpa o arquivo antes da execução
        std::ofstream ofs("bp.log", std::ios::out | std::ios::trunc);
        ofs.close();

        env = new GRBEnv();

        env->set(GRB_IntParam_LogToConsole, 0);
        env->set(GRB_StringParam_LogFile, "bp.log");

        GRBModel model = GRBModel(*env);

        model.set(GRB_StringAttr_ModelName, "Branch and Price");
        model.set(GRB_DoubleParam_TimeLimit, 1*60*60);
        //model.set(GRB_IntParam_Threads, 1);
        model.set(GRB_DoubleParam_Cutoff, ub);
        model.set(GRB_DoubleParam_FeasibilityTol, EPS);
        
        int n_lmbda = n;
        
        vector<GRBVar> lmbda(n_lmbda);
        for (int j = 0; j < n_lmbda; j++) {
            string vx = "lmbda[" + to_string(j+1) + "]";
            lmbda[j] = model.addVar(0.0, GRB_INFINITY, 1.0, GRB_CONTINUOUS, vx.c_str());
        }
        
        // Constraints
        vector<GRBConstr> constrs(n);
        for (int i = 0; i < n; i++) {
            string vx = "c" + to_string(i+1);
            constrs[i] = model.addConstr(lmbda[i] == 1, vx.c_str());
        }

        model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);


        auto start = std::chrono::high_resolution_clock::now();

        branch(model, lmbda, constrs, n_lmbda);

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;

        print_results(model, duration.count(), instance.getInstanceName());
        print_solution(model, lmbda, constrs);
        
    } catch (GRBException &e){
        cout << "Error code: " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch (...) {
        cout << "Error solving the model." << endl;
    }
    
    delete env;
}

void BP::branch(
    GRBModel &model,
    vector<GRBVar> &lmbda,
    vector<GRBConstr> &constrs,
    int &n_lmbda
) {
    vector<bool> lmbd_itens(n * n);
    for (int i = 0; i < n; i++)
        lmbd_itens[at(i,i)] = true;

    Node root;
    column_gen(model, lmbda, lmbd_itens, constrs, n_lmbda, root, SOLVE_DP);
    
    tree.push_back(root);

    while (!tree.empty()) {
        Node node = tree.back();
        tree.pop_back();

        // ceil(node.lb) >= ub
        if (node.lb + 1 >= ub) continue;

        if (node.is_feasible()) {
            ub = min(ub, node.lb);
        } else {
            Pair pair = node.most_fract;

            node.reqrd_pairs.push_back(pair);
            column_gen(model, lmbda, lmbd_itens, constrs, n_lmbda, node, SOLVE_MODEL);
            // ceil(node.lb) < ub
            if (node.lb + 1 < ub)
                tree.push_back(node);

            node.reqrd_pairs.pop_back();

            node.frbnd_pairs.push_back(pair);
            column_gen(model, lmbda, lmbd_itens, constrs, n_lmbda, node, SOLVE_MODEL);
            // ceil(node.lb) < ub
            if (node.lb + 1 < ub)
                tree.push_back(node);
        }
    }
}

inline void BP::column_gen(
    GRBModel &model, 
    vector<GRBVar> &lmbda,
    vector<bool> &lmbd_itens,
    vector<GRBConstr> &constrs, 
    int &n_lmbda,
    Node &node,
    int method
) {

    // Proibindo lambdas onde um par esta proibido
    // ou entao que nao contenha o par requerido
    vector<int> idx_add;
    vector<bool> add_lmbdas(n_lmbda);

    for (auto [i,j] : node.frbnd_pairs) {
        for (int k = 0; k < n_lmbda; k++) {
            if (!add_lmbdas[k] && lmbd_itens[at(k,i)] && lmbd_itens[at(k,j)]) {
                lmbda[k].set(GRB_DoubleAttr_UB, 0.0);
                add_lmbdas[k] = true;
                idx_add.push_back(k);
            }
        }
    }

    for (auto [i,j] : node.reqrd_pairs) {
        for (int k = 0; k < n_lmbda; k++) {
    //                              A soma = 1 significa que um 
    //                              item esta incluido mas o outro nao
            if (!add_lmbdas[k] && lmbd_itens[at(k,i)] + lmbd_itens[at(k,j)] == 1) {
                lmbda[k].set(GRB_DoubleAttr_UB, 0.0);
                add_lmbdas[k] = true;
                idx_add.push_back(k);
            }
        }
    }

    while (true) {
        model.optimize();

        if (model.get(GRB_DoubleAttr_ObjBound) >= ub) break;

        vector<double> duals(n);
        for (int i = 0; i < n; i++) {
            duals[i] = constrs[i].get(GRB_DoubleAttr_Pi);
        }
        
        SubProblem knps(n, duals, weight, capacity);
        knps.solve(method, node, env);

        if (knps.objVal + EPS >= 0.0) {
            break;
        } else {
            // adicionando col. no prob mestre
            GRBColumn col;
            for (int i = 0; i < n; i++) {
                double xi = (double) knps.solution[i];
                if (xi + EPS >= 1.0)
                    col.addTerm(xi, constrs[i]);
            }
            
            string vx = "lmbda[" + to_string(n_lmbda) + "]";
            n_lmbda++;
            lmbda.push_back(model.addVar(0.0, GRB_INFINITY, 1.0, GRB_CONTINUOUS, col, vx.c_str()));
        }
    }

    node.lb = model.get(GRB_DoubleAttr_ObjBound);
    if (node.lb >= ub) return;
    
    auto sol = most_fractional(model, lmbda, lmbd_itens, constrs, n_lmbda);
    node.most_fract_val = sol.first;
    node.most_fract = sol.second;

    // reativando os padroes proibidos
    for (int j : idx_add)
        lmbda[j].set(GRB_DoubleAttr_UB, GRB_INFINITY);
}

inline pair<double, Pair> BP::most_fractional(
    GRBModel &model, 
    vector<GRBVar> &lmbda,
    vector<bool> &lmbd_itens, 
    vector<GRBConstr> &constrs, 
    int &n_lmbda
) {
    // Pega a ultima linha antes do resize
    int old_sz = lmbd_itens.size() / n;
    lmbd_itens.resize(n_lmbda * n);

    for (int j = old_sz; j < n_lmbda; j++) {
        for (int i = 0; i < n; i++) {
            double coeff = model.getCoeff(constrs[i], lmbda[j]);
            lmbd_itens[at(j,i)] = coeff > EPS;
        }
    }

    double mst_fract = 0.0;
    Pair mst_fract_pair;

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n_lmbda; k++) {
                if (!lmbd_itens[at(k,i)] || !lmbd_itens[at(k,j)]) continue;
                sum += lmbda[k].get(GRB_DoubleAttr_X);
            }
            
            double val = min(sum - floor(sum), ceil(sum) - sum);
            if (val > mst_fract) {
                mst_fract = val;
                mst_fract_pair = make_pair(i,j);
            }
        }
    }

    return make_pair(mst_fract, mst_fract_pair);
}

inline int BP::at(int i, int j) {
    return i * n + j;
}