# 41343107 41343109

# Sorting Project

## 程式所需清單

|項目   |名稱       |
|------|-----------|
|1   |Online C++   |

## 計時精度與實驗控制

1. 系統時鐘的精確度（例如8毫秒）。
2. 若執行時間小於時鐘精度，需重複執行多次並取平均值。
3. 實驗數據目標誤差應控制在 1% 以內
   
### 數據量 (n) 設定
    
    初步測試點：
        n = 500, 1000, 2000, 3000, 4000, 5000
        若 n 較小時時間顯示為 0，代表需增加執行次數。

## 最壞情況數據生成

|演算法	 |最壞情況數據生成策略                |
|--------|----------------------------------|
|插入排序 |使用遞減序列 (n, n-1, n-2, ..., 1)。 |
|合併排序 |透過反向合併來建構導致最多比較次數的序列。|
|堆積排序 |使用隨機排列生成器進行多次採樣，取其中的最大運行時間。|
|快速排序 |對隨機排列進行測試，尋找導致最差效能的分布。|

## 程式實作

以下為主程式

```cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>

using namespace std;

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

    merge(left.begin(), left.end(), right.begin(), right.end(), a.begin());
}

// (d) 堆積排序
void heapSort(vector<int>& a) {
    make_heap(a.begin(), a.end());
    sort_heap(a.begin(), a.end());
}

// 隨機排列生成
void permute(vector<int>& a, int n) {
    a.resize(n);
    for (int i = 0; i < n; i++) a[i] = i + 1;
    for (int i = n - 1; i > 0; i--) {
        swap(a[i], a[rand() % (i + 1)]);
    }
}

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
```

## 測試與驗證

|n     |Insert(W)  |Quick(A)  |Merge(A)  |Heap(A)   |
|------|-----------|----------|----------|----------|
|500   |0.00087    |0.00007   |0.00032   |0.00017   |
|1000  |0.00304    |0.00015   |0.00066   |0.00037   |
|2000  |0.01212    |0.00034   |0.00138   | 0.00081  |
|3000  |0.02802    |0.00054   |0.00214   | 0.00126  |
|4000  |0.04825    |0.00074   |0.00291   | 0.00174  |
|5000  |0.07535    |0.00096   |0.00368   | 0.00222  |

## 效能分析

O(n2)與O(nlogn)

