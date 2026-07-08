#include <iostream>
#include <chrono>

#include "BP.h"
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
        model.set(GRB_IntParam_Threads, 1);
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

        column_gen(model, lmbda, constrs, n_lmbda);

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;

        print_results(model, duration.count(), instance.getInstanceName());
        //print_solution(model);
        
    } catch (GRBException &e){
        cout << "Error code: " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch (...) {
        cout << "Error solving the model." << endl;
    }
    
    delete env;
}

inline void BP::column_gen(
    GRBModel &model, 
    vector<GRBVar> &lmbda, 
    vector<GRBConstr> &constrs, 
    int &n_lmbda
) {
    while (true) {
        model.optimize();

        vector<double> duals(n);
        for (int i = 0; i < n; i++) {
            duals[i] = constrs[i].get(GRB_DoubleAttr_Pi);
        }
        
        SubProblem knps(n, duals, weight, capacity);
        knps.solve(SOLVE_DP, env);

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
}