#include "subwindow.h"
#include "ui_subwindow.h"

SubWindow::SubWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubWindow)
{
    ui->setupUi(this);
}

SubWindow::~SubWindow()
{
    delete ui;
}

// 接收主窗口传递的内容
void SubWindow::setContent(const QString &text)
{
    // 确保子窗口输入框对象名是 lineEdit_sub
    ui->lineEdit_sub->setText(text);
}

// 返回主窗口（关闭子窗口）
void SubWindow::on_pushButton_back_clicked()
{
    this->close();
}