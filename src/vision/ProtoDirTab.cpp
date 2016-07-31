#include "stdafx.h"
#include "ProtoDirTab.h"

ProtoDirTab::ProtoDirTab(QWidget *parent)
	: QSplitter(parent)
{
	proto_2_ils_map_ = Biko_Read_Config::instance()->get_proto_illusion_map();
	setup_ui();
}

ProtoDirTab::~ProtoDirTab()
{

}


void ProtoDirTab::setup_ui()
{
	//注意Qt的或者扶对象，布局对象，的销毁会回收子对象，
	this->setContentsMargins(5, 5, 5, 5);
	this->setOrientation(Qt::Horizontal);

	//配置的棋盘滚动区域

	proto_dir_tree_ = new QTreeWidget();
	proto_dir_tree_->setGeometry(QRect(0, 0, 1000, 600));

	this->addWidget(proto_dir_tree_);

	test_opearter_ = new QWidget();
	this->addWidget(test_opearter_);

	this->setStretchFactor(0, 4);
	this->setStretchFactor(1, 6);
}

//!
void ProtoDirTab::loead_illusion()
{
	if (proto_2_ils_map_->size() <= 0)
	{
		return;
	}
	proto_dir_tree_->setColumnCount(2);
	QStringList headers;
	headers << "Proto文件" << "配置需要更新";
	proto_dir_tree_->setHeaderLabels(headers);

	QStringList root_list,child_list;
	//!
	for (auto iter = proto_2_ils_map_->begin(); 
		 iter != proto_2_ils_map_->end();
		 ++iter)
	{
		root_list.clear();
		root_list.append(iter->first);
		QTreeWidgetItem *root = new QTreeWidgetItem(root_list);
		proto_dir_tree_->addTopLevelItem(root);

		for (size_t i = 0; i < iter->second.size(); ++i)
		{
			QTreeWidgetItem *child = NULL;
			child_list.clear();
			child_list.append(QString::fromLocal8Bit("Table Message 名称:") + iter->second[i]->table_message_name_);
			if (iter->second[i]->excelcfg_is_newer_)
			{
				child_list.append(QString::fromLocal8Bit("是"));
			}
			else
			{
				child_list.append(QString::fromLocal8Bit("是"));
			}
			child = new QTreeWidgetItem(child_list);
			root->addChild(child);

			child_list.clear();
			child_list.append(QString::fromLocal8Bit("Line Message 名称:") + iter->second[i]->line_message_name_);
			child = new QTreeWidgetItem(child_list);
			root->addChild(child);

			child_list.clear();
			child_list.append(QString::fromLocal8Bit("EXCEL文件名称:") + iter->second[i]->excel_file_name_);
			child = new QTreeWidgetItem(child_list);
			root->addChild(child);

			child_list.clear();
			child_list.append(QString::fromLocal8Bit("EXCEL SHEET:") + iter->second[i]->excel_sheet_name_);
			child = new QTreeWidgetItem(child_list);
			root->addChild(child);

			child_list.clear();
			child_list.append(QString::fromLocal8Bit("输出文件名称:") + iter->second[i]->outer_file_name_);
			child = new QTreeWidgetItem(child_list);
			root->addChild(child);
		}
	}
}
