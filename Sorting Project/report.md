# 41343107 41343109

# 作業三 Sorting Project

# 解題說明

## 解題策略

利用控制變因法，透過建構特定的輸入資料分布，來實測與驗證四種經典排序演算法在不同資料量 (n) 下的漸進時間複雜度 (O)。

針對每種演算法皆設計了「最好（Best）」、「平均（Average）」與「最壞（Worst）」三種測試情境：

### 基準與對照組設計：

1. 最好情況：利用完全遞增的數列（正序）測試，觀察演算法在幾乎不需更動資料時的理論極限。
2. 最壞情況：利用遞減數列（反序）或特定觸發條件，迫使演算法執行最大運算次數（如插入排序的頻繁位移）。
3. 平均情況：使用 Fisher-Yates Shuffle 洗牌演算法生成隨機排列，並重複執行 10 次（RUNS = 10）取平均值，以平滑掉單次隨機資料的極端誤差。

### 效能觀測與理論對照：

隨著 n 從 500 成長至 5000，預期將觀察到插入排序（Insertion Sort）的 Avg 與 Worst 數據呈二次曲線（平方級）暴增。相反地，快速、合併與堆積排序則會展現出穩定的線性對數級 (O(n log n)) 成長趨勢，藉此驗證理論與實務的一致性。

### 所用清單：

|項目   |名稱       |
|------|-----------|
|1     |Online C++ |
|2     |Visual Studio 2019 |

### 計時精度與控制

1. 系統時鐘的精確度（例如8毫秒）。
2. 若執行時間小於時鐘精度，需重複執行多次並取平均值。
3. 實驗數據目標誤差應控制在 1% 以內
   
### 數據量 (n) 設定
    
    初步測試點：
        n = 500, 1000, 2000, 3000, 4000, 5000
        若 n 較小時時間顯示為 0，代表需增加執行次數。

## 程式實作

以下為主程式

```cpp
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
    if (a.size() <= 1) return;
    int mid = a.size() / 2;
    vector<int> left(a.begin(), a.begin() + mid);
    vector<int> right(a.begin() + mid, a.end());

    mergeSort(left);
    mergeSort(right);

    merge(left.begin(), left.end(), right.begin(), right.end(), a.begin());
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

```

## 測試與驗證

| 測試案例 |項目  |輸入資料量  | 輸出  |
|---------|-----|--------|----------|
| 測試 1  |Visual Studio 2019 |數據量 (n) = 500, 1000, 2000, 3000, 4000, 5000 | <img width="1641" height="411" alt="image" src="https://github.com/user-attachments/assets/5816b14a-9a56-47de-a255-1b9eb1fb0f0e" /> |
| 測試 2  |Online C++ |數據量 (n) = 500, 1000, 2000, 3000, 4000, 5000 | <img width="1411" height="357" alt="image" src="https://github.com/user-attachments/assets/545cf42a-372c-49bd-b83e-4c53808e0dcd" /> |

## 效能分析

綜合 Visual Studio 2019 與 Online C++ 平台的測試數據，分析結果如下：

### 1. 插入排序 (Insertion Sort) — O(n) 與 O(n^2) 的極端對比

- 最好情況 (Best)：當輸入為完全正序時，執行時間幾乎為 0.0000 秒。演算法只需由左至右掃描一次陣列，僅進行 n-1 次比較而完全不需移動資料，符合理論的線性複雜度 O(n)。

- 平均與最壞情況 (Avg / Worst)：當資料量 n 從 500 增長至 5000 時，Avg 與 Worst 的耗時呈現接近 100 倍的平方級暴增。這驗證了插入排序在隨機或反序輸入下，必須執行大量元素位移，完全符合 O(n^2) 的理論特徵。

### 2. 快速排序 (Quick Sort) — 隨機樞紐 (Randomized Pivot)

- 最壞情況的改善：在快速排序中，若固定選擇最右側元素為樞紐，處理完全正序或完全反序的資料時，切分會極度不平衡，使遞迴樹退化成線性結構。這不僅會導致時間退化至 O(n^2)，在 n=5000 時更會耗盡呼叫記憶體進而引發 Stack overflow（堆疊溢位）崩潰。

- 效能表現：本程式實作加入了 rand() 隨機化樞紐策略。從測試結果可以看出，不論在 Best、Avg 還是 Worst 情境下，快速排序的執行時間都非常接近且極低，成功將最壞情況的預期時間複雜度控制在 O(n log n)，破除了結構性退化的風險。

### 3. 合併排序 (Merge Sort) — 穩定但具備空間開銷

- 高度穩定性：合併排序在正序、隨機與反序三種分布下的耗時幾乎完全一致。這是因為其不論資料初始狀態為何，都必須嚴格執行二分切分與合併的遞迴樹結構，符合 O(n log n) 的表現。

- 實作常數瓶頸：合併排序的整體執行速度略慢於快速排序與堆積排序。原因在於本實作在每次遞迴內部皆使用了 vector<int> left(...) 宣告，頻繁向作業系統申請與釋放動態記憶體（Memory Allocation），這些額外的常數開銷拉低了純計算效能。

### 4. 堆積排序 (Heap Sort) — 原地執行的效率

- 常數小且高度穩定：堆積排序在 Best、Avg、Worst 狀況下均穩定展現出 O(n \log n)$的高效率。
  
- 底層優化優勢：由於直接呼叫 C++ 標準函式庫（STL）經過高度優化的 make_heap 與 sort_heap。該標準庫在編譯底層進行了迴圈展開，且排序過程完全在原陣列空間內（In-place）進行，不需額外配置記憶體，因此在漸進複雜度相同的三大演算法中，堆積排序獲得了最小的常數開銷與最快的執行速度。

## 結論

### 理論與實務的量級驗證：

證明了漸進時間複雜度（Asymptotic Complexity）在軟體開發中的關鍵地位。當資料量 n 放大至 5000 時，O(n log n) 級別的演算法（快速、合併、堆積）與 O(n^2) 的插入排序在非最好情況下，執行效率產生了數百倍至數千倍的巨大差距。在大數據場景下，平方級演算法具有明顯的效能瓶頸。

### 隨機化與底層常數的取捨：

- 演算法魯棒性：透過實作隨機樞紐，解決了快速排序面臨特定資料分布時會引發系統崩潰（Stack overflow）的致命缺陷，證明了「演算法隨機化」在實務安全性上的重大價值。

- 硬體與記憶體友善：合併排序雖然具備穩定性（Stable Sort），但頻繁的動態記憶體配置會成為系統的隱形負擔；相比之下，原地的堆積排序（Heap Sort）因其不佔額外空間且常數極小，在追求純粹計算速度與記憶體效益的場景下，展現出了更為優異的實務效能。
