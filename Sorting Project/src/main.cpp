#include <iostream>      
#include <vector>        
#include <cstdlib>       
#include <ctime>         
#include <algorithm>     
#include <iomanip>       
#include <string>        

using namespace std;

// (a) Insertion Sort
void insertionSort(vector<int>& a) {          // 插入排序函式
    for (int i = 1; i < a.size(); i++) {      // 從第 2 個元素開始
        int key = a[i], j = i - 1;            // key 為待插入元素，j 指向前一個元素

        while (j >= 0 && a[j] > key) {        // 若前面元素較大
            a[j + 1] = a[j];                  // 元素往右移
            j--;                              // 往前比較
        }

        a[j + 1] = key;                       // 插入正確位置
    }
}

// (b) Quick Sort

int partition(vector<int>& a, int low, int high) {
    // 1. 找出中點
    int mid = low + (high - low) / 2;

    // 2. 將 low, mid, high 三個位置的數排序，讓中位數跑到 mid
    if (a[low] > a[mid]) swap(a[low], a[mid]);
    if (a[low] > a[high]) swap(a[low], a[high]);
    if (a[mid] > a[high]) swap(a[mid], a[high]);

    // 3. 把位於 mid 的中位數（Pivot）換到最後面（high），配合你原本的掃描邏輯
    swap(a[mid], a[high]);

    int pivot = a[high];
    int i = low - 1; // 記錄較小元素區域

    for (int j = low; j < high; j++) {
        if (a[j] <= pivot) {
            swap(a[++i], a[j]); // 放到左側區域
        }
    }

    swap(a[i + 1], a[high]); // Pivot 放回正確位置
    return i + 1;            // 回傳 Pivot 位置
}

void quickSort(vector<int>& a, int low, int high) {

    if (low < high) {                         // 區間長度大於1

        int pi = partition(a, low, high);     // 分割

        quickSort(a, low, pi - 1);            // 排序左半部

        quickSort(a, pi + 1, high);           // 排序右半部
    }
}

// (c) Merge Sort

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
void heapify(vector<int>& a, int i, int n) {
    int largest = i;       // 先假設自己（爸爸）最大
    int left = 2 * i + 1;  // 左小孩子的索引
    int right = 2 * i + 2; // 右小孩子的索引

    // 如果左小孩在範圍內，且值大於目前的 largest
    if (left < n && a[left] > a[largest]) {
        largest = left;
    }

    // 如果右小孩在範圍內，且值大於目前的 largest
    if (right < n && a[right] > a[largest]) {
        largest = right;
    }

    // 如果最大的不是爸爸，代表需要交換，並繼續向下調整
    if (largest != i) {
        swap(a[i], a[largest]);

        // 遞迴調整受到影響的子樹
        heapify(a, largest, n);
    }
}

void heapSort(vector<int>& a) {
    int n = a.size();
    // 從最後一個擁有小孩子的節點 (n/2 - 1) 開始，倒著往上做 heapify
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(a, i, n);
    }

    // 每次把樹根（索引 0，最大值）換到目前堆積的最後面，然後把堆積範圍減 1，重新調整樹根
    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);   // 把最大值丟到後面排好
        heapify(a, 0, i);   // 重新調整樹根，注意這時有效邊界變成了 i
    }
}

// 隨機打亂資料

