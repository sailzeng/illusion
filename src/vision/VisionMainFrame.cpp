#include "stdafx.h"
#include "IllusionWidget.h"
#include "SelectEachDirDialog.h"
#include "SelectOneDirDialog.h"
#include "OverrideCursor.h"
#include "VisionMainFrame.h"

//实例指针
VisionMainFrame *VisionMainFrame::instace_ = NULL;

VisionMainFrame::VisionMainFrame(QWidget *parent)
    : QMainWindow(parent)
{
    setup_ui();
    setup_action();

    out_info(PZ_INFO, QString::fromLocal8Bit("好吧，欢迎实用illusion配置转换工具，用于读取转换配置。"));
    out_info(PZ_INFO, QString::fromLocal8Bit("这个图标来自illusion 公司的一个著名软件。"));

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


    illlusion_widget_ = new IllusionWidget(this);
    main_splitter_->addWidget(illlusion_widget_);

    //信息窗口
    info_widget_ = new QTableWidget(main_splitter_);
    info_widget_->setColumnCount(3);
    info_widget_->setRowCount(MAX_TIPS_INFO_NUMBER);
    main_splitter_->addWidget(info_widget_);
    QStringList headers;
    headers << QString::fromLocal8Bit("时间") << QString::fromLocal8Bit("级别") << QString::fromLocal8Bit("信息");
    info_widget_->setHorizontalHeaderLabels(headers);
    info_widget_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    info_widget_->setSelectionBehavior(QAbstractItemView::SelectRows);
    info_widget_->verticalHeader()->setVisible(false);
    info_widget_->setColumnWidth(0, 200);
    info_widget_->setColumnWidth(1, 60);
    info_widget_->setColumnWidth(2, 1000);
    for (size_t i = 0; i < MAX_TIPS_INFO_NUMBER; ++i)
    {
        info_widget_->setRowHeight(i, 18);
    }

    vertical_layout->addWidget(main_splitter_);

    main_splitter_->setStretchFactor(0, 15);
    main_splitter_->setStretchFactor(1, 4);

    this->setCentralWidget(central_widget);


    this->setWindowTitle(QString::fromLocal8Bit("ILLUSION 配置转换工具"));

    m_alarm = new QSound(".\\res\\alarm\\alarm.wav", this);

    QMetaObject::connectSlotsByName(this);
}



void VisionMainFrame::setup_action()
{
    //图标来自
    //https://www.iconfinder.com/icons/474135/construction_home_spanner_tool_wrench_icon#size=32
    //https://www.iconfinder.com/icons/852004/check_clipboard_select_tasks_icon#size=128
    //https://www.iconfinder.com/icons/822958/check_clipboard_select_tasks_icon#size=128
    act_allinone_ = new QAction(QIcon(".\\res\\icon\\allinone.png"),
                                QString::fromLocal8Bit("选择ALLINONE目录，适合所有目录放在一个目录下的处理."),
                                this);
    connect(act_allinone_,
            &QAction::triggered,
            this,
            &VisionMainFrame::allinone_process);

    act_eachdir_ = new QAction(QIcon(".\\res\\icon\\each.png"),
                               QString::fromLocal8Bit("分别选择每个目录，适合目录放在不同地方."),
                               this);
    connect(act_eachdir_,
            &QAction::triggered,
            this,
            &VisionMainFrame::eachdir_process);

    act_select_all_ = new QAction(QIcon(".\\res\\icon\\select_all.png"),
                                  QString::fromLocal8Bit("选择所有的Message 用于导出"),
                                  this);
    connect(act_select_all_,
            &QAction::triggered,
            this,
            &VisionMainFrame::select_all_message);

    act_select_none_ = new QAction(QIcon(".\\res\\icon\\select_none.png"),
                                   QString::fromLocal8Bit("取消所有的选择Message"),
                                   this);
    connect(act_select_none_,
            &QAction::triggered,
            this,
            &VisionMainFrame::select_none_message);

    act_read_all_ = new QAction(QIcon(".\\res\\icon\\read_all.png"),
                                QString::fromLocal8Bit("对所有的选择的Message进行读取"),
                                this);
    connect(act_read_all_,
            &QAction::triggered,
            this,
            &VisionMainFrame::proto_read_all);


    act_exit_ = new QAction(QIcon(".\\res\\icon\\exit.png"),
                            QString::fromLocal8Bit("退出"),
                            this);
    connect(act_exit_,
            &QAction::triggered,
            this,
            &VisionMainFrame::exit_appliaction);

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

    toolbar_ = new QToolBar(this);
    toolbar_->addAction(act_allinone_);
    toolbar_->addAction(act_eachdir_);
    toolbar_->addSeparator();

    toolbar_->addAction(act_select_all_);
    toolbar_->addAction(act_select_none_);
    toolbar_->addAction(act_read_all_);
    toolbar_->addSeparator();

    toolbar_->addAction(act_exit_);
    this->addToolBar(Qt::TopToolBarArea, toolbar_);
}

//
void VisionMainFrame::eachdir_process()
{
    int ret = 0;
    SelectEachDirDialog dialog(instace_);
    if (QDialog::Accepted == dialog.exec())
    {
        QString proto_path;
        QString excel_path;
        QString outer_path;
        QStringList import_list;
        QStringList tips_ary;
        dialog.get_path_str(proto_path, excel_path, outer_path, import_list);
        ret = Biko_Read_Config::instance()->init_read_all(proto_path,
                                                          excel_path,
                                                          outer_path,
                                                          import_list,
                                                          tips_ary);
        out_tips_ary(tips_ary);
        if (0 != ret)
        {
            QMessageBox::critical(VisionMainFrame::instance(),
                                  QString::fromLocal8Bit("错误"),
                                  QString::fromLocal8Bit("初始化失败, 请检查输入参数，留意输出信息区的提示。"));
            return;
        }
        OverrideWaitCursor wait;
        illlusion_widget_->loead_illusion();
    }
}

