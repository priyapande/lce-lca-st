/*
    Run: g++ LCE.cpp --std=c++17
*/
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

const char EOP_CHAR = '#'; // End of path character
int MAX_D = INT_MIN;

struct End {
    int val;
    End(int v) : val(v) {}
};


struct SuffixNode {
    unordered_map <char, SuffixNode*> child;

    int start, index, sdepth;
    End* end;
    SuffixNode* suffixLink;

    SuffixNode(int s, End* e) {
        start = s;
        end = e;
        suffixLink = NULL;
        index = 0;
        sdepth = 0;
    }
};


struct ActivePoint {
    SuffixNode* activeNode;
    int activeLength, activeEdge;

    ActivePoint(SuffixNode* node) {
        activeNode = node;
        activeLength = 0;
        activeEdge = -1;
    }
};


class SuffixTree {

private:
    string input;
    SuffixNode* root; 
    int remaining, N;
    End* end;
    ActivePoint* active;
    int D;
    unordered_map <SuffixNode*, unordered_map<int, SuffixNode*>> store;
    unordered_map <SuffixNode*, int> nodeLevelMap;
    unordered_map <int, SuffixNode*> leafIdxMap;

public:
    SuffixTree(string &s) {
        input = (s + '$');
        root = NULL;
        remaining = 0;
        N = input.length();
    }

    SuffixNode* getCurrNode() {
        return active->activeNode->child[input[active->activeEdge]];
    }

    int diff(SuffixNode* node) {
        return node->end->val - node->start;
    }

    char nextChar(int idx) {

        SuffixNode* node = getCurrNode();

        if(diff(node) >= active->activeLength) {
            return input[node->start + active->activeLength];
        }

        if(diff(node) + 1 == active->activeLength) {
            if(node->child.count(input[idx])) {
                return input[idx];
            }
        }
        else {
            active->activeNode = node;
            active->activeLength = active->activeLength - diff(node) - 1;
            active->activeEdge = active->activeEdge + diff(node) + 1;
            
            return nextChar(idx);
        }

        return EOP_CHAR;
    }

    void walkDown(int idx) {

        SuffixNode* node = getCurrNode();

        if(diff(node) < active->activeLength){
            active->activeNode = node;
            active->activeLength = active->activeLength - diff(node);
            active->activeEdge = node->child[input[idx]]->start;
        }
        else{
            active->activeLength += 1;
        }
    }

    void startPhase(int idx) {
        SuffixNode* lastCreatedInternalNode = NULL;

        end->val += 1;
        remaining += 1;

        while(remaining > 0) {
            if(active->activeLength == 0) {
                SuffixNode* currNode = NULL;

                if(active->activeNode->child.count(input[idx])) {
                    currNode = active->activeNode->child[input[idx]];
                }

                if(currNode) {
                    active->activeEdge = currNode->start;
                    active->activeLength += 1;
                    break;
                }
                else {
                    root->child[input[idx]] = new SuffixNode(idx, end);
                    remaining -= 1;
                }
            }
            else {
                char ch = nextChar(idx);
                
                if(ch != EOP_CHAR) {
                    if(ch == input[idx]) {
                        if(lastCreatedInternalNode){
                            lastCreatedInternalNode->suffixLink = getCurrNode();
                        }

                        walkDown(idx);
                        break;
                    }
                    else {
                        SuffixNode* node = getCurrNode();

                        int oldStart = node->start;
                        node->start += active->activeLength;

                        // Create new internal node
                        SuffixNode* newInternalNode = new SuffixNode(oldStart, new End(oldStart + active->activeLength - 1));

                        // Create new leaf node
                        SuffixNode* newLeafNode = new SuffixNode(idx, end);

                        // Set internal nodes child as old node and new leaf node.
                        newInternalNode->child[input[newInternalNode->start + active->activeLength]] = node;
                        newInternalNode->child[input[idx]] = newLeafNode;
                        newInternalNode->index = -1;
                        active->activeNode->child[input[newInternalNode->start]] = newInternalNode;


                        if (lastCreatedInternalNode) {
                            lastCreatedInternalNode->suffixLink = newInternalNode;
                        }
                        
                        lastCreatedInternalNode = newInternalNode;
                        newInternalNode->suffixLink = root;

                        // If active node is not root then follow suffix link
                        if(active->activeNode != root){
                            active->activeNode = active->activeNode->suffixLink;
                        }

                        // If active node is root then increase active index by one and decrease active length by 1
                        else {
                            active->activeEdge += 1;
                            active->activeLength -= 1;
                        }

                        remaining -= 1;
                    }
                }
                else {
                    SuffixNode* node = getCurrNode();

                    node->child[input[idx]] = new SuffixNode(idx, end);

                    if (lastCreatedInternalNode) {
                        lastCreatedInternalNode->suffixLink = node;
                    }
                    lastCreatedInternalNode = node;

                    // If active node is not root then follow suffix link
                    if(active->activeNode != root){
                        active->activeNode = active->activeNode->suffixLink;
                    }

                    // If active node is root then increase active index by one and decrease active length by 1
                    else{
                        active->activeEdge += 1;
                        active->activeLength -= 1;
                    }

                    remaining -= 1;
                }
            }
        }
    }


