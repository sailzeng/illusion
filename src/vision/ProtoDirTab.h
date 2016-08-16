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

	// SLOT
public:

public:

	//!选择所有的Mesage
	void select_all();

	//!不选所有的Message
	void select_none();

	//!取得所有的取得的ITEM
	void selected_item(QStringList &selected_message);

protected:

	///测试棋盘的内容
	QTreeWidget *proto_dir_tree_ = NULL;

	///测试棋盘的操作
	QWidget *test_opearter_ = NULL;


	QSTRING_2_ILLUSIONARY_MAP *proto_2_ils_map_ = NULL;


};

#endif // PROTO_DIR_TAB_H

