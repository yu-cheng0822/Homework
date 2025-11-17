# Polynomial Calculator - Qt GUI Application

這是一個基於 Qt 框架的多項式計算器 GUI 應用程式，實作了兩個主要的按鈕功能。

## 功能說明

### 1. on_pushButton_clicked() - 匯入功能
- 點擊「匯入」按鈕可以從檔案讀取多項式資料
- 支援標準的多項式輸入格式
- 自動解析並計算多項式的加法、乘法和求值
- 在輸出區域顯示計算結果

### 2. on_pushButton_2_clicked() - 結束並且匯出功能
- 點擊「結束並且匯出」按鈕可以將計算結果儲存到檔案
- 如果尚未匯入資料，會先嘗試從輸入框讀取
- 將所有計算結果儲存到使用者指定的檔案
- 儲存完成後自動關閉程式

## 檔案結構

```
homework3/
├── src/
│   ├── main.cpp           # 主程式入口
│   ├── mainwindow.h       # MainWindow 類別宣告
│   ├── mainwindow.cpp     # MainWindow 實作（包含兩個按鈕函式）
│   └── polynomial.h       # Polynomial 類別定義
├── polynomial_calculator.pro  # Qt 專案檔
├── test_input.txt         # 測試輸入檔案
└── README.md              # 本說明文件
```

## 編譯與執行

### 前置需求
- Qt 5.x 或更高版本
- C++11 或更高版本的編譯器
- qmake

### 編譯指令
```bash
cd homework3
qmake polynomial_calculator.pro
make
```

### 執行
```bash
./polynomial_calculator
```

## 使用方式

1. **啟動程式**：執行編譯後的可執行檔
2. **匯入資料**：
   - 方法一：點擊「匯入」按鈕，選擇包含多項式資料的文字檔
   - 方法二：直接在輸入區域手動輸入多項式資料
3. **查看結果**：輸出區域會自動顯示多項式的加法、乘法和求值結果
4. **匯出結果**：點擊「結束並且匯出」按鈕，選擇儲存位置，程式會將結果儲存並關閉

## 輸入格式

多項式資料格式如下：
```
n1
coef1_1 exp1_1
coef1_2 exp1_2
...
n2
coef2_1 exp2_1
coef2_2 exp2_2
...
x_value
```

其中：
- `n1`, `n2`：多項式的項數
- `coef`：係數
- `exp`：指數
- `x_value`：求值時使用的 x 值

## 範例

使用提供的 `test_input.txt`：
```
3
2 2
3 1
4 0
2
5 1
6 0
2
```

表示：
- P1 = 2X^2 + 3X^1 + 4
- P2 = 5X^1 + 6
- x = 2

計算結果會顯示：
- P1+P2（多項式加法）
- P1*P2（多項式乘法）
- P1(x)（多項式表示）
- P1(2)（多項式求值）

## 實作細節

### on_pushButton_clicked() 實作
1. 使用 QFileDialog 開啟檔案選擇對話框
2. 讀取檔案內容並顯示在輸入區域
3. 使用 stringstream 解析多項式資料
4. 呼叫 processPolynomials() 計算並顯示結果
5. 顯示成功訊息

### on_pushButton_2_clicked() 實作
1. 檢查是否已載入資料，若無則嘗試從輸入框讀取
2. 使用 QFileDialog 開啟儲存對話框
3. 將輸出區域的結果寫入指定檔案
4. 顯示成功訊息並關閉程式

## 注意事項

- 確保輸入檔案格式正確，否則會顯示錯誤訊息
- 程式會在成功匯出後自動關閉
- 支援多次匯入不同的資料檔案
