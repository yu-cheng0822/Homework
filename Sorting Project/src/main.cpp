#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>

using namespace std;

// ==========================================
// 1. 演算法實作（精簡版）
// ==========================================

// (a) 插入排序
void insertionSort(vector<int>& a) {
    for (int i = 1; i < a.size(); i++) {
        int key = a[i], j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

// (b) 快速排序
int partition(vector<int>& a, int low, int high) {
    int pivot = a[high], i = low - 1;
    for (int j = low; j < high; j++) {
        if (a[j] <= pivot) swap(a[++i], a[j]);
    }
    swap(a[i + 1], a[high]);
    return i + 1;
}
void quickSort(vector<int>& a, int low, int high) {
    if (low < high) {
        int pi = partition(a, low, high);
        quickSort(a, low, pi - 1);
        quickSort(a, pi + 1, high);
    }
}

// (c) 合併排序
void mergeSort(vector<int>& a) {
    if (a.size() <= 1) return;
    int mid = a.size() / 2;
    vector<int> left(a.begin(), a.begin() + mid);
    vector<int> right(a.begin() + mid, a.end());
    
    mergeSort(left);
    mergeSort(right);
    
    // 利用 C++ 內建的 merge 函式，一行搞定合併
    merge(left.begin(), left.end(), right.begin(), right.end(), a.begin());
}

// (d) 堆積排序
void heapSort(vector<int>& a) {
    // 利用 C++ 內建的 Heap 函式，直接把 vector 變成 Max Heap 並排序
    make_heap(a.begin(), a.end());
    sort_heap(a.begin(), a.end());
}

// ==========================================
// 2. 隨機排列生成器（作業要求：不重複使用同組數據）
// ==========================================
void permute(vector<int>& a, int n) {
    a.resize(n);
    for (int i = 0; i < n; i++) a[i] = i + 1;
    for (int i = n - 1; i > 0; i--) {
        swap(a[i], a[rand() % (i + 1)]);
    }
}

// ==========================================
// 3. 測試與計時主程式
// ==========================================
int main() {
    srand(time(NULL));
    vector<int> n_values = {500, 1000, 2000, 3000, 4000, 5000};
    const int RUNS = 100; // 聚合計時跑 100 次不同排列

    cout << setw(6) << "n" << setw(12) << "Insert(W)" << setw(12) << "Quick(A)" 
         << setw(12) << "Merge(A)" << setw(12) << "Heap(A)" << "\n";
    cout << "--------------------------------------------------------\n";

    for (int n : n_values) {
        vector<int> data;
        clock_t start, end;

        // 1. 插入排序 最壞情況 (遞減序列)
        data.resize(n);
        for (int i = 0; i < n; i++) data[i] = n - i;
        start = clock();
        insertionSort(data);
        double t_insert = double(clock() - start) / CLOCKS_PER_SEC;

        // 2. 快速排序 平均情況
        double t_quick = 0;
        for (int i = 0; i < RUNS; i++) {
            permute(data, n);
            start = clock();
            quickSort(data, 0, n - 1);
            t_quick += double(clock() - start) / CLOCKS_PER_SEC;
        }

        // 3. 合併排序 平均情況
        double t_merge = 0;
        for (int i = 0; i < RUNS; i++) {
            permute(data, n);
            start = clock();
            mergeSort(data);
            t_merge += double(clock() - start) / CLOCKS_PER_SEC;
        }

        // 4. 堆積排序 平均情況
        double t_heap = 0;
        for (int i = 0; i < RUNS; i++) {
            permute(data, n);
            start = clock();
            heapSort(data);
            t_heap += double(clock() - start) / CLOCKS_PER_SEC;
        }

        // 輸出結果 (將總時間除以 RUNS 得到單次平均)
        cout << setw(6) << n 
             << setw(12) << fixed << setprecision(5) << t_insert
             << setw(12) << fixed << setprecision(5) << (t_quick / RUNS)
             << setw(12) << fixed << setprecision(5) << (t_merge / RUNS)
             << setw(12) << fixed << setprecision(5) << (t_heap / RUNS) << "\n";
    }
    return 0;
}
