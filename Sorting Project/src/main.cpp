#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

// (a) Insertion
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

// (b) Quick
int partition(vector<int>& a, int low, int high) {
    int randomIndex = low + rand() % (high - low + 1);
    swap(a[randomIndex], a[high]);
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

// (c) Merge
void mergeSort(vector<int>& a) {

    int n = a.size();

    // w 代表目前要合併的子陣列長度 每次乘以 2 (1, 2, 4, 8...)
    for (int w = 1; w < n; w *= 2) {

        // 兩兩合併區間[i, i + w - 1] 和 [i + w, i + 2*w - 1]
        for (int i = 0; i < n; i += 2 * w) {
            int low = i;
            int mid = min(i + w, n);
            int high = min(i + 2 * w, n);

            // inplace_merge 會自動將兩個相鄰的已排序區間 (low, mid) 與 (mid, high) 合併
            if (mid < high) {
                inplace_merge(a.begin() + low, a.begin() + mid, a.begin() + high);
            }
        }
    }
}


// (d) Heap Sort
void heapSort(vector<int>& a) {
    make_heap(a.begin(), a.end());
    sort_heap(a.begin(), a.end());
}

// 隨機打亂
void permute(vector<int>& a, int n) {
    a.resize(n);
    for (int i = 0; i < n; i++) a[i] = i + 1;
    for (int i = n - 1; i > 0; i--) {
        swap(a[i], a[rand() % (i + 1)]);
    }
}

int main() {
    srand(time(NULL));

    vector<int> n_values = { 500, 1000, 2000, 3000, 4000, 5000 };
    const int RUNS = 10;

    cout << setw(6) << "n"
        << " | " << setw(26) << "      Insertion Sort    "
        << " | " << setw(26) << "        Quick Sort      "
        << " | " << setw(26) << "        Merge Sort      "
        << " | " << setw(26) << "        Heap Sort       " << "\n";
    cout << setw(6) << ""
        << " | " << setw(8) << "Best" << setw(9) << "Avg" << setw(9) << "Worst"
        << " | " << setw(8) << "Best" << setw(9) << "Avg" << setw(9) << "Worst"
        << " | " << setw(8) << "Best" << setw(9) << "Avg" << setw(9) << "Worst"
        << " | " << setw(8) << "Best" << setw(9) << "Avg" << setw(9) << "Worst" << "\n";
    cout << string(125, '-') << "\n";

    for (int n : n_values) {
        vector<int> data;
        clock_t start;

        double ins_b, ins_a = 0, ins_w;
        double q_b, q_a = 0, q_w;
        double m_b, m_a = 0, m_w;
        double h_b, h_a = 0, h_w;

        // 最好情況

        vector<int> sorted_data(n);
        for (int i = 0; i < n; i++) sorted_data[i] = i + 1;

        data = sorted_data; 
        start = clock(); 
        insertionSort(data); 
        ins_b = double(clock() - start) / CLOCKS_PER_SEC;

        data = sorted_data; 
        start = clock(); 
        mergeSort(data); 
        m_b = double(clock() - start) / CLOCKS_PER_SEC;

        data = sorted_data; 
        start = clock(); 
        heapSort(data); 
        h_b = double(clock() - start) / CLOCKS_PER_SEC;

        permute(data, n); 
        start = clock(); 
        quickSort(data, 0, n - 1); 
        q_b = double(clock() - start) / CLOCKS_PER_SEC;

        // 最壞情況

        vector<int> reverse_data(n);
        for (int i = 0; i < n; i++) reverse_data[i] = n - i;
        
        data = reverse_data; 
        start = clock(); 
        insertionSort(data); 
        ins_w = double(clock() - start) / CLOCKS_PER_SEC;

        data = sorted_data; 
        start = clock(); 
        quickSort(data, 0, n - 1); 
        q_w = double(clock() - start) / CLOCKS_PER_SEC;

        data = reverse_data; 
        start = clock(); 
        mergeSort(data); 
        m_w = double(clock() - start) / CLOCKS_PER_SEC;

        data = reverse_data; 
        start = clock(); 
        heapSort(data); 
        h_w = double(clock() - start) / CLOCKS_PER_SEC;

        // 平均情況

        for (int i = 0; i < RUNS; i++) {
            permute(data, n);
            vector<int> temp;

            temp = data; start = clock(); 
            insertionSort(temp); 
            ins_a += double(clock() - start) / CLOCKS_PER_SEC;

            temp = data; start = clock(); 
            quickSort(temp, 0, n - 1);
            q_a += double(clock() - start) / CLOCKS_PER_SEC;

            temp = data; start = clock();
            mergeSort(temp); 
            m_a += double(clock() - start) / CLOCKS_PER_SEC;

            temp = data; 
            start = clock(); 
            heapSort(temp); 
            h_a += double(clock() - start) / CLOCKS_PER_SEC;
        }

        ins_a /= RUNS; 
        q_a /= RUNS; 
        m_a /= RUNS;
        h_a /= RUNS;

        cout << setw(6) << n << " | "
            << fixed << setprecision(4)
            << setw(8) << ins_b << setw(9) << ins_a << setw(9) << ins_w << " | "
            << setw(8) << q_b << setw(9) << q_a << setw(9) << q_w << " | "
            << setw(8) << m_b << setw(9) << m_a << setw(9) << m_w << " | "
            << setw(8) << h_b << setw(9) << h_a << setw(9) << h_w << "\n";
    }
    return 0;
}
