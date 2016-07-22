#include "stdafx.h"
#include "VisionMainFrame.h"

VisionMainFrame::VisionMainFrame(QWidget *parent)
	: QMainWindow(parent)
{
	setup_ui();
	setup_action();

	out_info(PZ_INFO, QString::fromLocal8Bit("好吧，你如果看懂了这个工具的图标，你的年纪应该也和差不多把，老头你好。"));
	out_info(PZ_INFO, QString::fromLocal8Bit("这游戏的片尾曲叫《Eyes on me》，王菲当年嗓音正是余音绕梁的时间，当然动画也很好看。"));
	out_info(PZ_INFO, QString::fromLocal8Bit("他说六扇门里太龌龊 不如六根弦上取磊落 他说六扇门里太龌龊 不如六根弦上取磊落 他说六扇门里太龌龊 不如键盘上取磊落"));
	out_info(PZ_INFO, QString::fromLocal8Bit("欢迎您使用这个工具，如果有建议或者反馈bug请联系hunterhli，和seiferan, yvanfyin,sailzeng。"));
	out_info(PZ_INFO, QString::fromLocal8Bit("欢迎您使用这个工具，如果玩法建议请联系mooncake,renahrhuang。"));
}

VisionMainFrame::~VisionMainFrame()
{

}



void VisionMainFrame::setup_ui()
{
	if (this->objectName().isEmpty())
	{
		this->setObjectName(QStringLiteral("this"));
	}
	this->resize(1380, 960);

	//
	QWidget *central_widget = new QWidget(this);
	QVBoxLayout *vertical_layout = new QVBoxLayout(central_widget);
	vertical_layout->setSpacing(6);
	vertical_layout->setContentsMargins(11, 11, 11, 11);

	main_splitter_ = new QSplitter(central_widget);

	main_splitter_->setOrientation(Qt::Vertical);


	main_tab_widget_ = new QTabWidget(main_splitter_);
	main_tab_widget_->setTabPosition(QTabWidget::South);

	config_tab_ = new ConfigChessBoardTab(main_tab_widget_);
	main_tab_widget_->addTab(config_tab_, QString::fromLocal8Bit("关卡设计"));
	simulator_tab_ = new SimulatorTab(main_tab_widget_);
	main_tab_widget_->addTab(simulator_tab_, QString::fromLocal8Bit("跑分器"));
	statist_tab_ = new StatistPuzzleInfoTab(main_tab_widget_);
	main_tab_widget_->addTab(statist_tab_, QString::fromLocal8Bit("统计关卡"));
	comparedir_puzzle_tab_ = new CompareDirPuzzleTab(main_tab_widget_);
	main_tab_widget_->addTab(comparedir_puzzle_tab_, QString::fromLocal8Bit("关卡比较"));
	elements_protperties_tab_ = new ElementsPropertiesTab(main_tab_widget_);
	main_tab_widget_->addTab(elements_protperties_tab_, QString::fromLocal8Bit("元素设置"));

	main_splitter_->addWidget(main_tab_widget_);

	//信息窗口
	m_info_widget = new QTableWidget(main_splitter_);
	m_info_widget->setColumnCount(3);
	m_info_widget->setRowCount(MAX_TIPS_INFO_NUMBER);
	main_splitter_->addWidget(m_info_widget);
	QStringList headers;
	headers << QString::fromLocal8Bit("时间") << QString::fromLocal8Bit("级别") << QString::fromLocal8Bit("信息");
	m_info_widget->setHorizontalHeaderLabels(headers);
	m_info_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_info_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_info_widget->verticalHeader()->setVisible(false);
	m_info_widget->setColumnWidth(0, 200);
	m_info_widget->setColumnWidth(1, 60);
	m_info_widget->setColumnWidth(2, 800);
	for (size_t i = 0; i < MAX_TIPS_INFO_NUMBER; ++i)
	{
		m_info_widget->setRowHeight(i, 18);
	}

	vertical_layout->addWidget(main_splitter_);

	main_splitter_->setStretchFactor(0, 15);
	main_splitter_->setStretchFactor(1, 4);

	this->setCentralWidget(central_widget);

	main_tab_widget_->setCurrentIndex(0);

	this->setWindowTitle(QString::fromLocal8Bit("We6PuzzleDesigner 棋局设计"));

	m_alarm = new QSound(".\\res\\alarm\\alarm.wav", this);

	QMetaObject::connectSlotsByName(this);
}



