#ifndef PROTO_DIR_TAB_H
#define PROTO_DIR_TAB_H



class ProtoDirTab : public QSplitter
{
	Q_OBJECT

public:
	ProtoDirTab(QWidget *parent);
	~ProtoDirTab();

	//!
	void loead_illusion();

protected:

	///安装UI
	void setup_ui();


protected:



	///测试棋盘的内容
	QTreeWidget *proto_dir_tree_ = NULL;

	///测试棋盘的操作
	QWidget *test_opearter_ = NULL;


	QSTRING_2_ILLUSIONARY_MAP *proto_2_ils_map_ = NULL;


};

#endif // PROTO_DIR_TAB_H

