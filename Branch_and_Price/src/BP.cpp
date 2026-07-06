#include "BP.h"

BP::BP(Data &data, double ub_) : instance(data), ub(ub_) {
    n = instance.getDimension();
    capacity = instance.getCapacity();

    weight.resize(n);
    
    for (int i = 0; i < n; i++)
        weight[i] = instance.getWeight(i);
}