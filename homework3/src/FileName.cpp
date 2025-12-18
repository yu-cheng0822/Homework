#include <iostream>
using namespace std;

struct Term {
    double coef;
    int exp;
    Term(double c = 0, int e = 0) : coef(c), exp(e) {}
};

// Forward declarations
template<class T> class ChainIterator;
template<class T> class AvailableList;


template <class T>
class ChainNode {
    friend class AvailableList<T>;
    friend class ChainIterator<T>;
    template<class U> friend class Chain;
private:
    T element;
    ChainNode<T>* next;
public:
    ChainNode() : next(nullptr) {}
    ChainNode(const T& elem) : element(elem), next(nullptr) {}
    ChainNode(const T& elem, ChainNode<T>* nextNode) : element(elem), next(nextNode) {}
    void setElement(const T& elem) { element = elem; }
    void setNext(ChainNode<T>* nextNode) { next = nextNode; }
};


template<class T>
class Chain {
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    ChainNode<T>* head;
public:
    Chain() : head(nullptr) {}
    ~Chain() {
        ChainNode<T>* current = head;
        while (current != nullptr) {
            ChainNode<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
    }
    ChainNode<T>* release() {
        ChainNode<T>* oldHead = head;
        head = nullptr;
        return oldHead;
    }
    ChainIterator<T> begin() const { return ChainIterator<T>(head); }
    ChainIterator<T> end() const { return ChainIterator<T>(nullptr); }

    void insert(int idx, const T& element) {
        ChainNode<T>* newNode = AvailableList<T>::getNode();
        if (!newNode) newNode = new ChainNode<T>(element);
        else newNode->setElement(element);

        if (idx == 0) {
            newNode->next = head;
            head = newNode;
        } else {
            ChainNode<T>* prev = head;
            for (int i = 0; i < idx - 1 && prev != nullptr; ++i) prev = prev->next;
            if (prev != nullptr) {
                newNode->next = prev->next;
                prev->next = newNode;
            } else {
                AvailableList<T>::getBack(newNode);
            }
        }
    }
};


template<class T>
class ChainIterator {
private:
    ChainNode<T>* currentNode;
public:
    ChainIterator(ChainNode<T>* startNode = nullptr) : currentNode(startNode) {}
    T& operator*() const { return currentNode->element; }
    T* operator->() const { return &currentNode->element; }
    ChainIterator& operator++() { currentNode = currentNode->next; return *this; }
    ChainIterator operator++(int) { ChainIterator old = *this; currentNode = currentNode->next; return old; }
    bool operator!=(const ChainIterator& right) const { return currentNode != right.currentNode; }
    bool operator==(const ChainIterator& right) const { return currentNode == right.currentNode; }

    int operator-(const ChainIterator& right) const {
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


template<class T>
class AvailableList {
private:
    static Chain<T> availableItems;
public:
    static void getBack(ChainNode<T>* firstNode) {
        if (!firstNode) return;
        ChainNode<T>* current = firstNode;
        while (current->next) current = current->next;
        current->next = availableItems.head;
        availableItems.head = firstNode;
    }
    static bool isEmpty() { return availableItems.head == nullptr; }
    static ChainNode<T>* getNode() {
        if (!availableItems.head) return nullptr;
        ChainNode<T>* node = availableItems.head;
        availableItems.head = availableItems.head->next;
        node->next = nullptr;
        return node;
    }
};
template<class T> Chain<T> AvailableList<T>::availableItems;


template<class Term>
class Polynomial {
    template<class U> friend std::ostream& operator<<(std::ostream& os, const Polynomial<U>& poly);
    template<class U> friend std::istream& operator>>(std::istream& is, Polynomial<U>& x);
private:
    Chain<Term> terms;
public:
    Polynomial() {}
    Polynomial(const Polynomial& other) {
        for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it)
            terms.insert(it - other.begin(), *it);
    }
    Polynomial& operator=(const Polynomial& other) {
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

    void newTerm(double coef, int exp) {
        int index = 0;
        for (ChainIterator<Term> it = terms.begin(); it != terms.end(); ++it, ++index) {
            if (it->exp == exp) { it->coef += coef; return; }
            else if (it->exp < exp) break;
        }
        terms.insert(index, Term(coef, exp));
    }

    Polynomial operator+(const Polynomial& other) const {
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

    Polynomial operator-(const Polynomial& other) const {
        Polynomial neg;
        for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it) neg.newTerm(-it->coef, it->exp);
        return (*this) + neg;
    }

    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;
        for (ChainIterator<Term> it1 = this->begin(); it1 != this->end(); ++it1)
            for (ChainIterator<Term> it2 = other.begin(); it2 != other.end(); ++it2)
                result.newTerm(it1->coef * it2->coef, it1->exp + it2->exp);
        return result;
    }
};

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