void permute(vector<int>& a, int n) {

    a.resize(n);                              // 調整大小

    for (int i = 0; i < n; i++)               // 產生 1~n
        a[i] = i + 1;

    for (int i = n - 1; i > 0; i--) {         // Fisher-Yates Shuffle

        swap(a[i], a[rand() % (i + 1)]);      // 隨機交換
    }
}
int main() {

    srand(time(NULL));                         // 使用目前時間作為亂數種子

    vector<int> n_values = { 500, 1000, 2000, 3000, 4000, 5000 };
    // 測試資料筆數

    const int RUNS = 10;                       // 平均情況重複測試次數

    cout << setw(6) << "n"                     // 輸出表頭 n
        << " | " << setw(26) << "      Insertion Sort    "
        << " | " << setw(26) << "        Quick Sort      "
        << " | " << setw(26) << "        Merge Sort      "
        << " | " << setw(26) << "        Heap Sort       " << "\n";

    cout << setw(6) << ""                      // 第二列標題
        << " | " << setw(8) << "Best" << setw(9) << "Avg" << setw(9) << "Worst"
        << " | " << setw(8) << "Best" << setw(9) << "Avg" << setw(9) << "Worst"
        << " | " << setw(8) << "Best" << setw(9) << "Avg" << setw(9) << "Worst"
        << " | " << setw(8) << "Best" << setw(9) << "Avg" << setw(9) << "Worst" << "\n";

    cout << string(125, '-') << "\n";

    for (int n : n_values) {                   // 逐一測試各種資料量
        vector<int> data;                      // 儲存測試資料
        clock_t start;                         // 計時起點
        double ins_b, ins_a = 0, ins_w;        // Insertion 最好 平均 最差
        double q_b, q_a = 0, q_w;             // Quick 最好 平均 最差
        double m_b, m_a = 0, m_w;             // Merge 最好 平均 最差
        double h_b, h_a = 0, h_w;             // Heap 最好 平均 最差

        // 最好情況 Best Case
        vector<int> sorted_data(n);            // 建立已排序資料
        for (int i = 0; i < n; i++)
            sorted_data[i] = i + 1;           // 填入 1~n
        data = sorted_data;                   // 複製資料
        start = clock();                      // 開始計時
        insertionSort(data);                  // 執行 Insertion Sort
        ins_b = double(clock() - start) / CLOCKS_PER_SEC;
        // 計算執行時間
        data = sorted_data;                   // 複製資料
        start = clock();                      // 開始計時
        mergeSort(data);                      // 執行 Merge Sort
        m_b = double(clock() - start) / CLOCKS_PER_SEC;
        // 計算執行時間
        data = sorted_data;                   // 複製資料
        start = clock();                      // 開始計時
        heapSort(data);                       // 執行 Heap Sort
        h_b = double(clock() - start) / CLOCKS_PER_SEC;
        // 計算執行時間
        permute(data, n);                     // 產生隨機資料
        start = clock();                      // 開始計時
        quickSort(data, 0, n - 1);            // 執行 Quick Sort
        q_b = double(clock() - start) / CLOCKS_PER_SEC;
        // 計算執行時間

// 最壞情況 Worst Case
        vector<int> reverse_data(n);          // 建立反向排序資料
        for (int i = 0; i < n; i++)
            reverse_data[i] = n - i;          // 填入 n~1
        data = reverse_data;                  // 複製反向資料
        start = clock();                      // 開始計時
        insertionSort(data);                  // 執行 Insertion Sort
        ins_w = double(clock() - start) / CLOCKS_PER_SEC;
        // 計算執行時間
        data = sorted_data;                   // 已排序資料
        start = clock();                      // 開始計時
        quickSort(data, 0, n - 1);            // 執行 Quick Sort
        q_w = double(clock() - start) / CLOCKS_PER_SEC;
        // 計算執行時間
        data = reverse_data;                  // 使用反向資料
        start = clock();                      // 開始計時
        mergeSort(data);                      // 執行 Merge Sort
        m_w = double(clock() - start) / CLOCKS_PER_SEC;
        // 計算執行時間
        data = reverse_data;                  // 使用反向資料
        start = clock();                      // 開始計時
        heapSort(data);                       // 執行 Heap Sort
        h_w = double(clock() - start) / CLOCKS_PER_SEC;
        // 計算執行時間

// 平均情況 Average Case

        for (int i = 0; i < RUNS; i++) {
            permute(data, n);                 // 產生隨機排列資料
            vector<int> temp;                 // 暫存資料a
            temp = data;                      // 複製資料
            start = clock();                  // 開始計時
            insertionSort(temp);              // 執行 Insertion Sort
            ins_a += double(clock() - start) / CLOCKS_PER_SEC;
            // 累加時間
            temp = data;                      // 複製資料
            start = clock();                  // 開始計時
            quickSort(temp, 0, n - 1);        // 執行 Quick Sort
            q_a += double(clock() - start) / CLOCKS_PER_SEC;
            // 累加時間
            temp = data;                      // 複製資料
            start = clock();                  // 開始計時
            mergeSort(temp);                  // 執行 Merge Sort
            m_a += double(clock() - start) / CLOCKS_PER_SEC;
            // 累加時間
            temp = data;                      // 複製資料
            start = clock();                  // 開始計時
            heapSort(temp);                   // 執行 Heap Sort
            h_a += double(clock() - start) / CLOCKS_PER_SEC;
            // 累加時間
        }

        ins_a /= RUNS;                        // 計算 Insertion 平均時間
        q_a /= RUNS;                          // 計算 Quick 平均時間
        m_a /= RUNS;                          // 計算 Merge 平均時間
        h_a /= RUNS;                          // 計算 Heap 平均時間
        cout << setw(6) << n << " | "         // 輸出

            << fixed << setprecision(4)       // 小數點四位

            << setw(8) << ins_b
            << setw(9) << ins_a
            << setw(9) << ins_w << " | "

            << setw(8) << q_b
            << setw(9) << q_a
            << setw(9) << q_w << " | "

            << setw(8) << m_b
            << setw(9) << m_a
            << setw(9) << m_w << " | "

            << setw(8) << h_b
            << setw(9) << h_a
            << setw(9) << h_w << "\n";       // 輸出各排序時間
    }

    return 0;
}
