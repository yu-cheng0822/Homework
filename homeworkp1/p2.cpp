#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

class BST {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;

        Node(int k) : key(k), left(NULL), right(NULL) {}
    };

    Node* root;

    Node* insert(Node* node, int key);
    int height(Node* node);
    Node* deleteNode(Node* node, int key);
    Node* findMin(Node* node);

public:
    BST() { root = NULL; }

    void insert(int key);
    int getHeight();
    void remove(int key);

    void clear(Node* node);
   // ~BST();
};
BST::Node* BST::insert(Node* node, int key) {
    if (node == NULL)
        return new Node(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else
        node->right = insert(node->right, key);

    return node;
}

void BST::insert(int key) {
    root = insert(root, key);
}
int BST::height(Node* node) {
    if (node == NULL)
        return 0;

    int leftH = height(node->left);
    int rightH = height(node->right);

    return max(leftH, rightH) + 1;
}

int BST::getHeight() {
    return height(root);
}
BST::Node* BST::findMin(Node* node) {
    while (node->left != NULL)
        node = node->left;
    return node;
}

BST::Node* BST::deleteNode(Node* node, int key) {
    if (node == NULL) return NULL;

    if (key < node->key)
        node->left = deleteNode(node->left, key);
    else if (key > node->key)
        node->right = deleteNode(node->right, key);
    else {
       
        if (node->left == NULL) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        else if (node->right == NULL) {
            Node* temp = node->left;
            delete node;
            return temp;
        }

        Node* temp = findMin(node->right);
        node->key = temp->key;
        node->right = deleteNode(node->right, temp->key);
    }

    return node;
}
void BST::clear(Node* node) {
    if (node == NULL) return;
    clear(node->left);
    clear(node->right);
    delete node;
}
void BST::remove(int key) {
    root = deleteNode(root, key);
}
int main() {
    srand(time(NULL));

    int test[] = { 100, 500, 1000, 2000, 3000, 5000, 7000, 10000 };

    for (int i = 0; i < 8; i++) {
        int n = test[i];

        BST tree;

        for (int j = 0; j < n; j++) {
            int val = rand(); 
            tree.insert(val);
        }

        int h = tree.getHeight();
        double ratio = h / log2(n);

        cout <<n <<" " << h <<" " << ratio << endl;
    }
    BST tree;
    for (int i = 0; i < 10; i++) {
        int val = rand() % 100;
        cout << val << " ";

        tree.insert(val);
    }
    cout <<endl;
    cout <<tree.getHeight() << endl;

    int a;
    cin >> a;

    tree.remove(a);  
   
    cout << endl;
    cout <<tree.getHeight() << endl;
    
    return 0;
}