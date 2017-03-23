#include "stdafx.h"
#include "VisionMainFrame.h"
#include "SelectOneDirDialog.h"

SelectOneDirDialog::SelectOneDirDialog(QWidget *parent)
    : QDialog(parent)
{

    setup_ui();
}

SelectOneDirDialog::~SelectOneDirDialog()
{

}

//!
void SelectOneDirDialog::setup_ui()
{
    QVariant var;
    VisionMainFrame::instance()->read_config("recently", "allinone", var);
    allinone_path_ = var.toString();
    //去掉帮助按钮
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    QLabel *lb_tmp = NULL;
    resize(750, 10);
    setWindowTitle(QString::fromLocal8Bit("设置ALL IN ONE路径"));
    QGridLayout *grid_layout = new QGridLayout(this);
    grid_layout->setSpacing(6);
    grid_layout->setContentsMargins(5, 5, 5, 5);

    QLabel *lbl_tmp = NULL;
    int line_use = 0;

    lbl_tmp = new QLabel(QString::fromLocal8Bit("设置ALL IN ONE目录，适用目录集中放在一起的情况。"));
    grid_layout->addWidget(lbl_tmp, line_use, 0, 1, 30);

    ++line_use;
    lbl_tmp = new QLabel(QString::fromLocal8Bit("ALL IN ONE文件目录："));
    grid_layout->addWidget(lbl_tmp, line_use, 0, 1, 1);
    le_allinone_dir_ = new QLineEdit();
    le_allinone_dir_->setText(allinone_path_);

    grid_layout->addWidget(le_allinone_dir_, line_use, 1, 1, 30);
    bt_allinone_dir_ = new QPushButton(QString::fromLocal8Bit("选择目录"));
    grid_layout->addWidget(bt_allinone_dir_, line_use, 31, 1, 1);
    connect(bt_allinone_dir_, &QPushButton::clicked, this, &SelectOneDirDialog::on_select_dir);

    ++line_use;
    QPushButton *ok_button = new QPushButton(QString::fromLocal8Bit("确认"));
    grid_layout->addWidget(ok_button, line_use, 10, 1, 1);
    connect(ok_button, &QPushButton::clicked, this, &SelectOneDirDialog::on_ok_clicked);

    QPushButton *cancel_button = new QPushButton(QString::fromLocal8Bit("取消"));
    grid_layout->addWidget(cancel_button, line_use, 18, 1, 1);
    connect(cancel_button, &QPushButton::clicked, this, &SelectOneDirDialog::on_cancel_clicked);
}

//
void SelectOneDirDialog::on_select_dir()
{
    QFileDialog file_dialog;

    file_dialog.setWindowTitle(QString::fromLocal8Bit("选择ALL IN ONE的目录"));
    file_dialog.setDirectory(".\\");
    file_dialog.setFilter(QDir::AllDirs);
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setOption(QFileDialog::ShowDirsOnly);
    file_dialog.setViewMode(QFileDialog::Detail);
    file_dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (file_dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QString selected_path = file_dialog.selectedFiles()[0];

    QObject *bt_obj = sender();

    if (bt_obj == bt_allinone_dir_)
    {
        le_allinone_dir_->setText(selected_path);
    }
    else
    {
        Q_ASSERT(false);
    }
}


void SelectOneDirDialog::on_ok_clicked()
{
    allinone_path_ = le_allinone_dir_->text();
    QFileInfo file_info(allinone_path_);
    if (!file_info.isDir())
    {
        QMessageBox::warning(this,
                             QString::fromLocal8Bit("警告"),
                             QString::fromLocal8Bit("请选择ALL IN ONE目录,或者输入的ALL IN ONE目录不是真正的目录。")
                            );
        le_allinone_dir_->setFocus();
        return;
    }

    this->setResult(QDialog::Accepted);
    accept();
    this->close();
    VisionMainFrame::instance()->write_config("recently", "allinone", allinone_path_);
    return;
}

//!取消点击
void SelectOneDirDialog::on_cancel_clicked()
{
    this->setResult(QDialog::Rejected);
    rejected();
    this->close();
}

//得到用户选择的目录
void SelectOneDirDialog::get_path_str(QString &allinone_path) const
{
    allinone_path = allinone_path_;
}

