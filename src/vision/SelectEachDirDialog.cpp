#include "stdafx.h"
#include "VisionMainFrame.h"
#include "SelectEachDirDialog.h"

SelectEachDirDialog::SelectEachDirDialog(QWidget *parent)
    : QDialog(parent)
{
	for (size_t i=0;i<MAX_IMPORT_DIR_SIZE;++i)
	{
		le_import_list_[i] = NULL;
		bt_import_list_[i] = NULL;
	}
    setup_ui();
}

SelectEachDirDialog::~SelectEachDirDialog()
{

}

//!
void SelectEachDirDialog::setup_ui()
{
	QVariant var;
	VisionMainFrame::instance()->read_config("recently", "proto_path", var);
	proto_path_ = var.toString();
	VisionMainFrame::instance()->read_config("recently", "excel_path", var);
	excel_path_ = var.toString();
	VisionMainFrame::instance()->read_config("recently", "outer_path", var);
	outer_path_ = var.toString();
	VisionMainFrame::instance()->read_config("recently", "import_list", var);
	import_list_ = var.toStringList();

    //去掉帮助按钮
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    QLabel *lb_tmp = NULL;
    resize(750, 10);
    setWindowTitle(QString::fromLocal8Bit("分别设置各种路径"));
    QGridLayout *grid_layout = new QGridLayout(this);
    grid_layout->setSpacing(6);
    grid_layout->setContentsMargins(5, 5, 5, 5);

	QLabel *lbl_tmp = NULL;
    int line_use = 0;

	lbl_tmp = new QLabel(QString::fromLocal8Bit("分别设置各个目录，适用目录不放在一起的情况。"));
	grid_layout->addWidget(lbl_tmp, line_use, 0, 1, 30);

	++line_use;
	lbl_tmp = new QLabel(QString::fromLocal8Bit("PROTO文件目录："));
    grid_layout->addWidget(lbl_tmp, line_use, 0, 1, 1);
	le_proto_dir_ = new QLineEdit();
    grid_layout->addWidget(le_proto_dir_, line_use, 1, 1, 30);
	bt_proto_dir_ = new QPushButton(QString::fromLocal8Bit("选择目录"));
    grid_layout->addWidget(bt_proto_dir_, line_use, 31, 1, 1);
    connect(bt_proto_dir_, &QPushButton::clicked, this, &SelectEachDirDialog::on_select_dir);


    ++line_use;
	lbl_tmp = new QLabel(QString::fromLocal8Bit("EXCEL文件目录："));
	grid_layout->addWidget(lbl_tmp, line_use, 0, 1, 1);
	le_excel_dir_ = new QLineEdit();
	grid_layout->addWidget(le_excel_dir_, line_use, 1, 1, 30);
	bt_excel_dir_ = new QPushButton(QString::fromLocal8Bit("选择目录"));
	grid_layout->addWidget(bt_excel_dir_, line_use, 31, 1, 1);
	connect(bt_excel_dir_, &QPushButton::clicked, this, &SelectEachDirDialog::on_select_dir);

	++line_use;
	lbl_tmp = new QLabel(QString::fromLocal8Bit("OUTER文件目录："));
	grid_layout->addWidget(lbl_tmp, line_use, 0, 1, 1);
	le_outer_dir_ = new QLineEdit();
	grid_layout->addWidget(le_outer_dir_, line_use, 1, 1, 30);
	bt_outer_dir_ = new QPushButton(QString::fromLocal8Bit("选择目录"));
	grid_layout->addWidget(bt_outer_dir_, line_use, 31, 1, 1);
	connect(bt_outer_dir_, &QPushButton::clicked, this, &SelectEachDirDialog::on_select_dir);


	++line_use;
	lbl_tmp = new QLabel(QString::fromLocal8Bit("IMPORT目录1(可选)："));
	grid_layout->addWidget(lbl_tmp, line_use, 0, 1, 1);
	le_import_list_[0] = new QLineEdit();
	grid_layout->addWidget(le_import_list_[0], line_use, 1, 1, 30);
	bt_import_list_[0] = new QPushButton(QString::fromLocal8Bit("选择目录"));
	grid_layout->addWidget(bt_import_list_[0], line_use, 31, 1, 1);
	connect(bt_import_list_[0], &QPushButton::clicked, this, &SelectEachDirDialog::on_select_dir);

	++line_use;
	lbl_tmp = new QLabel(QString::fromLocal8Bit("IMPORT目录2(可选)："));
	grid_layout->addWidget(lbl_tmp, line_use, 0, 1, 1);
	le_import_list_[1] = new QLineEdit();
	grid_layout->addWidget(le_import_list_[1], line_use, 1, 1, 30);
	bt_import_list_[1] = new QPushButton(QString::fromLocal8Bit("选择目录"));
	grid_layout->addWidget(bt_import_list_[1], line_use, 31, 1, 1);
	connect(bt_import_list_[1], &QPushButton::clicked, this, &SelectEachDirDialog::on_select_dir);

	++line_use;
	lbl_tmp = new QLabel(QString::fromLocal8Bit("IMPORT目录3(可选)："));
	grid_layout->addWidget(lbl_tmp, line_use, 0, 1, 1);
	le_import_list_[2] = new QLineEdit();
	grid_layout->addWidget(le_import_list_[2], line_use, 1, 1, 30);
	bt_import_list_[2] = new QPushButton(QString::fromLocal8Bit("选择目录"));
	grid_layout->addWidget(bt_import_list_[2], line_use, 31, 1, 1);
	connect(bt_import_list_[2], &QPushButton::clicked, this, &SelectEachDirDialog::on_select_dir);

	++line_use;
	lbl_tmp = new QLabel(QString::fromLocal8Bit("IMPORT目录3(可选)："));
	grid_layout->addWidget(lbl_tmp, line_use, 0, 1, 1);
	le_import_list_[3] = new QLineEdit();
	grid_layout->addWidget(le_import_list_[3], line_use, 1, 1, 30);
	bt_import_list_[3] = new QPushButton(QString::fromLocal8Bit("选择目录"));
	grid_layout->addWidget(bt_import_list_[3], line_use, 31, 1, 1);
	connect(bt_import_list_[3], &QPushButton::clicked, this, &SelectEachDirDialog::on_select_dir);

	++line_use;
	lbl_tmp = new QLabel(QString::fromLocal8Bit("IMPORT目录3(可选)："));
	grid_layout->addWidget(lbl_tmp, line_use, 0, 1, 1);
	le_import_list_[4] = new QLineEdit();
	grid_layout->addWidget(le_import_list_[4], line_use, 1, 1, 30);
	bt_import_list_[4] = new QPushButton(QString::fromLocal8Bit("选择目录"));
	grid_layout->addWidget(bt_import_list_[4], line_use, 31, 1, 1);
	connect(bt_import_list_[4], &QPushButton::clicked, this, &SelectEachDirDialog::on_select_dir);

    ++line_use;
    QPushButton *ok_button = new QPushButton(QString::fromLocal8Bit("确认"));
    grid_layout->addWidget(ok_button, line_use, 10, 1, 1);
    connect(ok_button, &QPushButton::clicked, this, &SelectEachDirDialog::on_ok_clicked);

    QPushButton *cancel_button = new QPushButton(QString::fromLocal8Bit("取消"));
    grid_layout->addWidget(cancel_button, line_use, 18, 1, 1);
    connect(cancel_button, &QPushButton::clicked, this, &SelectEachDirDialog::on_cancel_clicked);

    //connect(this, &QDialog::finished, this, &SelectTwoDirDialog::on_ok_clicked);
}

