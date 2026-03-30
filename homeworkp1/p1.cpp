#include <iostream>
using namespace std;
template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    // virtual destructor
    virtual bool IsEmpty() const = 0;
    // return true iff the priority queue is empty
    virtual const T& Top() const = 0;
    // return reference to max element
    virtual void Push(const T&) = 0;
    // add an element to the priority queue
    virtual void Pop() = 0;
    // delete element with max priority
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
        if(size+1==capacity)
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

    // 加入元素
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