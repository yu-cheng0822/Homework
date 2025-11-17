#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), dataLoaded(false) {
    
    // 設置視窗標題
    setWindowTitle("Polynomial Calculator");
    
    // 創建中央部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 創建佈局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // 輸入區域
    QLabel *inputLabel = new QLabel("輸入區域 (格式: n coef1 exp1 coef2 exp2 ...):", this);
    mainLayout->addWidget(inputLabel);
    
    inputTextEdit = new QTextEdit(this);
    inputTextEdit->setPlaceholderText("請輸入多項式資料或點擊「匯入」按鈕從檔案載入");
    mainLayout->addWidget(inputTextEdit);
    
    // 按鈕區域
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    pushButton = new QPushButton("匯入", this);
    connect(pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    buttonLayout->addWidget(pushButton);
    
    pushButton_2 = new QPushButton("結束並且匯出", this);
    connect(pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_clicked);
    buttonLayout->addWidget(pushButton_2);
    
    mainLayout->addLayout(buttonLayout);
    
    // 輸出區域
    QLabel *outputLabel = new QLabel("輸出結果:", this);
    mainLayout->addWidget(outputLabel);
    
    outputTextEdit = new QTextEdit(this);
    outputTextEdit->setReadOnly(true);
    mainLayout->addWidget(outputTextEdit);
    
    // 設置視窗大小
    resize(600, 500);
}

MainWindow::~MainWindow() {
}

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
