# 41343109

作業三

## 解題說明

完成一個多項式的加法乘法。

### 解題策略

1. struct Term  
   用來表示多項式中的單一項，包含  
   double coe（係數）與 int exp（次方）。

2. class ChainNode  
   單向鏈結串列的節點，負責儲存一個資料元素（Term）以及指向下一個節點的指標。

3. class Chain  
   實作單向鏈結串列，用來動態儲存多項式的各項。  
   提供節點插入、串列釋放（release）以及疊代器存取等功能。

4. class ChainIterator  
   為 Chain 所設計的疊代器，支援走訪串列、解參考、遞增與距離計算，  
   使 Polynomial 能以類似 STL 的方式操作串列。

5. class AvailableList  
   管理回收後的 ChainNode 節點，避免重複配置與釋放記憶體，  
   提升記憶體使用效率。

6. Polynomial::Polynomial()  
   建構子：初始化一個空的多項式，其內部以 Chain 儲存所有項。

7. Polynomial::~Polynomial()  
   解構子：將目前多項式的所有節點釋放並交回 AvailableList 進行回收。

8. void Polynomial::newTerm(double coef, int exp)  
   新增一項至多項式中：  
    若已存在相同次方的項，則累加其係數  
    否則依次方大小插入適當位置，使多項式維持依次方遞減排序  

9. Polynomial Polynomial::operator+(const Polynomial& poly)  
   多項式加法：  
   透過同時走訪兩個已排序的串列，  
   相同次方的項則合併係數，回傳新的多項式結果。

10. Polynomial Polynomial::operator-(const Polynomial& poly)  
    多項式減法：  
    先將另一多項式的所有項係數取負，再利用加法運算完成減法。

11. Polynomial Polynomial::operator*(const Polynomial& poly)  
    多項式乘法：  
    對兩多項式中的每一對項進行相乘（係數相乘、次方相加），  
    並利用 newTerm 合併相同次方的結果項。

12. istream& operator>>(istream& in, Polynomial& poly)  
    多項式輸入：  
    先讀入項數 n，再讀入 n 組 (coef, exp)，並逐一呼叫 newTerm 插入。

13. ostream& operator<<(ostream& out, const Polynomial& poly)  
    多項式輸出：  
    依序輸出每一項的係數與次方，格式為 coef x^exp，  
    並以可讀的多項式形式顯示。




## 程式實作

以下為主要程式碼：

