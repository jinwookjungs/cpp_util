/**
 * @file    subset_sum.cpp
 * @author  Jinwook Jung (jinwookjung@kaist.ac.kr)
 * @date    2018-02-06 22:58:10
 *
 * Created on Tue Feb  6 22:58:10 2018.
 */

#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

using namespace std;

const double DELTA = 0.2;     // Trimming parameter, (0,1)
const double EPS = 0.1;       // Approximation parameter, (0,1)

/**
 * Returns the sorted list that is the merge of its two sorted input lists,
 * with duplicates removed.
 */
template <typename T>
vector<T> merge_lists (vector<T> l_1, vector<T> l_2)
{
    vector<T> l(l_1);
    l.insert(l.end(), l_2.begin(), l_2.end());

    sort(l.begin(), l.end());
    l.erase(unique(l.begin(), l.end()), l.end());

    return l;
}

/**
 * If two values in l are close to each other, we do not need to maintain both
 * of them explicitly because we want just an approximate solution.
 */
template <typename T>
void trim (vector<T>& l, const double delta)
{
    vector<T> l_new {l[0]};
    auto last = l_new[0];

    for (size_t i = 1; i < l.size(); i++) {
        if (l[i] > last * (1 + delta)) {
            // l_new[i] >= last because l is sorted
            l_new.emplace_back(l[i]);
            last = l[i];
        }
    }

    l = std::move(l_new);
}


template <typename T>
vector<T> remove_large_elements (vector<T>& l, const T t)
{
    vector<T> l_new;
    l_new.reserve(l.size());

    for (auto v : l) {
        if (v > t) {
            continue;
        }
        l_new.emplace_back(v);
    }

    return l_new;
}

/**
 * Returns a value z whose value is within a 1+eps factor of the optimal.
 */
template <typename T>
T approx_subset_sum (vector<T>& S, const T t, const double eps)
{
    const auto N = S.size();
    const auto delta = eps / 2 / N;
    vector<T> l_0 {0};

    vector<T> l_prev = l_0;
    for (size_t i = 0; i < N; i++) {
        auto l_temp(l_prev);
        transform(l_temp.begin(), l_temp.end(), l_temp.begin(),
                  bind2nd(std::plus<T>(), S[i]));

        auto l_i = merge_lists(l_prev, l_temp);
        trim(l_i, delta);

        // Remove from l_i every element that is greater than t
        l_i = remove_large_elements(l_i, t);
        l_prev = std::move(l_i);
    }

    return l_prev.back();
}

int main (void)
{
    vector<double> example {10, 11, 12, 15, 20, 21, 22, 23, 24, 29};
    // trim (example, DELTA);

    for (auto d : example) {
        cout << d << " ";
    }
    cout << endl;

    vector<int> S{104, 102, 201, 101};
    auto z = approx_subset_sum(S, 308, 0.40);
    cout << z << endl;

    return 0;
}
