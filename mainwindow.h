#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlDatabase>  // 新增：数据库头文件
#include <QSqlQuery>     // 新增：SQL 查询头文件
#include <QSqlError>
#include <QListWidgetItem> // 新增：列表控件头文件
#include <QFile>        // 文件操作
#include <QFileDialog>  // 文件保存对话框
#include <QTextStream>  // 文本流

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // 新增：初始化数据库函数
    bool initSQLiteDB();

    // 新增：删除指定ID的记录
    bool deleteRecord(int id);
    // 新增：带筛选条件的加载函数
    void loadHistoryRecords(const QString &keyword = "");


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

    // 新增槽函数：双击列表项删除记录
    void on_listWidget_history_itemDoubleClicked(QListWidgetItem *item);

    // 新增：清空历史记录
    void on_pushButton_clear_history_clicked();

    // 新增：搜索按钮槽函数
    void on_pushButton_search_clicked();
    // 新增：搜索框回车触发搜索
    void on_lineEdit_search_returnPressed();

    // 新增：导出TXT
    void on_pushButton_export_txt_clicked();
    // 新增：导出CSV
    void on_pushButton_export_csv_clicked();


private:
    Ui::MainWindow *ui;
    QLineEdit *input; // 输入框指针
    QSqlDatabase db; // 新增：数据库对象
};

#endif // MAINWINDOW_H