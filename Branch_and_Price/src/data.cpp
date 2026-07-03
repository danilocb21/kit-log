#include "data.h"

using namespace std;

//Inicializador
Data::Data(int qtParam, char *instance):
weight(NULL) {

	if (qtParam < 2) {
        cout << "\nMissing parameters\n";
        cout << " ./bp [Instance] " << endl;
        exit(1);
    }

    if (qtParam > 3) {
        cout << "\nToo many parameters\n";
        cout << " ./bp [Instance] [Upper Bound]" << endl;
        exit(1);
    }

	instaceName = instance;
	nbOfPar = qtParam;
	dimension = -1;
}

void Data::read() {
	ifstream InBP(instaceName, ios::in);

    if (!InBP) {
        cerr << "File not found" << endl;
		exit(1);
    }

    InBP >> dimension; // Get number of nodes
    InBP >> capacity; // Get capacity

	weight = shared_ptr<double[]>(new double [dimension]); // Weights vector
    
    for (int i = 0; i < dimension; i++) {
        InBP >> weight[i];
    }

    InBP.close();
}

void Data::printCapacity() {
    cout << capacity << '\n';
}

void Data::printWeight() {
    for (int i = 0; i < dimension; i++) {
        cout << weight[i] << ' ';
    }
    cout << '\n';
}

string Data::getInstanceName() { // Get the name of instance

	string::size_type loc = instaceName.find_last_of(".", instaceName.size() );
	string::size_type loc2 = instaceName.find_last_of("/", instaceName.size() );

	string instance;

	if (loc != string::npos) {
		instance.append(instaceName, loc2+1, loc-loc2-1 );
	} else {
		instance.append(instaceName, loc2+1, instance.size() );
	}

	return instance;
}