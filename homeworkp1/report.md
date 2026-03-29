# 41343109

作業一之2

## 解題說明

二元樹搜尋

## 解題策略  

1. Node 結構  
   每個節點包含：
   int key：儲存資料  
    Node* left：左子節點  
    Node* right：右子節點  

2. insert() 
   使用遞迴方式：
    若 key 較小  插入左子樹  
    否則  插入右子樹  

3. height()  
   使用遞迴計算：
    高度 = max(左子樹, 右子樹) + 1  

4. deleteNode()  
   分三種情況：
    無子節點 → 直接刪除  
    一個子節點 → 用子節點取代  
    兩個子節點 → 找右子樹最小值取代  

5. findMin() 
   找右子樹最小值（最左節點）  

6. clear()  
   使用後序走訪刪除所有節點（避免記憶體洩漏）  

7. 主程式測試 
    建立不同大小的 BST  
    計算高度與 log₂(n) 比值  
    測試刪除節點前後的變化  


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

## 效能分析 (AI分析)

設 n 為樹中節點數，h 為樹高。

| 函式 | 時間複雜度 | 空間複雜度 |
|:---:|:---:|:---:|
| insert | O(h) | O(h) |
| height | O(n) | O(h) |
| delete | O(h) | O(h) |
| findMin | O(h) | O(1) |
| clear | O(n) | O(h) |
| main（整體） | 平均 O(n log n)，最壞 O(n²) | O(n) |

### 複雜度說明
1. 在隨機情況下，BST 高度 h ≈ log₂(n)，因此：
    insert / delete 約為 O(log n)
2. 在最壞情況（資料已排序）下：
    BST 會退化成鏈結串列
    h = n，因此操作為 O(n)
3. height() 必須走訪所有節點，因此為 O(n)

---

## 測試與驗證  

### 測試方式  
1. 使用亂數產生 n 筆資料插入 BST  
2. 計算樹高 h  
3. 比較 h / log₂(n)  

### 測試結果（範例）

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
