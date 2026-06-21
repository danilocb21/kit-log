#ifndef CONSTRUCTION
#define CONSTRUCTION

struct construction
{
    static double construc(double **costs, int n);
};

struct InsertionInfo {
    int inserted_node;
    double cost;

    bool operator<(const InsertionInfo &p) const {
        return cost < p.cost;
    }

    InsertionInfo(int inserted_node, double cost) : 
        inserted_node(inserted_node), cost(cost) {}
};


#endif