void VisionMainFrame::setup_action()
{
	//图标来自
	// https://www.iconfinder.com/icons/196690/edit_pencil_icon#size=32

	act_new_ = new QAction(QIcon(".\\res\\icon\\new.png"),
						   QString::fromLocal8Bit("新建？蓉儿非要个新建，每次重新打开不行吗？"),
						   this);
	connect(act_new_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::new_cfg_file);

	act_open_ = new QAction(QIcon(".\\res\\icon\\open.png"),
							QString::fromLocal8Bit("打开(CTRL+O)"),
							this);
	connect(act_open_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::open_cfg_file);
	act_save_ = new QAction(QIcon(".\\res\\icon\\save.png"),
							QString::fromLocal8Bit("保存(Ctrl+S)"),
							this);
	connect(act_save_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::save_cfg_file);
	act_saveas_ = new QAction(QIcon(".\\res\\icon\\saveas.png"),
							  QString::fromLocal8Bit("另存为(F12)"),
							  this);
	connect(act_saveas_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::saveas_cfg_file);

	act_undo_ = new QAction(QIcon(".\\res\\icon\\undo.png"),
							QString::fromLocal8Bit("撤销(Ctrl+Z)"),
							this);
	connect(act_undo_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::go_back);

	act_redo_ = new QAction(QIcon(".\\res\\icon\\redo.png"),
							QString::fromLocal8Bit("重做(Ctrl+Y)"),
							this);
	connect(act_redo_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::go_front);

	act_statist_ = new QAction(QIcon(".\\res\\icon\\statist.png"),
							   QString::fromLocal8Bit("统计一个目录下的配置文件"),
							   this);
	connect(act_statist_,
			&QAction::triggered,
			statist_tab_,
			&StatistPuzzleInfoTab::statist_puzzle_dir);

	act_statresult_ = new QAction(QIcon(".\\res\\icon\\statresult.png"),
								  QString::fromLocal8Bit("保存统计的结果到EXCEL文件，保存EXCEL非常慢，"
														 "桃子可以给微软提单，我是为你们呕心沥血了"),
								  this);
	connect(act_statresult_,
			&QAction::triggered,
			statist_tab_,
			&StatistPuzzleInfoTab::save_statist_result);

	act_surprise_ = new QAction(QIcon(".\\res\\icon\\surprise.png"),
								QString::fromLocal8Bit("surprise me. 这不是《美食总动员》的台词，这是产生一个随机的棋盘样子."),
								this);
	connect(act_surprise_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::surprise_puzzle);

	act_checkdir = new QAction(QIcon(".\\res\\icon\\checkdir.png"),
							   QString::fromLocal8Bit("检查目录下所有的配置文件"),
							   this);
	connect(act_checkdir,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::on_check_dir);

	act_fill_ = new QAction(QIcon(".\\res\\icon\\fill.png"),
							QString::fromLocal8Bit("一键填充随机棋子"),
							this);
	connect(act_fill_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::fill_random);

	act_compare_ = new QAction(QIcon(".\\res\\icon\\compare.png"),
							   QString::fromLocal8Bit("比较两个目录的文件"),
							   this);


	act_special_ = new QAction(QIcon(".\\res\\icon\\special.png"),
							   QString::fromLocal8Bit("特殊服务"),
							   this);


	act_exit_ = new QAction(QIcon(".\\res\\icon\\exit.png"),
							QString::fromLocal8Bit("退出"),
							this);
	connect(act_exit_,
			&QAction::triggered,
			this,
			&PuzzleMainFrame::exit_appliaction);



	/* 菜单，暂时不想用这个，
	m_menubar = new QMenuBar(this);
	m_menubar->setGeometry(QRect(0, 0, 1242, 23));
	m_mainmenu = new QMenu(QString::fromLocal8Bit("文件"),m_menubar);
	this->setMenuBar(m_menubar);

	m_menubar->addAction(m_mainmenu->menuAction());
	m_mainmenu->addAction(m_act_open);
	m_mainmenu->addAction(m_act_save);
	m_mainmenu->addAction(m_act_saveas);
	m_mainmenu->addSeparator();
	m_mainmenu->addAction(m_act_exit);
	*/

	m_toolbar = new QToolBar(this);
	m_toolbar->addAction(act_new_);
	m_toolbar->addAction(act_open_);
	m_toolbar->addAction(act_save_);
	m_toolbar->addAction(act_saveas_);
	m_toolbar->addSeparator();
	m_toolbar->addAction(act_undo_);
	m_toolbar->addAction(act_redo_);
	m_toolbar->addSeparator();
	m_toolbar->addAction(act_statist_);
	m_toolbar->addAction(act_statresult_);
	m_toolbar->addSeparator();
	m_toolbar->addAction(act_checkdir);

	m_toolbar->addSeparator();
	m_toolbar->addAction(act_surprise_);
	m_toolbar->addSeparator();
	m_toolbar->addAction(act_compare_);
	m_toolbar->addAction(act_compresult_);


	m_toolbar->addSeparator();
	m_toolbar->addAction(act_special_);
	m_toolbar->addAction(act_fill_);

	m_toolbar->addSeparator();
	m_toolbar->addAction(act_exit_);
	this->addToolBar(Qt::TopToolBarArea, m_toolbar);
}

//退出APP
void VisionMainFrame::exit_appliaction()
{
	//QAction *evt_act = qobject_cast<QAction *>(sender());
	QApplication::instance()->quit();
}



