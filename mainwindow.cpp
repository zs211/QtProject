#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "subwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 找到输入框（确保ui里输入框的对象名是 lineEdit）
    input = ui->lineEdit; // 替换原来的 findChild，更稳定

    // 1. 初始化数据库（程序启动时自动执行）
    if (!initSQLiteDB()) {
        QMessageBox::critical(this, "数据库错误", "SQLite 初始化失败！");
    }
}

MainWindow::~MainWindow()
{
    // 关闭数据库连接
    if (db.isOpen()) {
        db.close();
    }
    delete ui;
}
// 新增：初始化 SQLite 数据库
bool MainWindow::initSQLiteDB()
{
    // 1. 添加 SQLite 数据库驱动
    db = QSqlDatabase::addDatabase("QSQLITE");
    // 2. 设置数据库文件路径（程序目录下的 data.db，自动创建）
    db.setDatabaseName("data.db");

    // 3. 打开数据库
    if (!db.open()) {
        QMessageBox::warning(this, "数据库错误", "打开失败：" + db.lastError().text());
        return false;
    }

    // 4. 创建表（用户输入记录表）
    QSqlQuery query;
    QString createSql = "CREATE TABLE IF NOT EXISTS input_record ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"  // 自增ID
                        "content TEXT NOT NULL,"                 // 输入内容
                        "create_time DATETIME DEFAULT CURRENT_TIMESTAMP)"; // 时间戳

    if (!query.exec(createSql)) {
        QMessageBox::warning(this, "建表错误", query.lastError().text());
        return false;
    }

    return true;
}
// 确认按钮逻辑
void MainWindow::on_pushButton_clicked()
{
    QString content = input->text();
    if (content.isEmpty()) {
        QMessageBox::warning(this, "警告", "输入框内容不能为空！");
        return;
    }

    // 插入数据到 SQLite
    QSqlQuery query;
    // 预处理 SQL（防止 SQL 注入，更安全）
    query.prepare("INSERT INTO input_record (content) VALUES (:content)");
    query.bindValue(":content", content);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "内容已保存！\n你输入的是：\n" + content);
    } else {
        QMessageBox::critical(this, "存储失败", query.lastError().text());
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




