#include <iostream>

#include "BP.h"
#include "gurobi_c++.h"

using namespace std;

void BP::solve() {
    try {
        env = new GRBEnv();

        GRBModel model = GRBModel(*env);

        model.set(GRB_StringAttr_ModelName, "Branch and Price");
        model.set(GRB_DoubleParam_TimeLimit, 1*60*60);
        model.set(GRB_IntParam_Threads, 1);
        model.set(GRB_DoubleParam_Cutoff, ub);
        
        GRBVar x = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x");
        GRBVar y = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y");
        GRBVar z = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "z");

        model.addConstr(x + 2 * y + 3 * z <= 4, "c0");
        model.addConstr(x + y >= 1, "c1");

        // GRBLinExpr obj = 0.0;
        // obj += x;
        // obj += y;
        // obj += 2*z;
        // model.setObjective(obj, GRB_MAXIMIZE);
        model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);

        model.optimize();

        print_results(model, instance.getInstanceName());
        //print_solution(model);
        
    } catch (GRBException &e){
        cout << "Error code: " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch (...) {
        cout << "Error solving the model." << endl;
    }

    delete env;
}