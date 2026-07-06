#include "BP.h"
#include "data.h"

using namespace std;

int main(int argc, char** argv) {
    double ub = numeric_limits<double>::max();

    if (argc > 2) {
        ub = stod(argv[2]);
    }
    
    auto data = Data(argc, argv[1]);
    data.read();

    BP bp = BP(data, ub);
    
    bp.solve();

    return 0;
}
