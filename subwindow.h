#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QDialog>

namespace Ui {
class SubWindow;
}

class SubWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SubWindow(QWidget *parent = nullptr);
    ~SubWindow();
    // 接收主窗口数据的函数
    void setContent(const QString &text);

private slots:
    // 返回按钮槽函数（对应ui里的 pushButton_back）
    void on_pushButton_back_clicked();

private:
    Ui::SubWindow *ui;
};

#endif // SUBWINDOW_H