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


int main() {
    #ifndef LOCAL_TESTING
        freopen("in.txt", "r", stdin);
        freopen("output_naive.txt", "w", stdout);
    #endif

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);cout.tie(NULL);

    string input; cin >> input;

    int Q; cin >> Q;

    for(int i = 1; i <= Q; i++) {
        int u, v; cin >> u >> v;

        u -= 1;
        v -= 1;

        cout << LCE(input, u, v) << endl;
    }

    return 0;
}