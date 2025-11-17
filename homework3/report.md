# 41343109

作業三 - Qt GUI 多項式計算器

## 解題說明

本作業要求完成兩個 Qt 按鈕槽函式（slot functions）：
1. `on_pushButton_clicked()` - 匯入功能
2. `on_pushButton_2_clicked()` - 結束並且匯出功能

這些函式整合了先前作業二的 Polynomial 類別，提供圖形化使用者介面來處理多項式運算。

### 解題策略

1. **MainWindow 類別設計**
   - 繼承 QMainWindow 創建主視窗
   - 包含輸入區域（QTextEdit）和輸出區域（QTextEdit）
   - 兩個按鈕：pushButton（匯入）和 pushButton_2（結束並匯出）
   - 儲存多項式物件 p1, p2 和資料載入狀態

2. **on_pushButton_clicked() 實作策略**
   - 使用 QFileDialog::getOpenFileName() 開啟檔案選擇對話框
   - 使用 QFile 和 QTextStream 讀取檔案內容
   - 使用 std::stringstream 解析多項式資料
   - 呼叫 processPolynomials() 處理多項式運算
   - 使用 QMessageBox 顯示成功或錯誤訊息

3. **on_pushButton_2_clicked() 實作策略**
   - 檢查資料是否已載入，若無則從輸入框讀取
   - 使用 QFileDialog::getSaveFileName() 開啟儲存對話框
   - 使用 QFile 和 QTextStream 寫入結果到檔案
   - 使用 QMessageBox 顯示成功訊息
   - 呼叫 close() 關閉視窗結束程式

4. **processPolynomials() 輔助函式**
   - 計算 p1 + p2（加法）
   - 計算 p1 * p2（乘法）
   - 計算 p1(x)（求值）
   - 格式化輸出結果到輸出區域

## 程式實作

### 主要類別宣告（mainwindow.h）

```cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include "polynomial.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();      // 匯入
    void on_pushButton_2_clicked();    // 結束並且匯出

private:
    QTextEdit *inputTextEdit;
    QTextEdit *outputTextEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    
    Polynomial p1, p2;
    bool dataLoaded;
    
    void processPolynomials();
};

#endif // MAINWINDOW_H
```

### on_pushButton_clicked() 實作

```cpp
void MainWindow::on_pushButton_clicked() {
    // 匯入功能：從檔案讀取多項式資料
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("開啟檔案"), "",
        tr("文字檔 (*.txt);;所有檔案 (*)"));
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "錯誤", "無法開啟檔案！");
        return;
    }
    
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    
    // 顯示載入的內容
    inputTextEdit->setPlainText(content);
    
    // 解析多項式資料
    std::stringstream ss(content.toStdString());
    
    try {
        ss >> p1 >> p2;
        dataLoaded = true;
        
        // 處理多項式並顯示結果
        processPolynomials();
        
        QMessageBox::information(this, "成功", "資料已成功匯入！");
    } catch (...) {
        QMessageBox::warning(this, "錯誤", "資料格式錯誤！");
        dataLoaded = false;
    }
}
```

### on_pushButton_2_clicked() 實作

```cpp
void MainWindow::on_pushButton_2_clicked() {
    // 結束並且匯出功能：將結果儲存到檔案
    if (!dataLoaded) {
        // 如果沒有從檔案載入，嘗試從輸入框讀取
        QString inputText = inputTextEdit->toPlainText();
        if (inputText.isEmpty()) {
            QMessageBox::warning(this, "錯誤", "請先匯入或輸入資料！");
            return;
        }
        
        std::stringstream ss(inputText.toStdString());
        try {
            ss >> p1 >> p2;
            processPolynomials();
            dataLoaded = true;
        } catch (...) {
            QMessageBox::warning(this, "錯誤", "資料格式錯誤！");
            return;
        }
    }
    
    // 選擇儲存位置
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("儲存結果"), "",
        tr("文字檔 (*.txt);;所有檔案 (*)"));
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "錯誤", "無法建立檔案！");
        return;
    }
    
    QTextStream out(&file);
    out << outputTextEdit->toPlainText();
    file.close();
    
    QMessageBox::information(this, "成功", "結果已成功匯出！程式即將結束。");
    
    // 結束程式
    close();
}
```

### processPolynomials() 實作

```cpp
void MainWindow::processPolynomials() {
    // 計算多項式運算結果
    Polynomial sum = p1.Add(p2);
    Polynomial product = p1.mult(p2);
    
    // 取得 x 值（如果有的話，否則使用預設值 1.0）
    float x = 1.0;
    QString inputText = inputTextEdit->toPlainText();
    QStringList lines = inputText.split('\n');
    for (const QString &line : lines) {
        QStringList parts = line.split(' ');
        if (parts.size() == 1 && !parts[0].isEmpty()) {
            bool ok;
            float val = parts[0].toFloat(&ok);
            if (ok) {
                x = val;
                break;
            }
        }
    }
    
    // 格式化輸出
    std::stringstream ss;
    ss << "P1+P2=" << sum << std::endl;
    ss << "P1*P2=" << product << std::endl;
    ss << "P1(x)=" << p1 << std::endl;
    ss << "P1(" << x << ")=" << p1.Eval(x) << std::endl;
    
    outputTextEdit->setPlainText(QString::fromStdString(ss.str()));
}
```

