#ifndef PROTO_DIR_TAB_H
#define PROTO_DIR_TAB_H



class IllusionWidget : public QSplitter
{
    Q_OBJECT

public:
    IllusionWidget(QWidget *parent);
    ~IllusionWidget();

    //!加载显示所有的ILLUSION 信息
    void load_illusion();

	

protected:
    //
    enum TREE_ITEM_TYPE
    {
        ITEM_PROTO_ROOT = 1001,
        ITEM_PROTO_FILE = 1002,
        ITEM_PROTO_TABLE_MESSAGE = 1003,
        ITEM_PROTO_LINE_MESSAGE = 1004,
        ITEM_PROTO_COMMENT_NAME = 1005,
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

    //!显示Line Message
    void show_line_message(const QString &line_name,
                           const Illusion_Message *ils_msg);

    //!显示EXCEL的Sheet
    void show_excel_sheet(const QString &file_name,
                          const QString &sheet_name);

	//!加载按PROTO 文件分类的ILLUSION 
	void load_proto_illusion();

	//!加载每个EXCEL SHEET表格的ILLUSION 
	void load_sheetmsg_illusion();

    // SLOT
protected:

    //!
    void item_double_clicked(QTreeWidgetItem *, int);

    //!
    void close_tab(int index);

public:

    //!选择所有的Mesage
    void select_all();

    //!不选所有的Message
    void select_none();

    //!取得所有的取得的ITEM
    void selected_item(QStringList &selected_message);

protected:

	//!PROTO 文件信息
	QTreeWidget *sheet_msg_tree_ = NULL;
    //!PROTO 文件信息
    QTreeWidget *proto_file_tree_ = NULL;


    //!测试棋盘的操作
    QTabWidget *show_config_tab_ = NULL;

	//!显示文件各种表格
	QTabWidget *show_tree_tab_ = NULL;

    //!
    QTextEdit *show_readme_ = NULL;

    //!
    QSTRING_2_ILLUSIONARY_MAP *proto_2_ils_map_ = NULL;

	//!
	ILLUSION_MESSAGE_ARRAY *illusion_sheetmsg_ary_ = NULL;
};

#endif // PROTO_DIR_TAB_H

