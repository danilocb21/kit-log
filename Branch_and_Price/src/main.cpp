#include "data.h"
#include "gurobi_c++.h"

using namespace std;

// modelo exemplo

/*
    maximize    x +   y + 2 z
    subject to  x + 2 y + 3 z <= 4
                x +   y       >= 1
                x, y, z binary
*/

int main(int argc, char** argv) {
    
    auto data = Data(argc, argv[1]);
    data.read();

    cout << data.getDimension() << ' ' << data.getCapacity() << '\n';
    for (int i = 0; i < data.getDimension(); i++)
        cout << data.getWeight(i) << " \n"[i==data.getDimension()-1];

    return 0;

    
    try {
        GRBEnv env = GRBEnv(true);
        //env.set("LogFile", "mip1.log");
        env.start();

        GRBModel model = GRBModel(env);

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

        cout << x.get(GRB_StringAttr_VarName) << " "
            << x.get(GRB_DoubleAttr_X) << endl;
        cout << y.get(GRB_StringAttr_VarName) << " "
            << y.get(GRB_DoubleAttr_X) << endl;
        cout << z.get(GRB_StringAttr_VarName) << " "
            << z.get(GRB_DoubleAttr_X) << endl;

        cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;


    } catch (GRBException &e){
        cout << "Error code: " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch (...) {
        cout << "Error solving the model." << endl;
    }

    return 0;
}