    void assignSuffixIndexes(SuffixNode* root, int len, int level) {
        if(!root) {
            return;
        }
        
        MAX_D = max(level, MAX_D);
        len += root->end->val - root->start + 1;
        root->sdepth = len;

        // If leaf node, all internal nodes have root->index = -1
        if(root->index != -1) { 
            root->index = N - len;
            leafIdxMap[root->index] = root;
            return;
        }
        
        for(auto [ch, childNode] : root->child) {
            assignSuffixIndexes(childNode, len, level + 1);
        }
    }   



    void buildTree() {
        root = new SuffixNode(1, new End(0));
        root->index = -1;

        active = new ActivePoint(root);
        end = new End(-1);

        for(int i = 0; i < N; i++) {
            startPhase(i);
        }

        if(remaining != 0) {
            cout << "Something wrong happened" << endl;
        }

        assignSuffixIndexes(root, 0, 1);
        
        D = log2(MAX_D) + 1;
    }
    

    /*
        SUFFIX TREE VALIDATION
    */
    void debug(string err, int idx, int curr) {
        cout << err << " Failed at : " << curr << " at index : " << idx << endl;
    }

    bool validateSuffixTreeHelper(SuffixNode* root, int idx, int curr) {
        if(!root) {
            debug("[Root is NULL]", idx, curr);
            return false;
        }

        if(root->index != -1) {
            if(root->index != idx){
                debug("[Index not same] 1", idx, curr);
                return false;        
            }
            else{
                return true;
            }
        }

        if(curr >= N){
            debug("[Index not same] 2", idx, curr);
            return false;        
        }
        
        SuffixNode* node = root->child[input[curr]];

        if(node == NULL){
            debug("[Index not same] 3", idx, curr);
            return false;
        }

        int j = 0;
        for(int i = node->start; i <= node->end->val; i++){
            if(input[curr + j] != input[i] ) {
                cout << "[Mismatch found] " << input[curr+j] << " and " << input[i] << endl;
                return false;
            }
            j++;
        }

        curr += node->end->val - node->start + 1;

        return validateSuffixTreeHelper(node, idx, curr);
    }

    bool validateSuffixTree() {
        for(int i = 0; i < N; i++) {
            if(!validateSuffixTreeHelper(root, i, i)) {
                return false;
            }
        }

        return true;
    }

    void dfsHelper(SuffixNode* root, vector <char> &res) {
        if(!root) return;

        if(root->index != -1) {
            for(int i = root->start; i <= root->end->val; i++) {
                res.push_back(input[i]);
            }

            cout << root->index << endl;

            string suffix = "";
            for(auto r : res) suffix += r;

            cout << suffix << endl;
            cout << endl;

            for(int i = root->start; i <= root->end->val; i++) {
                res.pop_back();
            }
            return;
        }

        for(int i = root->start; i <= root->end->val; i++) {
            res.push_back(input[i]);
        }

        for(auto [ch, childNode] : root->child) {
            dfsHelper(childNode, res);
        }
        
        for(int i = root->start; i <= root->end->val; i++) {
            res.pop_back();
        }
    }