```cpp

#include <iostream>
using namespace std;
//struct 為輸入提供模板
struct Term {
    double coef;
    int exp;
    Term(double c = 0, int e = 0) : coef(c), exp(e) {}
};
//Term結構:表示多項式的一個項(係數 * x^次方)


//前項宣告 讓類別之間可以互相成為 friend
template<class T> class ChainIterator;
template<class T> class AvailableList;

//chainNode類別:鏈結串列的節點
template <class T>
class ChainNode {
    friend class AvailableList<T>; // 允許可用節點串列存取
	friend class ChainIterator<T>; // 允許疊代器存取
	template<class U> friend class Chain; // 允許Chain操作節點
private:
    T element;
    ChainNode<T>* next;
public:
    ChainNode() : next(nullptr) {}// 預設建構子
    ChainNode(const T& elem) : element(elem), next(nullptr) {}// 只給資料的建構子
    ChainNode(const T& elem, ChainNode<T>* nextNode) : element(elem), next(nextNode) {}// 給資料與下一個節點的建構子
    void setElement(const T& elem) { element = elem; }//設定資料
    void setNext(ChainNode<T>* nextNode) { next = nextNode; }//設定下一個節點
};


// Chain 類別:單向鏈結串列
template<class T>
class Chain {
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    ChainNode<T>* head;
public:
    Chain() : head(nullptr) {}//建構子，初始化為空
    ~Chain() {
        ChainNode<T>* current = head;
        while (current != nullptr) {
            ChainNode<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
    }// 解構子，釋放所有節點

    ChainNode<T>* release() {
        ChainNode<T>* oldHead = head;
        head = nullptr;
        return oldHead;
    }// 將整條串列交給AvailableList 回收

    ChainIterator<T> begin() const { return ChainIterator<T>(head); }// 回傳起始疊代器
    ChainIterator<T> end() const { return ChainIterator<T>(nullptr); }// 回傳結尾疊代器

    void insert(int idx, const T& element) {// 在指定索引插入元素
        ChainNode<T>* newNode = AvailableList<T>::getNode();// 優先從可用串列取節點
        if (!newNode) newNode = new ChainNode<T>(element);
        else newNode->setElement(element);

        if (idx == 0) {// 插入在最前面
            newNode->next = head;
            head = newNode;
        } else {// 找到前一個節點
            ChainNode<T>* prev = head;
            for (int i = 0; i < idx - 1 && prev != nullptr; ++i) prev = prev->next;
            if (prev != nullptr) {
                newNode->next = prev->next;
                prev->next = newNode;
            } else {// 索引錯誤，回收節點
                AvailableList<T>::getBack(newNode);
            }
        }
    }
};


//ChainIterator 類別:鏈結串列疊代器
template<class T>
class ChainIterator {
private:
    ChainNode<T>* currentNode;
public:
    ChainIterator(ChainNode<T>* startNode = nullptr) : currentNode(startNode) {}
    T& operator*() const { return currentNode->element; }
    T* operator->() const { return &currentNode->element; }// 成員存取運算子
    ChainIterator& operator++() { currentNode = currentNode->next; return *this; }// 前置 ++
    ChainIterator operator++(int) { ChainIterator old = *this; currentNode = currentNode->next; return old; }// 後置 ++
    bool operator!=(const ChainIterator& right) const { return currentNode != right.currentNode; }// 不等於比較
    bool operator==(const ChainIterator& right) const { return currentNode == right.currentNode; }// 等於比較

    int operator-(const ChainIterator& right) const {//計算兩個疊代器距離用於索引
        int distance = 0;
        ChainNode<T>* temp = right.currentNode;
        while (temp != currentNode) {
            if (!temp) throw std::out_of_range("Iterators not in same chain");
            temp = temp->next;
            ++distance;
        }
        return distance;
    }
};

//                   AvailableList類別:節點回收再利用
template<class T>
class AvailableList {
private:
    static Chain<T> availableItems;
public:
    static void getBack(ChainNode<T>* firstNode) {// 將節點串列放回可用清單
        if (!firstNode) return;
        ChainNode<T>* current = firstNode;
        while (current->next) current = current->next;
        current->next = availableItems.head;
        availableItems.head = firstNode;
    }
    static bool isEmpty() { return availableItems.head == nullptr; }// 判斷是否為空
    static ChainNode<T>* getNode() {// 取得一個可用節點
        if (!availableItems.head) return nullptr;
        ChainNode<T>* node = availableItems.head;
        availableItems.head = availableItems.head->next;
        node->next = nullptr;
        return node;
    }
};
template<class T> Chain<T> AvailableList<T>::availableItems;

//						Polynomial類別:多項式
template<class Term>
class Polynomial {
    template<class U> friend std::ostream& operator<<(std::ostream& os, const Polynomial<U>& poly);
    template<class U> friend std::istream& operator>>(std::istream& is, Polynomial<U>& x);
private:
    Chain<Term> terms;
public:
    Polynomial() {}
    Polynomial(const Polynomial& other) {// 複製建構子
        for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it)
            terms.insert(it - other.begin(), *it);
    }
    Polynomial& operator=(const Polynomial& other) {// 指派運算子
        if (this != &other) {
            AvailableList<Term>::getBack(terms.release());
            for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it)
                terms.insert(it - other.begin(), *it);
        }
        return *this;
    }
    ~Polynomial() { AvailableList<Term>::getBack(terms.release()); }

    ChainIterator<Term> begin() const { return terms.begin(); }
    ChainIterator<Term> end() const { return terms.end(); }

    void newTerm(double coef, int exp) {// 新增一項（依次方排序，合併同次方)
        int index = 0;
        for (ChainIterator<Term> it = terms.begin(); it != terms.end(); ++it, ++index) {
            if (it->exp == exp) { it->coef += coef; return; }
            else if (it->exp < exp) break;
        }
        terms.insert(index, Term(coef, exp));
    }

    Polynomial operator+(const Polynomial& other) const {// 多項式加法
        Polynomial result;
        ChainIterator<Term> it1 = this->begin();
        ChainIterator<Term> it2 = other.begin();
        while (it1 != this->end() && it2 != other.end()) {
            if (it1->exp > it2->exp) { 
                result.newTerm(it1->coef, it1->exp); 
                ++it1; 
            }
            else if (it1->exp < it2->exp) {
                result.newTerm(it2->coef, it2->exp);
                ++it2; 
            }
            else {
                double c = it1->coef + it2->coef;
                if (c != 0) result.newTerm(c, it1->exp); 
                ++it1; 
                ++it2;
            }
        }
        while (it1 != this->end()) { 
            result.newTerm(it1->coef, it1->exp); ++it1;
        }
        while (it2 != other.end()) {
            result.newTerm(it2->coef, it2->exp); ++it2; 
        }
        return result;
    }

    Polynomial operator-(const Polynomial& other) const {// 多項式減法
        Polynomial neg;
        for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it) neg.newTerm(-it->coef, it->exp);
        return (*this) + neg;
    }

    Polynomial operator*(const Polynomial& other) const {// 多項式乘法
        Polynomial result;
        for (ChainIterator<Term> it1 = this->begin(); it1 != this->end(); ++it1)
            for (ChainIterator<Term> it2 = other.begin(); it2 != other.end(); ++it2)
                result.newTerm(it1->coef * it2->coef, it1->exp + it2->exp);
        return result;
    }
};
//						輸入
template<class T>
istream& operator>>(istream& is, Polynomial<T>& x) {
    int n, e;
    double c;
    is >> n;
    for (int i = 0; i < n; ++i) {
        is >> c  >> e;
        x.newTerm(c, e);
    }
    return is;
}
//						輸出
template<class T>
ostream& operator<<(ostream& os, const Polynomial<T>& poly) {
    bool first = true;
    for (ChainIterator<Term> it = poly.begin(); it != poly.end(); ++it) {
        if (!first) os << " + ";
        os << it->coef << "x^" << it->exp;
        first = false;
    }
    return os;
}
//							主函式
int main() {
    Polynomial<Term> p1, p2;
    cout << "p1: ";
    cin >> p1;
    cout << "p2:  ";
    cin >> p2;
    cout << "p1 + p2 = " << p1 + p2 << endl;
    cout << "p1 - p2 = " << p1 - p2 << endl;
    cout << "p1 * p2 = " << p1 * p2 << endl;
}

```

