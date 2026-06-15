# 41343109

作業一之1  

## 題目說明  

Min Heap實作  

---

## 解題策略  

1. MinPQ 抽象類別  
   定義優先佇列的基本操作介面：  
   - IsEmpty()：判斷是否為空  
   - Top()：取得最小元素  
   - Push()：插入元素  
   - Pop()：刪除最小元素  

---

2. minheap 類別實作

使用陣列來實作最小堆，並維持以下性質：  
- 每個節點都小於等於其子節點 

---

3. 資料結構設計  

- heap[]：儲存堆的陣列從 index 1 開始  
- size：目前元素數量  
- capacity：陣列容量  

---

4. Push() 插入操作（上浮） 

步驟：  
1. 將新元素放到最後  
2. 與父節點比較  
3. 若較小則交換  
4. 直到滿足 Min Heap 性質  

時間複雜度：O(log n)

---

5. Pop() 刪除操作（下沉）  

步驟：  
1. 將最後一個元素移到根節點  
2. 與較小的子節點比較  
3. 若較大則交換 
4. 直到滿足 Min Heap 性質  

時間複雜度：O(log n)

---

6. resize() 動態擴充  

當陣列滿時：  
 容量擴大為原本的 2 倍  
 將舊資料複製到新陣列  

時間複雜度：O(n)

---

7. Top() 取得最小值  

 直接回傳根節點heap[1]  
 時間複雜度：O(1)  

---

8. 主程式測試  

- 插入多個整數  
- 測試最小值輸出  
- 逐一刪除並觀察排序結果  

---

## 程式實作  

```cpp
#include <iostream>
using namespace std;

template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};

template <class T>
class minheap : public MinPQ<T> {
private:
    T *heap;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        T* nheap = new T[capacity];
        for (int i = 1; i <= size; i++) {
            nheap[i] = heap[i];
        }
        delete[] heap;
        heap = nheap;
    }

public:
    minheap(int cap = 10) {
        capacity = cap;
        size = 0;
        heap = new T[capacity];
    }

    ~minheap() { delete[] heap; }

    bool IsEmpty() const { return size == 0; }

    const T& Top() const {
        if (IsEmpty()) throw "Empty Heap";
        return heap[1];
    }

    void Push(const T& n) {
        if (size + 1 == capacity)
            resize();

        heap[++size] = n;
        int current = size;

        while (current > 1 && heap[current] < heap[current / 2]) {
            swap(heap[current], heap[current / 2]);
            current /= 2;
        }
    }

    void Pop() {
        if (IsEmpty()) throw "Empty Heap";

        heap[1] = heap[size--];
        int current = 1;

        while (2 * current <= size) {
            int child = 2 * current;

            if (child + 1 <= size && heap[child + 1] < heap[child])
                child++;

            if (heap[current] <= heap[child]) break;

            swap(heap[current], heap[child]);
            current = child;
        }
    }
};

int main() {
    minheap<int> h;

    h.Push(5);
    h.Push(3);
    h.Push(8);
    h.Push(1);
    h.Push(6);

    cout << h.Top() << endl;

    while (!h.IsEmpty()) {
        cout << h.Top() << " ";
        h.Pop();
    }
    cout << endl;

    return 0;
}
```
## 效能分析

設 n 為元素數量，h 為堆高（h ≈ log₂(n)）

| 函式 | 時間複雜度 | 空間複雜度 |
|:---:|:---:|:---:|
| Push | O(log n) | O(1) |
| Pop | O(log n) | O(1) |
| Top | O(1) | O(1) |
| resize | O(n) | O(n) |
| main（整體） | O(n log n) | O(n) |

---

### 複雜度說明  

1. Min Heap 為完全二元樹，因此高度為：  
   h ≈ log₂(n)  

2. 插入（Push）與刪除（Pop）  
   需要沿著樹上下移動，因此為 O(log n)  

3. Top()  
   直接存取根節點，因此為 O(1)  

4. resize()  
   需要複製整個陣列，因此為 O(n)  

---

## 測試與驗證  

### 測試方式  

1. 插入多筆資料  
2. 呼叫 Top() 檢查最小值  
3. 持續 Pop() 並輸出結果  
### 測試結果（範例）

| 測試案例 | 輸入參數  |
|----------|--------------|
| 測試一   | <img width="422" height="180" alt="image" src="https://github.com/user-attachments/assets/d30e37de-a67c-45bc-9d37-0de5abae2829" />| 
---

### 結果分析  

- 輸出結果會呈現由小到大排序 
- 表示 Min Heap 正確維持結構  
- 每次取出的都是當前最小值  

---

## 結論  

1. 本程式成功實作最小優先佇列（Min Heap）  
2. 插入與刪除操作效率為 O(log n)  
3. 可有效用於排序、排程等應用  
4. 使用陣列實作可提升空間與存取效率  

---

## 心得討論  

在這次實作中，較具挑戰的是維持 Min Heap 的結構性質，特別是在插入與刪除時，需要透過「上浮」與「下沉」來調整節點位置。
透過實驗可以觀察到，每次取出的元素都是目前最小值，顯示資料結構確實正確運作。這也讓我更理解優先佇列在演算法中的重要性，例如排序與圖論演算法等應用。

---



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
    無子節點  直接刪除  
    一個子節點  用子節點取代  
    兩個子節點  找右子樹最小值取代  

5. findMin() 
   找右子樹最小值最左節點 

6. clear()  
   使用後序走訪刪除所有節點避免記憶體洩漏  

