#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    input = findChild<QLineEdit*>("lineEdit");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::information(this, "提示", "按钮点击成功！");
}


void MainWindow::on_pushButton_clicked()
{
    QString content = input->text();
    if (content.isEmpty()) {
        QMessageBox::warning(this, "警告", "输入框内容不能为空！");
    } else {
        QMessageBox::information(this, "输入内容", "你输入的是：\n" + content);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::information(this, "成功", "操作成功完成！");
}

