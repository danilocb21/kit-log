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
        //print_solution(model, lmbda, constrs);
        
    } catch (GRBException &e){
        cout << "Error code: " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch (...) {
        cout << "Error solving the model." << endl;
    }
    
    delete env;
}

void BP::branch(
    GRBModel& model,
    vector<GRBVar>& lmbda,
    vector<GRBConstr>& constrs,
    int& n_lmbda
) {
    vector<bool> lmbd_itens(n * n);
    for (int i = 0; i < n; i++)
        lmbd_itens[at(i,i)] = true;

    Node root;
    root.added_pair.resize(n * n);
    column_gen(model, lmbda, lmbd_itens, constrs, n_lmbda, root, SOLVE_DP);
    
    tree.push_back(root);

    while (!tree.empty()) {
        Node node = tree.back();
        tree.pop_back();

        // ceil(node.lb) >= ub
        if (node.lb + 1 > ub) continue;

        if (node.is_feasible()) {
            ub = min(ub, node.lb);
        } else {
            Pair pair = node.most_fract;
            
            node.added_pair[at(pair.first, pair.second)] = true;

            node.reqrd_pairs.push_back(pair);
            column_gen(model, lmbda, lmbd_itens, constrs, n_lmbda, node, SOLVE_MODEL);
            // ceil(node.lb) < ub
            if (node.lb + 1 <= ub)
                tree.push_back(node);

            node.reqrd_pairs.pop_back();

            node.frbnd_pairs.push_back(pair);
            column_gen(model, lmbda, lmbd_itens, constrs, n_lmbda, node, SOLVE_MODEL);
            // ceil(node.lb) < ub
            if (node.lb + 1 <= ub)
                tree.push_back(node);
        }
    }
}

void BP::column_gen(
    GRBModel& model, 
    vector<GRBVar>& lmbda,
    vector<bool>& lmbd_itens,
    vector<GRBConstr>& constrs, 
    int& n_lmbda,
    Node& node,
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

    // bool infeasible = false;

    int prev_sz = n_lmbda;

    while (true) {
        model.optimize();

        // if (model.get(GRB_IntAttr_Status) != GRB_OPTIMAL) {
        //     infeasible = true;
        //     break;
        // }

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
                    
                // lmbd_itens.push_back(xi + EPS >= 1.0);
            }
            
            string vx = "lmbda[" + to_string(n_lmbda) + "]";
            n_lmbda++;
            lmbda.push_back(model.addVar(0.0, GRB_INFINITY, 1.0, GRB_CONTINUOUS, col, vx.c_str()));

        }
    }

    // reativando os padroes proibidos
    for (int j : idx_add)
        lmbda[j].set(GRB_DoubleAttr_UB, GRB_INFINITY);

    // if (infeasible) {
    //     node.lb = INF;
    //     cout << "Error" << endl;
    //     getchar();
    //     return;
    // }

    node.lb = model.get(GRB_DoubleAttr_ObjBound);

    if (node.lb >= ub) return;

    node.lmbda_val.resize(n_lmbda);
    for (int j = 0; j < n_lmbda; j++)
        node.lmbda_val[j] = lmbda[j].get(GRB_DoubleAttr_X);

    // int removed = 0;
    // for (int j = prev_sz; j < n_lmbda; j++) {
    //     node.lmbda_val[j - removed] = node.lmbda_val[j];

    //     if (node.lmbda_val[j] <= EPS) {

    //         model.remove(lmbda[j - removed]);
    //         lmbda.erase(lmbda.begin() + j - removed);

    //         removed++;
    //     }
    // }
    
    // n_lmbda -= removed;
    
    most_fractional(model, lmbda, lmbd_itens, constrs, node, n_lmbda);

    // cout << "UB: " << ub << endl;
    // cout << "LB: " << node.lb << endl;
    // cout << node.most_fract.first << ' ' << node.most_fract.second << endl;
}

void BP::most_fractional(
    GRBModel& model, 
    vector<GRBVar>& lmbda,
    vector<bool>& lmbd_itens, 
    vector<GRBConstr>& constrs, 
    Node& node,
    int& n_lmbda
) {
    // // Pega a ultima linha antes do resize
    int old_sz = lmbd_itens.size() / n;
    lmbd_itens.resize(n_lmbda * n);

    for (int j = old_sz; j < n_lmbda; j++) {
        for (int i = 0; i < n; i++) {
            double coeff = model.getCoeff(constrs[i], lmbda[j]);
            lmbd_itens[at(j,i)] = coeff > EPS;
        }
    }
    // for (int j = 0; j < n_lmbda; j++) {
    //     cout << "lmb" << j << "- " << node.lmbda_val[j] << ": ";
    //     for (int i = 0; i < n; i++) {
    //         cout << (lmbd_itens[at(j,i)] ? 1 : 0) << " \n"[i==n-1];
    //     }

    // }

    static vector<double> sum_pair(n*n);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            sum_pair[at(i,j)] = 0.0;
    
    for (int k = 0; k < n_lmbda; k++) {
        if (node.lmbda_val[k] <= EPS) continue;
        
        for (int i = 0; i < n; i++) {
            for (int j = i+1; j < n; j++) {
                if (!node.added_pair[at(i,j)] && lmbd_itens[at(k,i)] && lmbd_itens[at(k,j)]) {
                    sum_pair[at(i,j)] += node.lmbda_val[k];
                }
            }
        }
    }

    node.most_fract_val = 0.0;
    node.most_fract = make_pair(0,0);

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
                        
            double sum = sum_pair[at(i,j)];
            double val = min(sum - floor(sum), ceil(sum) - sum);

            if (val > node.most_fract_val) {
                node.most_fract_val = val;
                node.most_fract = make_pair(i,j);
            }
        }
    }
}

inline int BP::at(int i, int j) {
    return i * n + j;
}