//
void SelectEachDirDialog::on_select_dir()
{
    QFileDialog file_dialog;

    file_dialog.setWindowTitle(QString::fromLocal8Bit("选择棋局配置文件的目录"));
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

    if (bt_obj == bt_proto_dir_)
    {
        le_proto_dir_->setText(selected_path);
    }
	else if (bt_obj == bt_excel_dir_)
	{
		le_excel_dir_->setText(selected_path);
	}
    else if (bt_obj == bt_outer_dir_)
    {
        le_outer_dir_->setText(selected_path);
    }
	else if (bt_obj == bt_import_list_[0])
	{
		le_import_list_[0]->setText(selected_path);
	}
	else if (bt_obj == bt_import_list_[1])
	{
		le_import_list_[1]->setText(selected_path);
	}
	else if (bt_obj == bt_import_list_[2])
	{
		le_import_list_[2]->setText(selected_path);
	}
	else if (bt_obj == bt_import_list_[3])
	{
		le_import_list_[3]->setText(selected_path);
	}
	else if (bt_obj == bt_import_list_[4])
	{
		le_import_list_[4]->setText(selected_path);
	}
    else
    {
        Q_ASSERT(false);
    }
}


void SelectEachDirDialog::on_ok_clicked()
{

	proto_path_ = le_proto_dir_->text();
    QFileInfo file_info(proto_path_);
    if (!file_info.isDir())
    {
        QMessageBox::warning(this,
                             QString::fromLocal8Bit("警告"),
                             QString::fromLocal8Bit("请选择PROTO目录,或者输入的PROTO目录不是真正的目录。")
                            );
		le_proto_dir_->setFocus();
        return;
    }
	excel_path_ = le_excel_dir_->text();
    file_info.setFile(excel_path_);
    if (!file_info.isDir())
    {
        QMessageBox::warning(this,
                             QString::fromLocal8Bit("警告"),
                             QString::fromLocal8Bit("请选择EXCEL目录,或者输入的EXCEL目录不是真正的目录。"));
		le_excel_dir_->setFocus();
        return;
    }
	outer_path_ = le_outer_dir_->text();
	file_info.setFile(outer_path_);
	if (!file_info.isDir())
	{
		QMessageBox::warning(this,
							 QString::fromLocal8Bit("警告"),
							 QString::fromLocal8Bit("请选择OUTER目录,或者输入的OUTER目录不是真正的目录。"));
		le_excel_dir_->setFocus();
		return;
	}

	for (size_t i = 0; i < MAX_IMPORT_DIR_SIZE; ++i)
	{
		QString import_dir = le_import_list_[i]->text();
		if (import_dir.isEmpty())
		{
			continue;
		}
		file_info.setFile(import_dir);
		if (!file_info.isDir())
		{
			QMessageBox::warning(this,
								 QString::fromLocal8Bit("警告"),
								 QString::fromLocal8Bit("请选择IMPORT[%1]目录,或者输入的IMPORT目录不是真正的目录。").
							     arg(i));
			le_import_list_[i]->setFocus();
			return;
		}

		import_list_.append(le_import_list_[i]->text());
	}

    this->setResult(QDialog::Accepted);
    accept();
    this->close();

	VisionMainFrame::instance()->write_config("recently", "proto_path", proto_path_);
	VisionMainFrame::instance()->write_config("recently", "excel_path", excel_path_);
	VisionMainFrame::instance()->write_config("recently", "outer_path", outer_path_);
	VisionMainFrame::instance()->write_config("recently", "import_list", import_list_);

    return;
}

//!取消点击
void SelectEachDirDialog::on_cancel_clicked()
{
    this->setResult(QDialog::Rejected);
    rejected();
    this->close();
}

//得到用户选择的目录
void SelectEachDirDialog::get_path_str(QString &proto_path,
										 QString &excel_path,
										 QString &outer_path,
										 QStringList &import_list)
{
	proto_path = proto_path_;
	excel_path = excel_path_;
	outer_path = outer_path_;
	import_list_ = import_list;
}