//输出信息
void VisionMainFrame::out_info(PZ_TIP_LEVEL level, const QString &out_info)
{
	if (cur_info_row_ >= MAX_TIPS_INFO_NUMBER)
	{
		//删除掉最早的一条记录
		m_info_widget->removeRow(0);
		cur_info_row_--;
		m_info_widget->insertRow(MAX_TIPS_INFO_NUMBER - 1);
		m_info_widget->setRowHeight(MAX_TIPS_INFO_NUMBER - 1, 18);
	}

	QColor color(Qt::red);
	QDateTime cur_time = QDateTime::currentDateTime();
	m_info_widget->setItem(cur_info_row_, 0, new QTableWidgetItem(cur_time.toString("yyyy-dd-MM HH:mm:ss")));

	switch (level)
	{
	case PZ_DEBUG:
		color = Qt::green;
		m_info_widget->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("调试")));
		break;
	case PZ_INFO:
		color = Qt::blue;
		m_info_widget->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("信息")));
		break;
	case PZ_WARNNING:
		color = Qt::magenta;
		m_info_widget->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("警告")));
		m_alarm->play();
		break;
	case PZ_ERROR:
		color = Qt::red;
		m_info_widget->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("错误")));
		m_alarm->play();
		break;
	}
	m_info_widget->setItem(cur_info_row_, 2, new QTableWidgetItem(out_info));
	m_info_widget->setCurrentCell(cur_info_row_, 1);

	QTableWidgetItem *table_item = NULL;
	table_item = m_info_widget->item(cur_info_row_, 0);
	table_item->setTextColor(color);
	table_item = m_info_widget->item(cur_info_row_, 1);
	table_item->setTextColor(color);
	table_item = m_info_widget->item(cur_info_row_, 2);
	table_item->setTextColor(color);

	cur_info_row_++;
}

//
void VisionMainFrame::select_tab(int tab_id)
{
	main_tab_widget_->setCurrentIndex(tab_id);
}

//
VisionMainFrame *VisionMainFrame::configboard_tab()
{
	return config_tab_;
}

//设置实例指针
void VisionMainFrame::set_instance(VisionMainFrame *inst)
{
	instace_ = inst;
}
//获取实例指针
VisionMainFrame *VisionMainFrame::instance()
{
	return instace_;
}

bool VisionMainFrame::read_config(const QString &section,
								  const QString &key,
								  const QString &defult_data,
								  QString &data
)
{
	data = defult_data;
	QFile json_file(".\\cfg\\local_config.json");
	bool rd_ok = json_file.open(QIODevice::ReadOnly);
	if (!rd_ok)
	{
		return false;
	}

	//读取Json
	QByteArray b = json_file.readAll();
	QJsonParseError json_error;
	QJsonDocument json_doc = QJsonDocument::fromJson(b, &json_error);
	if (json_error.error != QJsonParseError::NoError)
	{
		return false;
	}

	if (!json_doc.isObject())
	{
		return false;
	}

	QVariantMap section_result = json_doc.toVariant().toMap();
	QVariantMap::const_iterator iter_find = section_result.find(section);
	if (section_result.end() == iter_find)
	{
		return false;
	}
	QVariantMap key_result = iter_find->toMap();
	iter_find = key_result.find(key);
	if (key_result.end() == iter_find)
	{
		return false;
	}

	//终于找到了
	data = iter_find->toString();
	return true;
}

//!写入配置
bool VisionMainFrame::write_config(const QString &section,
								   const QString &key,
								   const QString &data)
{
	QFile json_file(".\\cfg\\local_config.json");
	bool rd_ok = json_file.open(QIODevice::ReadWrite);
	if (!rd_ok)
	{
		QMessageBox::critical(VisionMainFrame::instance(),
							  QString::fromLocal8Bit("错误"),
							  QString::fromLocal8Bit("本地配置文件[.\\cfg\\local_config.json]无法正确读取,请检查"));
		return false;
	}

	//读取Json
	QByteArray b = json_file.readAll();
	QJsonParseError json_error;
	QJsonDocument json_doc = QJsonDocument::fromJson(b, &json_error);


	QVariantMap key_result;

	QVariantMap section_result = json_doc.toVariant().toMap();
	QVariantMap::const_iterator iter_find = section_result.find(section);
	if (section_result.end() == iter_find)
	{
		key_result.insert(key, data);
		section_result.insert(section, key_result);
	}
	else
	{
		key_result = iter_find->toMap();
		key_result[key] = data;
		section_result.insert(section, key_result);
	}

	QJsonDocument write_doc = QJsonDocument::fromVariant(section_result);
	json_file.resize(0);
	qint64 wt_ok = json_file.write(write_doc.toJson());
	if (wt_ok <= 0)
	{
		QMessageBox::critical(VisionMainFrame::instance(),
							  QString::fromLocal8Bit("错误"),
							  QString::fromLocal8Bit("本地配置文件[.\\cfg\\local_config.json]无法正确写入,请检查"));
		return false;
	}
	json_file.close();

	return true;
}