7. 主程式測試 
    建立不同大小的 BST  
    計算高度與 log₂(n) 比值  
    測試刪除節點前後的變化  


## 程式實作

以下為主要程式碼：

```cpp

#include <iostream>
#include <cmath>      
#include <cstdlib>    // rand()
#include <ctime>      // time()
using namespace std;

class BST {
private:
    struct Node {
        int key;        // 節點值
        Node* left;     // 左子樹
        Node* right;    // 右子樹

        Node(int k) : key(k), left(NULL), right(NULL) {} // 建構子
    };

    Node* root; // 樹的根節點

    Node* insert(Node* node, int key);   // 插入節點 遞迴
    int height(Node* node);              // 計算高度
    Node* deleteNode(Node* node, int key); // 刪除節點
    Node* findMin(Node* node);           // 找最小值

public:
    BST() { root = NULL; } // 初始化

    void insert(int key);  // 外部呼叫插入
    int getHeight();       // 取得整棵樹高度
    void remove(int key);  // 刪除節點

    void clear(Node* node); // 釋放記憶體
   // ~BST(); // 解構子
};

// 插入節點（遞迴版本）
BST::Node* BST::insert(Node* node, int key) {
    if (node == NULL)
        return new Node(key); // 找到空位置就建立新節點

    if (key < node->key)
        node->left = insert(node->left, key);   // 往左子樹插入
    else
        node->right = insert(node->right, key); // 往右子樹插入（含重複）

    return node; // 回傳更新後的節點
}

// 對外的 insert
void BST::insert(int key) {
    root = insert(root, key); // 從 root 開始插入
}

// 計算樹高度遞迴
int BST::height(Node* node) {
    if (node == NULL)
        return 0; // 空節點高度為 0

    int leftH = height(node->left);   // 左子樹高度
    int rightH = height(node->right); // 右子樹高度

    return max(leftH, rightH) + 1; // 取較大者 +1
}

// 對外取得高度
int BST::getHeight() {
    return height(root);
}

// 找右子樹最小值
BST::Node* BST::findMin(Node* node) {
    while (node->left != NULL)
        node = node->left; // 一直往左走
    return node;
}

// 刪除節點
BST::Node* BST::deleteNode(Node* node, int key) {
    if (node == NULL) return NULL; // 找不到

    if (key < node->key)
        node->left = deleteNode(node->left, key);   // 往左找
    else if (key > node->key)
        node->right = deleteNode(node->right, key); // 往右找
    else {
        // 找到要刪的節點

        if (node->left == NULL) {
            Node* temp = node->right; // 用右子樹補
            delete node;
            return temp;
        }
        else if (node->right == NULL) {
            Node* temp = node->left; // 用左子樹補
            delete node;
            return temp;
        }

        // 兩個子節點 → 用右子樹最小值替代
        Node* temp = findMin(node->right);
        node->key = temp->key;
        node->right = deleteNode(node->right, temp->key);
    }

    return node;
}

// 清空整棵樹
void BST::clear(Node* node) {
    if (node == NULL) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

// 對外刪除
void BST::remove(int key) {
    root = deleteNode(root, key);
}

int main() {
    srand(time(NULL)); // 設定隨機種子

    int test[] = { 100, 500, 1000, 2000, 3000, 5000, 7000, 10000 };

    // 測試不同 n 的高度
    for (int i = 0; i < 8; i++) {
        int n = test[i];

        BST tree;

        for (int j = 0; j < n; j++) {
            int val = rand(); // 產生隨機數
            tree.insert(val);
        }

        int h = tree.getHeight();       // 實際高度
        double ratio = h / log2(n);     // 與 log2(n) 比較

        cout << n << " " << h << " " << ratio << endl;
    }

    BST tree;

    // 插入 10 個隨機數
    for (int i = 0; i < 10; i++) {
        int val = rand() % 100;
        cout << val << " ";
        tree.insert(val);
    }

    cout << endl;
    cout << tree.getHeight() << endl; // 原高度

    int a;
    cin >> a; // 輸入要刪的值

    tree.remove(a);  // 刪除

    cout << tree.getHeight() << endl; // 刪除後高度
    
    return 0;
}

```

## 效能分析

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
| 測試一   | <img width="426" height="367" alt="image" src="https://github.com/user-attachments/assets/770dc2b2-8184-444d-9576-61b15106def0" /> | 
| 測試二   |  <img width="463" height="370" alt="image" src="https://github.com/user-attachments/assets/6f792351-9893-4ba4-aa39-4c5aaded8666" />| 




### 結果分析  
- h / log₂(n) 約為常數（約 2~3）  
- 表示 BST 在隨機情況下接近平衡  
- 符合理論時間複雜度 O(log n)  

---

## 結論  

1. 本程式成功實作 Binary Search Tree基本操作  
2. 插入、刪除與搜尋效率取決於樹高 h  
3. 在隨機輸入下，BST 表現接近 O(log n)  
4. 在極端情況下可能退化為 O(n)，顯示平衡樹的重要性  

---

## 心得討論  

在這次實作中，困難的是是刪除節點的邏輯處理，在節點同時具有左右子樹時，需要透過右子樹最小值來取代原節點，並遞迴刪除節點。

觀察到 BST 的效率高度依賴輸入資料的分布。透過實驗 h / log₂(n) 的比值，可以清楚看出在隨機情況下樹高接近平衡，但若輸入為有序資料，則會明顯退化。

