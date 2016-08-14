#ifndef SELECTONEDIRDIALOG_H
#define SELECTONEDIRDIALOG_H



class QLabel;
class QLineEdit;
class QPushButton;

class SelectOneDirDialog : public QDialog
{
    Q_OBJECT

public:

    //
	SelectOneDirDialog(QWidget *parent);
    ~SelectOneDirDialog();


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
    void get_path_str(QString &allinone_path) const;

private:


    //!文件目录输入框
    QLineEdit *le_allinone_dir_ = NULL;
    
    //!选择目录的目录
    QPushButton *bt_allinone_dir_ = NULL;

    //!文件目录的字符串
    QString allinone_path_;

};

#endif // SELECTONEDIRDIALOG_H
