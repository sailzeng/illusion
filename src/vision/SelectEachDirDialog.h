#ifndef SELECTEACHDIRDIALOG_H
#define SELECTEACHDIRDIALOG_H



class QLabel;
class QLineEdit;
class QPushButton;

class SelectEachDirDialog : public QDialog
{
    Q_OBJECT

public:

    //
    SelectEachDirDialog(QWidget *parent);
    ~SelectEachDirDialog();


private:

    //!设计UI
    void setup_ui();

    //!SLOT
    //! 选择目录
    void on_select_dir();

    //!确认点击
    void on_ok_clicked();

    //!取消点击
    void on_cancel_clicked();

public:

    //!得到用户选择的目录
    void get_path_str(QString &proto_path,
                      QString &excel_path,
					  QString &outer_path,
					  QStringList &import_list) const;
private:

	//
	static const size_t MAX_IMPORT_DIR_SIZE = 5;

private:


    //!文件目录输入框
	
    QLineEdit *le_proto_dir_ = NULL;
    QLineEdit *le_excel_dir_ = NULL;
	QLineEdit *le_outer_dir_ = NULL;
	QLineEdit *le_import_list_[MAX_IMPORT_DIR_SIZE];

    //!选择目录的目录
    QPushButton *bt_proto_dir_ = NULL;
    QPushButton *bt_outer_dir_ = NULL;
	QPushButton *bt_excel_dir_ = NULL;
	QPushButton *bt_import_list_[MAX_IMPORT_DIR_SIZE];
	


    //!文件目录的字符串
    QString proto_path_;
    QString excel_path_;
	QString outer_path_;

	//!import的path列表，可以有多个
	QStringList import_list_;

};

#endif // SELECTEACHDIRDIALOG_H
