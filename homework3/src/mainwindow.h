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
