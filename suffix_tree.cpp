using namespace std;

#include <iostream>
#include <map>
#include <algorithm>

class Node { 
    public:
    Node() {
        suffix_link = NULL;
    }
    Node(Node* parent, int start_index, int* end_index) {
        this->parent = parent;
        this->start_index = start_index;
        this->end_index = end_index;
        suffix_link = NULL;
    }     
    Node *parent;
    int start_index;
    int *end_index;
    Node *suffix_link;
    map<int, Node*> children;
    
    void add_child(int index, Node* child) {
        this->children[index] = child;
    }
    
    void remove_child(int index) {
        this->children.erase(index);
    }
    
    Node* split(int index) {
        auto new_child = new Node;
        new_child->parent = this->parent;
        new_child->start_index = this->start_index;
        new_child->end_index = new int(index);
        
        parent->remove_child(this->start_index);
        parent->add_child(this->start_index, new_child);

        this->parent = new_child;
        this->start_index = index+1;
        new_child->add_child(this->start_index, this);
        return new_child;
    }
};

// int end = -1;
int *endp = new int(-1);
int length;
string s;
Node* active_node;
int active_edge = -1;
int active_length = 0;
int remaining = 0;

int height(Node *root) {
    if(root == NULL) {
        return 0;
    }
    int h = 1;
    for(auto& [key, value] : root->children) {
        // cout << "damnn\n";
        h = max(h, 1+height(value));
    }
    return h;
}

void bfs_level(Node *root, int level, string s) {
    if(root == NULL) {
        return;
    }
    if(level==0) {
        cout << "N[" << s.substr(root->start_index, *(root->end_index)-root->start_index+1) << ", id-" << root << ", parid-" << root->parent 
        << ", slinkid-" << root->suffix_link << "]";
    } else if(level>0) {
        for(auto& [key, value] : root->children) {
           bfs_level(value, level-1, s);
        }
    }
}

void bfs(Node *root, string s) {
    cout << " -------- printing tree ---------\n";
    int h = height(root);
    // cout << "height is" << h << "\n";
    for(int i=0;i<h;i++) {
        bfs_level(root, i, s);
        cout << "----------- level ---------------\n";
    }
}

void change_active_node(Node *active_child, int length, string old, string s) {
    // cout << "changing active node \n";
    for(auto& [key, value] : active_child->children) {
        // cout << "string " << old << "," << length << "\n";
        if(s[key] == old[length]) {
            if(*(value->end_index) < key+active_length-length) {
                // active_length -= length;
                return change_active_node(value, length+(*(value->end_index)-value->start_index+1), old, s);
            } else {
                active_edge = key;
                active_node = active_child;
                active_length = active_length - length;
                return;
            }
        }
    }
}

void construct_tree(string s, Node *root) {
    length = s.length();
    active_node = root;
    for(auto i=0; i<length; i++) {
        (*endp)++;
        remaining++;
        Node* prev_created_internal_node = nullptr;
        // cout << "phase number " << i+1 << "\n";
        while(remaining) {
            int picked = active_edge;
            bool match = false;
            int match_key;
            Node *active_child;
            // cout << "active point " << active_edge  << "," << active_length << "," << active_node << ","<< "\n" ;
            // bfs(root, s);
            if(active_length!=0) {
                for(auto& [key, value] : active_node->children) {
                    if(s[active_edge] == s[key]) {
                        picked = key;
                        active_child = value;
                        break;
                    }
                }
                // active_child = active_node->children[active_edge];
                // cout << "here x " << active_edge  << "," << active_length << "," << active_node << ","<< "\n" ;
                if(*(active_child->end_index) >= picked+active_length  && s[i] == s[picked+active_length]) {
                    // cout << "here y " << active_length << "\n" ;
                    match = true;    
                } else if(*(active_child->end_index) < picked+active_length) {
                    // cout << "here z " << *(active_child->end_index) << "," << active_length << "\n" ;
                    string old = s.substr(active_edge, active_length);
                    change_active_node(active_child,(*(active_child->end_index)-active_child->start_index+1), old, s);
                    picked = active_edge;
                    // cout << "changed active point " << active_edge  << "," << active_length << "," << active_node << ","<< "\n" ;
                    active_child = active_node->children[active_edge];
                    if(s[i] == s[active_edge+active_length]) {
                        match = true;
                    }
                }
            } else {
                for(auto& [key, value] : active_node->children) {
                    if(s[i] == s[key]) {
                        active_edge = key;
                        active_child = active_node->children[active_edge];
                        picked = active_edge;
                        match = true;
                        break;
                    }
                }
            }
            // cout << "here 2 " << match << "\n" ;
            if(!match) {
                if(active_length==0) {
                    auto node = new Node(active_node, i, endp);
                    active_node->add_child(i, node);
                    // cout << "here 3 " << "\n" ;
                } else {
                    auto node = active_child->split(picked + active_length-1);
                    auto leaf = new Node(node, i, endp);
                    node->add_child(i, leaf);
                    node->suffix_link = root;
                    leaf->suffix_link = root;

                    if(prev_created_internal_node) {
                        // cout << "setting suff link" << "\n";
                        prev_created_internal_node->suffix_link = node;
                    }
                    prev_created_internal_node = node;
                    if(active_node != root) {
                        // cout << "from suff link" << "\n";
                        active_node = active_node->suffix_link;
                    } else {
                        active_length--;
                        active_edge++;
                    }
                }

                remaining--;
            } else {
                // RULE 3 extension - when a char is already there do nothing
                // update active length
                active_length++;
                break;
            }
        }
    }
}

int main() {
    Node *root = new Node(NULL, 0, new int(-1));
    string s = "xyzxyaxyz$";
    construct_tree(s, root);
    bfs(root, s);
  return 0;
}


// TEST CASES
// mississi
// xyzxyaxyz
// mississi$ - got seg fault for this