#include "BP.h"
#include "node.h"

bool Node::is_feasible() {
    // most_fract_val == 0
    return abs(most_fract_val) <= EPS;
}