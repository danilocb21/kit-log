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
        maxback_val[p] = 0.0;
    
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

            maxback_val[k] = 0.0;
    
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

pair<double, pair<int, int>> MaxBack2(vector<vector<double>> const &x, int n, int ct_merged, vector<bool> const &merged) {
    
    vector<bool> in_set(n);
    int p = 0;
    for (; p < n; p++) if (!merged[p]) break;
    in_set[p] = true;
    
    // O cut_val sera o valor 
    // do maxback no ultimo nó
    vector<double> maxback_val(n);

    for (int j = 0; j < p; j++) {
        if (!merged[j])
        maxback_val[j] += x[j][p];
    }
    for (int j = p + 1; j < n; j++) {
        if (!merged[j])
        maxback_val[j] += x[p][j];
    }

    int s, t = p;

    for (int k = 0; k < n - ct_merged - 1; k++) {
        s = t;
        
        double max_val = -1.0;
        for (int j = 0; j < n; j++) {
            if (!merged[j] && !in_set[j] && max_val < maxback_val[j] - EPSILON) {
                max_val = maxback_val[j];
                t = j;
            }
        }
        in_set[t] = true;
        
        for (int j = 0; j < t; j++) {
            if (in_set[j] || merged[j]) continue;
            maxback_val[j] += x[j][t];
        }
        for (int j = t + 1; j < n; j++) {
            if (in_set[j] || merged[j]) continue;
            maxback_val[j] += x[t][j];
        }

        if (k < n - ct_merged - 2)
            maxback_val[t] = 0.0;
    }

    return { maxback_val[t], { s, t }};
}

vector<vector<int>> MinCut(double** x, int n) {
    vector<vector<double>> x_new(n, vector<double>(n));
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            x_new[i][j] = x[i][j];
        }
    }
    
    vector<vector<int>> cut_sets;

    vector<vector<int>> nodes(n);
    for (int i = 0; i < n; i++) nodes[i] = { i };

    vector<bool> is_merged(n);
    int count_merged = 0;
    
    for (int k = 0; k < n - 1; k++) {
        auto res = MaxBack2(x_new, n, count_merged, is_merged);

        double cut_val = res.first;
        int s = res.second.first, t = res.second.second;

        if (cut_val < 2.0 - EPSILON && nodes[t].size() > 1 && nodes[t].size() < n - 1) {
            cut_sets.push_back(nodes[t]);
        }

        // Shrink(G, x, s, t);
        is_merged[t] = true;
        count_merged++;
        for (int nd : nodes[t])
            nodes[s].push_back(nd);
        nodes[t].clear();

        for (int i = 0; i < s; i++) {
            if (is_merged[i]) continue;
            if (i < t)
                x_new[i][s] += x_new[i][t];
            else
                x_new[i][s] += x_new[t][i];
        }

        for (int i = s + 1; i < n; i++) {
            if (is_merged[i]) continue;
            if (i < t)
                x_new[s][i] += x_new[i][t];
            else
                x_new[s][i] += x_new[t][i];
        }
    }

    return cut_sets;
}