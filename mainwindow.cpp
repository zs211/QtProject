#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 找到输入框（确保ui里输入框的对象名是 lineEdit）
    input = ui->lineEdit; // 替换原来的 findChild，更稳定
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 确认按钮逻辑
void MainWindow::on_pushButton_clicked()
{
    QString content = input->text();
    if (content.isEmpty()) {
        QMessageBox::warning(this, "警告", "输入框内容不能为空！");
    } else {
        QMessageBox::information(this, "输入内容", "你输入的是：\n" + content);
    }
}

// 成功按钮逻辑
void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::information(this, "成功", "操作成功完成！");
}

// 提示按钮逻辑
void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::information(this, "提示", "按钮点击成功！");
}

// 打开子窗口按钮逻辑
void MainWindow::on_pushButton_open_sub_clicked()
{
    SubWindow subWin(this);
    subWin.setModal(true);
    // 传递主窗口输入框内容到子窗口
    subWin.setContent(input->text());
    subWin.exec();
}

// 【文件-新建】菜单逻辑（示例：清空输入框）
void MainWindow::on_action_New_triggered()
{
    ui->lineEdit->clear(); // 清空主窗口输入框
    QMessageBox::information(this, "新建", "已清空输入框，准备新建内容！");
}

// 【文件-打开】菜单逻辑（示例：提示）
void MainWindow::on_action_Open_triggered()
{
    QMessageBox::information(this, "打开", "打开文件功能（可扩展为文件选择对话框）！");
}

// 【文件-退出】菜单逻辑（关闭程序）
void MainWindow::on_action_Exit_triggered()
{
    this->close(); // 关闭主窗口，程序退出
}

// 【帮助-关于】菜单逻辑（显示程序信息）
void MainWindow::on_action_About_triggered()
{
    QString aboutText = "Qt 桌面应用 v1.0.0\n"
                        "开发者：你自己\n"
                        "功能：多窗口 + 菜单栏 + 基础交互";
    QMessageBox::about(this, "关于程序", aboutText);
}
void MainWindow::on_actionToolExit_triggered()
{
    on_action_Exit_triggered(); // 复用文件-退出的逻辑
}