    void dfs() {
        vector <char> res;

        for(auto [ch, childNode] : root->child) {
            dfsHelper(root, res);
        }
    }

    /* 
        LCA methods on suffix tree
    */
    void preprocessLCAHelper(SuffixNode* node, SuffixNode* parent, int level) {
        nodeLevelMap[node] = level;
        store[node][0] = parent;

        for(int i = 1; i <= D; i++) {
            if(store[node][i - 1]) {
                store[node][i] = store[store[node][i - 1]][i - 1];
            }
            else {
                store[node][i - 1] = NULL;
            }
        }

        for(auto [ch, childNode] : node->child) {
            preprocessLCAHelper(childNode, node, level + 1);
        }
    }

    SuffixNode* liftNode(SuffixNode* node, int K) { 
        if(!node or K == 0) {
            return node;
        }
    
        for(int i = D; i >= 0; i--) {
            if((1 << i) <= K) {
                return liftNode(store[node][i], K - (1 << i));
            }
        }
    
        return NULL;
    }

    void se(SuffixNode* node) {
        cout << node->start << " " << node->end->val << endl; 
    }

    SuffixNode* LCA(SuffixNode* node1, SuffixNode* node2) { 
        if(node1 == node2) return node1;

        for(int i = D; i >= 0; i--) {
            if(store[node1][i] != store[node2][i]) {
                node1 = store[node1][i];
                node2 = store[node2][i];
            }
        }

        return store[node1][0];
    }

    int LCE(int i, int j) {
        SuffixNode* node1 = leafIdxMap[i], *node2 = leafIdxMap[j];
        SuffixNode* toLiftNode = NULL, *otherNode = NULL;

        if(nodeLevelMap[node1] > nodeLevelMap[node2]) {
            toLiftNode = node1;
            otherNode = node2;
        }
        else {
            toLiftNode = node2;
            otherNode = node1;
        }
    
        int diff = abs(nodeLevelMap[node1] - nodeLevelMap[node2]);
        SuffixNode* liftedNode = liftNode(toLiftNode, diff);
        SuffixNode* LCANode = LCA(liftedNode, otherNode);

        return LCANode->sdepth;
    } 

    void preprocessLCA() {
        preprocessLCAHelper(root, NULL, 0);
    }
};


int main(int argc, char** argv) {
    #ifndef LOCAL_TESTING
        freopen(argv[1], "r", stdin);
        freopen(argv[2], "w", stdout);
    #endif

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);cout.tie(NULL);

    string input; cin >> input;
    int N = input.length();

    auto start = high_resolution_clock::now();
    auto start_t = start;

    SuffixTree st(input);
    st.buildTree();

    auto end = high_resolution_clock::now();
    auto duration_build_suffix_tree = duration_cast<milliseconds>(end - start);

    cout << "Suffix tree construction time O(N): " << duration_build_suffix_tree.count() << " ms" << endl;

    start = high_resolution_clock::now();
    st.preprocessLCA();

    end = high_resolution_clock::now();
    auto duration_lca_preprocessing = duration_cast<milliseconds>(end - start);

    cout << "LCA pre-processing time O(NlogN): " << duration_lca_preprocessing.count() << " ms" << endl;

    
    int Q; cin >> Q;

    start = high_resolution_clock::now();

    for(int i = 1; i <= Q; i++) {
        int u, v; cin >> u >> v;

        u -= 1;
        v -= 1;

        st.LCE(u, v);

        // cout << st.LCE(u, v) << endl;
    }

    end = high_resolution_clock::now();
    auto duration_lce_query = duration_cast<milliseconds>(end - start);
    cout << "Q = " << Q << " N = " << N << " LCE query time O(Q * logN): " << duration_lce_query.count() << " ms" << endl;

    auto duration_total = duration_cast<milliseconds>(end - start_t);
    cout << "Total time O(N + NlogN + QlogN) : " << duration_total.count() << " ms" << endl;

    // st.dfs();

    // if(st.validateSuffixTree()) {
    //     cout << input << " - Validated!" << endl;
    // }
    // else {
    //     cout << input << " Validation failed!" << endl;
    // }

    return 0;
}