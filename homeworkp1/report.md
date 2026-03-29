# 41343109

作業一之2

## 解題說明

二元樹搜尋

### 解題策略

1. class Term
儲存單一項的資料結構：float coef; int exp;。

2. Polynomial::Polynomial()
建構子：初始化動態陣列（初始 capacity = 2）與項數為 0。

3. Polynomial::~Polynomial()
解構子：釋放動態配置的 termArray。

4. void Polynomial::look(int n)
確保內部陣列至少有容量 n，不足時擴增並複製既有項目。

5. void Polynomial::newTerm(const float newcoef, int newexp)
新增一項（跳過係數為 0），若容量不足自動擴增並將項附加到陣列末端。

6. Polynomial Polynomial::Add(const Polynomial &poly)
回傳兩多項式相加的結果：先複製目前物件的項，再把 poly 的項合併（相同指數則累加係數）。

7. Polynomial Polynomial::mult(const Polynomial &poly)
回傳兩多項式相乘的結果：對每對項做相乘（係數相乘、指數相加），合併相同指數的項。

8. float Polynomial::Eval(float f)
在 x = f 評值多項式，對每項計算 coef * pow(f, exp) 並累加回傳總和。

9. istream& operator>>(istream& in, Polynomial& poly)
從輸入讀入多項式：先讀入項數 n，接著讀 n 組 coef exp 並呼叫 newTerm 插入。

10. ostream& operator<<(ostream& out, const Polynomial& poly)
輸出多項式為可讀字串：處理正負號與指數為 0 的情況，依陣列順序輸出每一項。



## 程式實作

以下為主要程式碼：

```cpp

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

    cout <<tree.getHeight() << endl;
    
    return 0;
}

```

## 效能分析(AI分析)
| 函式 | 時間複雜度 | 空間複雜度 |
|:---:|:---:|:---:|
| Constructor / Destructor | O(1) | O(1) |
| look(int n) |  O(1) |  O(capacity) | 
| newTerm(const float, int) | O(1) | O(1) | 
| Add(const Polynomial&) | O(m*n + m^2) | O(n + m) |
| mult(const Polynomial&) | O((n*m)^2) | O(n * m) |
| Eval(float) | O(n)（若 pow 當 O(1)） | O(1) | 
| operator>>(istream&, Polynomial&) | O(k) | O(k) | 
| operator<<(ostream&, const Polynomial&) | O(n) | O(1) | 
| main（整體） | O(m*n + m^2 + (n*m)^2 + n) | O(n*m + n + m) | 


## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數  |
|----------|--------------|
| 測試一   | <img width="397" height="291" alt="image" src="https://github.com/user-attachments/assets/cd0f8d9b-4ae8-46ec-9f5b-c2cf2756b112" />     | 
| 測試二   |   <img width="364" height="275" alt="image" src="https://github.com/user-attachments/assets/0839cd87-4566-446b-85dc-6a7ff6be871c" /> | 




### 結論
1. 功能正確：程式實作了 Polynomial 類別，新增項（newTerm）、相加（Add）、相乘（mult）、附值（Eval）以及輸入/輸出運算子，能完成多項式的基本運算與顯示。
2. 記憶體管理：使用動態陣列（look/newTerm），可正確保留既有項目並避免儲存係數為 0 的項。
3. 複雜度摘要（設 n = this.terms, m = poly.terms）：
Add：O(m*n + m^2)，空間 O(n + m)。
mult： O((nm)^2)，空間 O(nm)。
Eval：O(n)，空間 O(1)。

## 心得討論
在寫add 和 mult函式時出現了"block 0x000001e5087e9dd0"這個錯誤導致無法成功執行 上網查資料說C++ 預設是「淺拷貝」，容易導致記憶體重複刪除。
因此改成用「指標參考」傳遞，而不是「值傳遞」這樣就能解決問題;
