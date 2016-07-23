#ifndef EXCEL_DIR_H
#define EXCEL_DIR_H



class ExcelDirTab : public QSplitter
{
    Q_OBJECT

public:
    ExcelDirTab(QWidget *parent);
    ~ExcelDirTab();

protected:

    ///安装UI
    void setup_ui();


protected:



    ///测试棋盘的内容
    QWidget *testboard_contents_ = NULL;

    ///测试棋盘的操作
    QWidget *test_opearter_ = NULL;




};

#endif // EXCEL_DIR_H
