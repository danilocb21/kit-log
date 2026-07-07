#include <iostream>

#include "BP.h"
#include "subproblem.h"
#include "gurobi_c++.h"

using namespace std;

void BP::solve() {
    GRBVar    *lmbda   = 0;
    GRBConstr *constrs = 0;
    int n_lmbda = 0;

    try {
        env = new GRBEnv();

        GRBModel model = GRBModel(*env);

        model.set(GRB_StringAttr_ModelName, "Branch and Price");
        model.set(GRB_DoubleParam_TimeLimit, 1*60*60);
        model.set(GRB_IntParam_Threads, 1);
        model.set(GRB_DoubleParam_Cutoff, ub);
        model.set(GRB_DoubleParam_FeasibilityTol, EPS);
        
        n_lmbda = n;
        
        lmbda = new GRBVar[n_lmbda];
        for (int j = 0; j < n_lmbda; j++) {
            string vx = "lmbda[" + to_string(j+1) + "]";
            lmbda[j] = model.addVar(0.0, GRB_INFINITY, 1.0, GRB_CONTINUOUS, vx.c_str());
        }
        
        // Constraints
        constrs = new GRBConstr[n_lmbda];
        for (int i = 0; i < n_lmbda; i++) {
            string vx = "c" + to_string(i+1);
            constrs[i] = model.addConstr(lmbda[i] == 1, vx.c_str());
        }

        model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);

        column_gen(model, lmbda, constrs, n_lmbda);

        print_results(model, instance.getInstanceName());
        //print_solution(model);
        
    } catch (GRBException &e){
        cout << "Error code: " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch (...) {
        cout << "Error solving the model." << endl;
    }
    
    delete[] lmbda;
    delete[] constrs;
    delete env;
}

inline void BP::column_gen(
    GRBModel &model, 
    GRBVar* lmbda, 
    GRBConstr* constrs, 
    int &n_lmbda
) {
    while (true) {
        model.optimize();

        vector<double> duals(n_lmbda);
        for (int j = 0; j < n_lmbda; j++) {
            duals[j] = constrs[j].get(GRB_DoubleAttr_Pi);
        }

        SubProblem knps(n, duals, weight, capacity);
        knps.solve();

        // cout << knps.objVal << endl;
        // for (int x : knps.solution) cout << x << ' ';
        // cout << endl;

        if (knps.objVal >= EPS) {
            break;
        } else {
            // adicionando col. no prob mestre
            
        }
    }
}