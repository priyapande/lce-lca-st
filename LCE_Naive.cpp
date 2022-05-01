/*
    Run: g++ LCE_Naive.cpp --std=c++17
*/
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

int LCE(string &input, int i, int j) {
    int matches = 0, N = input.length();

    while(i < N and j < N and input[i] == input[j]) {
        matches++;
        i++;
        j++;
    }

    return matches;
}

int get_alphabet_size(string filename) {
    int n = filename.length();
    int occ = 0;
    string alphabet_size = "";
    
    for(int i = 0; i < n; i++) {
    	
        if(filename[i] == '.') {
            break;
        }
        
        if(filename[i] == '_') {
            occ += 1;
        }
        
        else if(occ == 2) {
            alphabet_size += filename[i];
        }
        
    }

    return stoi(alphabet_size);
}



int main(int argc, char** argv) {
    int ALPHABET_SIZE = get_alphabet_size(string(ch));

    #ifndef LOCAL_TESTING
        freopen(argv[1], "r", stdin);
        freopen(argv[2], "w", stdout);
    #endif

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);cout.tie(NULL);

    string input; cin >> input;
    int N = input.length();

    int Q; cin >> Q;
    int Qt = 0;

    auto start_t = high_resolution_clock::now();

    for(int i = 1; i <= Q; i++) {
        int u, v; cin >> u >> v;

        u -= 1;
        v -= 1;

        auto start = high_resolution_clock::now();

        LCE(input, u, v);

        auto end = high_resolution_clock::now();
        auto duration_lce_query = duration_cast<microseconds>(end - start);

        Qt += duration_lce_query.count();

        // cout << LCE(input, u, v) << endl;
    }

    cout "ALPHABET_SIZE = " << ALPHABET_SIZE << "Q = " << Q << " N = " << N << " | LCE query time O(Q * N) = " << Qt << " microseconds" << endl;

    // auto duration_total = duration_cast<microseconds>(high_resolution_clock::now() - start_t);
    // cout << "Total time O(Q * N) = " << duration_total.count() << " microseconds" << endl;

    return 0;
}