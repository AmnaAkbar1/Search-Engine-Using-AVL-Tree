#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
    string keyword;
    vector<string> urls;
    Node* left;
    Node* right;
    int height; //height of the node in the tree

    Node(const string& key, const string& url) : keyword(key), left(nullptr), right(nullptr), height(1) {
        urls.push_back(url);
    }
};

class AVLTree {
private:
    Node* root;

    int getHeight(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    int getBalanceFactor(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return getHeight(node->left) - getHeight(node->right);
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    Node* insertNode(Node* node, const string& key, const string& url) {
        if (node == nullptr) {
            return new Node(key, url);
        }

        if (key < node->keyword) {
            node->left = insertNode(node->left, key, url);
        }
        else if (key > node->keyword) {
            node->right = insertNode(node->right, key, url);
        }
        else {
            node->urls.push_back(url); //keyword already exists, add URL to existing node
            return node;
        }

        //update height of the current node
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        //get the balance factor of this node to check for imbalance
        int balance = getBalanceFactor(node);

        //perform rotations if the node becomes unbalanced

        //left Left Case
        if (balance > 1 && key < node->left->keyword) {
            return rightRotate(node);
        }
        //right right Case
        if (balance < -1 && key > node->right->keyword) {
            return leftRotate(node);
        }
        //Left Right Case
        if (balance > 1 && key > node->left->keyword) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        //Right Left Case
        if (balance < -1 && key < node->right->keyword) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    vector<string> searchNode(Node* node, const string& key) {
        vector<string> result;
        if (node == nullptr) {
            return result;
        }

        if (key < node->keyword) {
            return searchNode(node->left, key);
        }
        else if (key > node->keyword) {
            return searchNode(node->right, key);
        }
        else {
            result = node->urls;

            //Search in both left and right subtrees for the same keyword
            vector<string> leftResult = searchNode(node->left, key);
            vector<string> rightResult = searchNode(node->right, key);

            //Merge results from left and right subtrees with the current node's URLs
            result.insert(result.end(), leftResult.begin(), leftResult.end());
            result.insert(result.end(), rightResult.begin(), rightResult.end());
        }
        return result;
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(const string& key, const string& url) {
        root = insertNode(root, key, url);
    }

    vector<string> search(const string& key) {
        return searchNode(root, key);
    }
};

//read data from file and build AVL tree
AVLTree readDataFromFile(const string& filename) {
    ifstream file(filename);
    AVLTree avlTree;

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            string id, url, keywords;
            if (!line.empty()) {
                istringstream iss(line);
                iss >> id >> url;
                getline(file, keywords);

                istringstream kss(keywords);
                string keyword;
                while (kss >> keyword) {
                    avlTree.insert(keyword, url);
                }
            }
        }
        file.close();
    }
    return avlTree;
}


void searchInterface(AVLTree& avlTree) {
    string keyword;
    cout << "Please enter a word to search: ";
    cin >> keyword;

    vector<string> results = avlTree.search(keyword);
    cout << results.size() << " result(s) found\n";
    for (const string& result : results) {
        cout << result << endl;
    }
}

int main() {
    string filename;

    cout << endl << endl;
    cout << "\t\t" << "  GGGGG   OOOOO   OOOOO   GGGGG  L       EEEEE" << endl;
    cout << "\t\t" << " G       O     O O     O G       L       E    " << endl;
    cout << "\t\t" << " G  GGGG O     O O     O G  GGG  L       EEEE " << endl;
    cout << "\t\t" << " G    GG O     O O     O G    G  L       E    " << endl;
    cout << "\t\t" << "  GGGGGG  OOOOO   OOOOO   GGGGG  LLLLLLL EEEEE" << endl;
    cout << endl << endl;

    cout << "Please enter a filename: ";
    cin >> filename; //Assignment_data.txt

    AVLTree avlTree = readDataFromFile(filename);
    cout << "File loaded successfully.\n";

    searchInterface(avlTree);

    return 0;
}
