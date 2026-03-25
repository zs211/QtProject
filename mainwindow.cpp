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
    // 4. 程序启动时自动加载历史记录
    loadHistoryRecords();
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

// 改造：支持关键词模糊查询的加载函数
void MainWindow::loadHistoryRecords(const QString &keyword)
{
    ui->listWidget_history->clear();
    QSqlQuery query;

    QString selectSql;
    if (keyword.isEmpty()) {
        // 无关键词：查询所有
        selectSql = "SELECT id, content, create_time FROM input_record ORDER BY create_time DESC";
    } else {
        // 有关键词：模糊查询（LIKE + %通配符）
        selectSql = QString("SELECT id, content, create_time FROM input_record "
                            "WHERE content LIKE '%%1%' ORDER BY create_time DESC").arg(keyword);
    }

    if (!query.exec(selectSql)) {
        QMessageBox::warning(this, "查询失败", query.lastError().text());
        return;
    }

    // 遍历结果（和之前逻辑一致）
    while (query.next()) {
        int id = query.value(0).toInt();
        QString content = query.value(1).toString();
        QString time = query.value(2).toString();

        QString itemText = QString("[%1] %2").arg(time).arg(content);
        QListWidgetItem *item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, id);
        QFont itemFont = item->font();
        itemFont.setPointSize(12);
        item->setFont(itemFont);

        ui->listWidget_history->addItem(item);
    }
}

// 实现：搜索按钮点击
void MainWindow::on_pushButton_search_clicked()
{
    QString keyword = ui->lineEdit_search->text().trimmed(); // 去除首尾空格
    loadHistoryRecords(keyword);
}

// 实现：搜索框回车触发
void MainWindow::on_lineEdit_search_returnPressed()
{
    on_pushButton_search_clicked(); // 复用搜索逻辑
}

// 注意：把构造函数里的 loadHistoryRecords() 保留（启动时加载所有）
// 把「确认」「删除」后的 loadHistoryRecords() 也保留
// 新增：删除指定ID的记录
bool MainWindow::deleteRecord(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM input_record WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        return true;
    } else {
        QMessageBox::critical(this, "删除失败", query.lastError().text());
        return false;
    }
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
        input->clear(); // 清空输入框
        loadHistoryRecords(); // 新增：保存后刷新历史记录
    } else {
        QMessageBox::critical(this, "存储失败", query.lastError().text());
    }
}
// 新增：双击列表项删除记录
void MainWindow::on_listWidget_history_itemDoubleClicked(QListWidgetItem *item)
{
    // 确认删除
    int ret = QMessageBox::question(this, "删除确认", "是否删除这条记录？",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    // 获取隐藏的记录ID
    int id = item->data(Qt::UserRole).toInt();
    if (deleteRecord(id)) {
        // 删除成功后刷新列表
        loadHistoryRecords();
        QMessageBox::information(this, "成功", "记录已删除！");
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

// 新增：清空所有历史记录
void MainWindow::on_pushButton_clear_history_clicked()
{
    // 二次确认，防止误操作
    int ret = QMessageBox::warning(this, "危险操作", "确定要清空所有历史记录吗？\n此操作不可恢复！",
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    QSqlQuery query;
    if (query.exec("DELETE FROM input_record")) {
        ui->listWidget_history->clear(); // 清空列表UI
        QMessageBox::information(this, "成功", "所有历史记录已清空！");
    } else {
        QMessageBox::critical(this, "清空失败", query.lastError().text());
    }
}


// 通用导出函数（封装重复逻辑）
bool exportRecords(const QString &filePath, const QString &format)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "文件错误", "无法打开文件：" + file.errorString());
        return false;
    }

    QTextStream out(&file);
    QSqlQuery query("SELECT id, content, create_time FROM input_record ORDER BY create_time DESC");

    // 写入表头
    if (format == "txt") {
        out << "======= Qt 历史记录导出 =======\n";
        out << "ID\t内容\t创建时间\n";
        out << "-------------------------------\n";
    } else if (format == "csv") {
        out << "ID,内容,创建时间\n"; // CSV 表头（逗号分隔）
    }

    // 写入数据
    while (query.next()) {
        int id = query.value(0).toInt();
        QString content = query.value(1).toString();
        QString time = query.value(2).toString();

        if (format == "txt") {
            out << id << "\t" << content << "\t" << time << "\n";
        } else if (format == "csv") {
            // CSV 转义：内容含逗号时用双引号包裹
            if (content.contains(",")) {
                content = "\"" + content + "\"";
            }
            out << id << "," << content << "," << time << "\n";
        }
    }

    file.close();
    return true;
}

// 导出TXT
void MainWindow::on_pushButton_export_txt_clicked()
{
    // 弹出保存对话框，默认文件名：history_yyyyMMdd.txt
    QString filePath = QFileDialog::getSaveFileName(this, "导出TXT文件",
                                                    QString("history_%1.txt").arg(QDateTime::currentDateTime().toString("yyyyMMdd")),
                                                    "TXT 文件 (*.txt)");

    if (filePath.isEmpty()) return;

    if (exportRecords(filePath, "txt")) {
        QMessageBox::information(this, "导出成功", "记录已导出到：\n" + filePath);
    }
}

// 导出CSV
void MainWindow::on_pushButton_export_csv_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "导出CSV文件",
                                                    QString("history_%1.csv").arg(QDateTime::currentDateTime().toString("yyyyMMdd")),
                                                    "CSV 文件 (*.csv)");

    if (filePath.isEmpty()) return;

    if (exportRecords(filePath, "csv")) {
        QMessageBox::information(this, "导出成功", "记录已导出到：\n" + filePath);
    }
}