## 效能分析

| 函式 | 時間複雜度 | 空間複雜度 | 說明 |
|:---:|:---:|:---:|:---|
| on_pushButton_clicked() | O(n+m) | O(n+m) | 讀取檔案和解析多項式 |
| on_pushButton_2_clicked() | O(k) | O(k) | k 為輸出字串長度 |
| processPolynomials() | O((n*m)²) | O(n*m) | 包含多項式乘法運算 |

其中 n 和 m 分別為兩個多項式的項數。

## 功能特點

### on_pushButton_clicked() 特點
1. **檔案選擇對話框**：提供友善的 GUI 檔案選擇介面
2. **錯誤處理**：完整的檔案讀取和格式驗證錯誤處理
3. **即時顯示**：載入的資料立即顯示在輸入區域
4. **自動計算**：載入成功後自動執行多項式運算
5. **使用者回饋**：使用訊息框通知使用者操作結果

### on_pushButton_2_clicked() 特點
1. **彈性輸入**：支援從檔案載入或直接輸入的資料
2. **資料驗證**：確保資料已載入才能匯出
3. **檔案儲存對話框**：友善的儲存位置選擇介面
4. **完整流程**：匯出後自動關閉程式
5. **錯誤處理**：完整的檔案寫入錯誤處理

## 測試與驗證

### 測試案例一：標準輸入

**輸入檔案內容**：
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

**預期輸出**：
```
P1+P2=2X^2+8X^1+10
P1*P2=10X^3+27X^2+38X^1+24
P1(x)=2X^2+3X^1+4
P1(2)=18
```

### 測試案例二：空檔案錯誤處理

**操作步驟**：
1. 點擊「匯入」
2. 選擇空檔案

**預期結果**：顯示「資料格式錯誤！」訊息

### 測試案例三：未載入資料匯出

**操作步驟**：
1. 不匯入任何資料
2. 直接點擊「結束並且匯出」

**預期結果**：顯示「請先匯入或輸入資料！」訊息

### 編譯指令

```bash
cd homework3
qmake polynomial_calculator.pro
make
./polynomial_calculator
```

### 測試流程

1. 執行程式
2. 點擊「匯入」按鈕，選擇 test_input.txt
3. 查看輸出區域的計算結果
4. 點擊「結束並且匯出」按鈕
5. 選擇儲存位置（例如 output.txt）
6. 程式自動關閉

## 結論

1. **功能完整**：成功實作了 on_pushButton_clicked() 和 on_pushButton_2_clicked() 兩個函式，提供完整的匯入和匯出功能。

2. **使用者體驗**：使用 Qt 提供的標準對話框（QFileDialog、QMessageBox），提供友善的圖形化介面。

3. **錯誤處理**：完整的錯誤處理機制，包括檔案讀寫錯誤、資料格式錯誤等。

4. **程式架構**：採用物件導向設計，MainWindow 類別清楚分離 UI 和邏輯處理。

5. **整合性**：成功整合作業二的 Polynomial 類別，實現了 GUI 和核心邏輯的分離。

## 心得討論

在實作這兩個按鈕函式時，學習到了以下幾點：

1. **Qt Signal-Slot 機制**：Qt 的信號槽機制讓按鈕點擊事件的處理變得非常直觀，使用 `connect()` 函式就能輕鬆建立按鈕和槽函式的連接。

2. **檔案 I/O 處理**：Qt 提供的 QFile 和 QTextStream 類別讓檔案讀寫變得簡單，而且跨平台相容性好。

3. **對話框使用**：QFileDialog 和 QMessageBox 提供了標準化的使用者介面元件，不需要自己實作就能有專業的外觀和行為。

4. **資料轉換**：在 Qt 的 QString 和標準 C++ 的 string 之間轉換時，需要特別注意使用 `toStdString()` 和 `fromStdString()` 方法。

5. **錯誤處理的重要性**：在 GUI 應用程式中，完善的錯誤處理和使用者回饋機制非常重要，能大幅提升使用者體驗。

6. **程式生命週期管理**：使用 `close()` 方法可以優雅地關閉視窗，Qt 會自動處理資源清理。

整體而言，這次作業讓我深入了解了如何將先前的命令列程式轉換成具有圖形介面的應用程式，並學會了 Qt 框架的基本使用方法。