//
void VisionMainFrame::allinone_process()
{
    int ret = 0;
    SelectOneDirDialog dialog(instace_);
    if (QDialog::Accepted != dialog.exec())
    {
        return;
    }
    QString allinone_path;
    QStringList tips_ary;
    dialog.get_path_str(allinone_path);
    ret = Biko_Read_Config::instance()->init_read_all2(allinone_path,
                                                       tips_ary);
    out_tips_ary(tips_ary);
    if (0 != ret)
    {
        QMessageBox::critical(VisionMainFrame::instance(),
                              QString::fromLocal8Bit("错误"),
                              QString::fromLocal8Bit("初始化失败, 请检查输入参数，留意输出信息区的提示。"));
        return;
    }
    OverrideWaitCursor wait;
    illlusion_widget_->loead_illusion();
}


//退出APP
void VisionMainFrame::exit_appliaction()
{
    QApplication::instance()->quit();
}

void VisionMainFrame::out_tips_ary(const QStringList &tips_ary)
{
for (const QString & tips : tips_ary)
    {
        if (tips[0] == QChar('?'))
        {
            out_info(PZ_DEBUG, tips);
        }
        else if (tips[0] == QChar('.'))
        {
            out_info(PZ_INFO, tips);
        }
        else if (tips[0] == QChar(':'))
        {
            out_info(PZ_WARNNING, tips);
        }
        else if (tips[0] == QChar('!'))
        {
            out_info(PZ_INFO, tips);
        }
        else
        {
            out_info(PZ_INFO, tips);
        }
    }
}

//输出信息
void VisionMainFrame::out_info(PZ_TIP_LEVEL level, const QString &out_info)
{
    if (cur_info_row_ >= MAX_TIPS_INFO_NUMBER)
    {
        //删除掉最早的一条记录
        info_widget_->removeRow(0);
        cur_info_row_--;
        info_widget_->insertRow(MAX_TIPS_INFO_NUMBER - 1);
        info_widget_->setRowHeight(MAX_TIPS_INFO_NUMBER - 1, 18);
    }

    QColor color(Qt::red);
    QDateTime cur_time = QDateTime::currentDateTime();
    info_widget_->setItem(cur_info_row_, 0, new QTableWidgetItem(cur_time.toString("yyyy-dd-MM HH:mm:ss")));

    switch (level)
    {
        case PZ_DEBUG:
            color = Qt::green;
            info_widget_->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("调试")));
            break;
        case PZ_INFO:
            color = Qt::blue;
            info_widget_->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("信息")));
            break;
        case PZ_WARNNING:
            color = Qt::magenta;
            info_widget_->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("警告")));
            m_alarm->play();
            break;
        case PZ_ERROR:
            color = Qt::red;
            info_widget_->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("错误")));
            m_alarm->play();
            break;
    }
    info_widget_->setItem(cur_info_row_, 2, new QTableWidgetItem(out_info));
    info_widget_->setCurrentCell(cur_info_row_, 1);

    QTableWidgetItem *table_item = NULL;
    table_item = info_widget_->item(cur_info_row_, 0);
    table_item->setTextColor(color);
    table_item = info_widget_->item(cur_info_row_, 1);
    table_item->setTextColor(color);
    table_item = info_widget_->item(cur_info_row_, 2);
    table_item->setTextColor(color);

    cur_info_row_++;
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
                                  QVariant &data)
{
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
    data = *iter_find;
    return true;
}

//!写入配置
bool VisionMainFrame::write_config(const QString &section,
                                   const QString &key,
                                   const QVariant &data)
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


//!PROTO TAB页面选择所有的Message
void VisionMainFrame::select_all_message()
{
    illlusion_widget_->select_all();
}

//!PROTO TAB页面所有的Message都不选
void VisionMainFrame::select_none_message()
{
    illlusion_widget_->select_none();
}

//!PROTO TAB页面
void VisionMainFrame::proto_read_all()
{
    QStringList selected_msgary;
    illlusion_widget_->selected_item(selected_msgary);
    if (selected_msgary.size() <= 0)
    {
        QMessageBox::critical(VisionMainFrame::instance(),
                              QString::fromLocal8Bit("错误"),
                              QString::fromLocal8Bit("没有选择Message或者还咩有初始化导入Proto文件。请检查"));
        out_info(PZ_ERROR, QString::fromLocal8Bit("没有选择Message或者还咩有初始化导入Proto文件。请检查。咩咩咩"));
        return;
    }

    //!读取
    int ret = 0;
    for (int i = 0; i < selected_msgary.size(); ++i)
    {
        QStringList tips_ary;
        ret |= Biko_Read_Config::instance()->read_one_message(selected_msgary[i], tips_ary);
        out_tips_ary(tips_ary);
    }
    if (0 != ret)
    {
        QMessageBox::critical(VisionMainFrame::instance(),
                              QString::fromLocal8Bit("错误"),
                              QString::fromLocal8Bit("导出过程存在错误, 请检查输入参数，留意输出信息区的提示。"));
        return;
    }
}