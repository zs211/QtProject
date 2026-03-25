#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QMessageBox>
#include "subwindow.h" // 引入子窗口头文件

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 以下槽函数名必须和按钮对象名完全匹配
    void on_pushButton_clicked();         // 确认按钮
    void on_pushButton_2_clicked();       // 成功按钮
    void on_pushButton_3_clicked();       // 提示按钮
    void on_pushButton_open_sub_clicked();// 打开子窗口按钮

    // 新增菜单槽函数
    void on_action_Exit_triggered();       // 退出菜单

    void on_action_New_triggered();        // 新建菜单（示例）
    void on_action_Open_triggered();       // 打开菜单（示例）
    void on_actionToolExit_triggered();

    void on_action_About_triggered();

private:
    Ui::MainWindow *ui;
    QLineEdit *input; // 输入框指针
};

#endif // MAINWINDOW_H