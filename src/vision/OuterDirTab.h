#ifndef OUTER_DIR_TAB_H
#define OUTER_DIR_TAB_H



class OuterDirTab : public QSplitter
{
	Q_OBJECT

public:
	OuterDirTab(QWidget *parent);
	~OuterDirTab();

protected:

	///安装UI
	void setup_ui();


protected:



	///测试棋盘的内容
	QWidget *testboard_contents_ = NULL;

	///测试棋盘的操作
	QWidget *test_opearter_ = NULL;




};

#endif // OUTER_DIR_TAB_H
