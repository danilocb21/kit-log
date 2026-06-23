#include "separation.h"

vector<vector<int>> MaxBack(double** x, int n) {

    vector<vector<int>> cut_sets;
    
    vector<bool> cut_node(n);
    
    for (int p = 0; p < n; p++) {

        if (cut_node[p]) continue;

        vector<int> s = { p };
    
        vector<int> s_cur = { p };
        s_cur.reserve(n);
    
        vector<bool> in_set(n);
        in_set[p] = true;
        
        vector<double> maxback_val(n);
        maxback_val[p] = -1.0;
    
        double min_cut;
        double cut_val = 0.0;
        for (int j = 0; j < p; j++) {
            maxback_val[j] += x[j][p];
            cut_val += x[j][p];
        }
        for (int j = p + 1; j < n; j++) {
            maxback_val[j] += x[p][j];
            cut_val += x[p][j];
        }
    
        min_cut = cut_val;
    
        for (int i = 0; i < n - 2; i++) {
            int k = max_element(maxback_val.begin(), maxback_val.end()) - maxback_val.begin();
            s_cur.push_back(k);
            in_set[k] = true;
            
            double delta_k = 0.0;
            for (int j = 0; j < k; j++) {
                delta_k += x[j][k];
                if (!in_set[j]) maxback_val[j] += x[j][k];
            }
            for (int j = k + 1; j < n; j++) {
                delta_k += x[k][j];
                if (!in_set[j]) maxback_val[j] += x[k][j];
            }
    
            cut_val += delta_k - 2 * maxback_val[k];

            maxback_val[k] = -1.0;
    
            if (cut_val < min_cut) {
                min_cut = cut_val;
                s = s_cur;
            }
        }
    
        if (min_cut < 2.0 - EPSILON) {
            cut_sets.push_back(s);
            for (int x : s) cut_node[x] = true;
        }
    }

    return cut_sets;
}

vector<vector<int>> MinCut(double** x, int n) {
    return {};
}