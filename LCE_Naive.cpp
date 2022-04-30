/*
    Run: g++ LCE_Naive.cpp --std=c++17
*/
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

int LCE(string &input, int i, int j) {
    int matches = 0, N = input.length();

    while(i < N and j < N) {
        if(input[i] == input[j]) {
            matches++;
            i++;
            j++;
        }
        else {
            break;
        }
    }

    return matches;
}


int main(int argc, char** argv) {
    #ifndef LOCAL_TESTING
        freopen(argv[1], "r", stdin);
        freopen(argv[2], "w", stdout);
    #endif

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);cout.tie(NULL);

    string input; cin >> input;
    int N = input.length();

    int Q; cin >> Q;

    auto start = high_resolution_clock::now();
    auto start_t = start;

    for(int i = 1; i <= Q; i++) {
        int u, v; cin >> u >> v;

        u -= 1;
        v -= 1;

        LCE(input, u, v);

        // cout << LCE(input, u, v) << endl;
    }

    auto end = high_resolution_clock::now();
    auto duration_lce_query = duration_cast<milliseconds>(end - start);

    cout << "Q = " << Q << " N = " << N << " LCE query time O(Q * N): " << duration_lce_query.count() << " ms" << endl;

    auto duration_total = duration_cast<milliseconds>(end - start_t);
    cout << "Total time O(Q * N) : " << duration_total.count() << " ms" << endl;

    return 0;
}