## 效能分析
### ChainNode

| 函式 | 時間複雜度 | 說明 |
|----|----|----|
| ChainNode() | O(1) | 初始化指標 |
| ChainNode(const T&) | O(1) | 指派資料 |
| ChainNode(const T&, ChainNode*) | O(1) | 建構節點 |
| setElement | O(1) | 設定資料 |
| setNext | O(1) | 設定指標 |

---

### Chain

| 函式 | 時間複雜度 | 說明 |
|----|----|----|
| Chain() | O(1) | 初始化空串列 |
| ~Chain() | O(n) | 走訪並釋放節點 |
| release() | O(1) | 回傳 head |
| begin() / end() | O(1) | 建立 iterator |
| insert(idx, elem) | O(n) | 走訪到指定位置 |

---

### ChainIterator

| 函式 | 時間複雜度 | 說明 |
|----|----|----|
| operator* | O(1) | 取值 |
| operator-> | O(1) | 成員存取 |
| operator++ / operator++(int) | O(1) | 前進 |
| operator== / operator!= | O(1) | 比較 |
| operator- | O(n) | 計算距離 |

---

### AvailableList

| 函式 | 時間複雜度 | 說明 |
|----|----|----|
| getBack | O(k) | 回收 k 個節點 |
| isEmpty | O(1) | 判斷是否為空 |
| getNode | O(1) | 取一個節點 |

---

### Polynomial

| 函式 | 時間複雜度 | 說明 |
|----|----|----|
| Polynomial() | O(1) | 建構空多項式 |
| ~Polynomial() | O(n) | 回收節點 |
| Copy constructor | O(n²) | 逐項插入 |
| operator= | O(n²) | 釋放後重建 |
| begin() / end() | O(1) | 取得 iterator |
| newTerm | O(n) | 掃描並插入 |
| operator+ | O(n + m) | 合併兩串列 |
| operator- | O(n + m) | 加法 + 取負 |
| operator* | O(n² m²) | 傳統乘法 |

---

### 輸入輸出

| 函式 | 時間複雜度 | 說明 |
|----|----|----|
| operator>> | O(n²) | n 次 newTerm |
| operator<< | O(n) | 輸出所有項 |


## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數  |
|----------|--------------|
| 測試一   | <img width="515" height="287" alt="image" src="https://github.com/user-attachments/assets/68da1ab6-8f5b-4dab-9e21-68683ea9ce9e" />   | 
| 測試二   |   <img width="997" height="373" alt="image" src="https://github.com/user-attachments/assets/fee4f021-1876-42c1-b876-2e1fbad45f80" />
 | 




## 申論及開發報告
本題實作一個多項式（Polynomial）類別，並支援多項式的加法、減法與乘法運算。
為了能夠有效管理不定數量的多項式項目，程式並未採用固定大小的陣列，
而是以單向鏈結串列作為底層資料結構，
使多項式能依實際需求動態增減項目數量。

每一個多項式項目以 Term 結構表示，包含係數（coef）與次方（exp）。
多項式內部透過 Chain 類別儲存多個 Term 節點，
並維持項目依照次方由大到小排序，
以利後續多項式運算的進行。

在多項式加法與減法中，
利用兩個已排序串列同步走訪的方式，
有效避免不必要的重複掃描。
而在多項式乘法中，
則採用最直觀的雙層迴圈方式，
對每一對項進行係數相乘與次方相加，
並透過 newTerm 函式合併相同次方的結果項。

此外，程式額外設計 AvailableList 類別來管理回收後的節點，
避免頻繁配置與釋放記憶體，
在維持程式結構清楚的同時，也提升了記憶體使用效率。
1. **Term**  
   表示多項式中的單一項，包含係數與次方。
2. **ChainNode / Chain**  
   實作單向鏈結串列，用來動態儲存多項式的各個項目。
3. **ChainIterator**  
   提供串列走訪介面。
4. **AvailableList**  
   負責回收與再利用已釋放的節點，減少記憶體配置成本。
5. **Polynomial**  
   多項式的所有操作，包括項目新增、加法、減法與乘法。

## 心得討論
透過本次多項式系統的實作，對資料結構與物件導向設計有了更深入的理解。
一開始看似只是進行多項式的加減乘運算，
實際實作後才體會到底層資料結構的選擇，
會直接影響程式的設計方式與整體效能。





