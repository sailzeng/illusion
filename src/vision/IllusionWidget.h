#ifndef PROTO_DIR_TAB_H
#define PROTO_DIR_TAB_H



class IllusionWidget : public QSplitter
{
	Q_OBJECT

public:
	IllusionWidget(QWidget *parent);
	~IllusionWidget();

	//!
	void loead_illusion();

protected:
	//
	enum TREE_ITEM_TYPE
	{
		ITEM_PROTO_FILE = 1001,
		ITEM_PROTO_TABLE_MESSAGE = 1002,
		ITEM_PROTO_LINE_MESSAGE = 1003,
		ITEM_EXCEL_FILE = 2001,
		ITEM_EXCEL_SHEET = 2002,
		ITEM_OUTER_FILE = 3001,
	};
	
	//!安装UI
	void setup_ui();

	//!显示PROTO文件
	void show_proto_file(const QString &file_name);

	//!显示Outer文件，当然是文本的
	void show_outer_file(const QString &file_name);

	// SLOT
public:

	//!
	void item_double_clicked(QTreeWidgetItem*, int);

public:

	//!选择所有的Mesage
	void select_all();

	//!不选所有的Message
	void select_none();

	//!取得所有的取得的ITEM
	void selected_item(QStringList &selected_message);

protected:

	//!PROTO 文件信息
	QTreeWidget *proto_dir_tree_ = NULL;

	//!测试棋盘的操作
	QTabWidget *show_illusion_tab_ = NULL;

	//!
	QTextEdit *show_readme_ = NULL;
	
	//!
	QSTRING_2_ILLUSIONARY_MAP *proto_2_ils_map_ = NULL;


};

#endif // PROTO_DIR_TAB_H

