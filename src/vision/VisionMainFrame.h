#ifndef VISIONMAINWND_H
#define VISIONMAINWND_H

#include <QtWidgets/QMainWindow>


//
enum PZ_TIP_LEVEL
{
	PZ_DEBUG,
	PZ_INFO,
	PZ_WARNNING,
	PZ_ERROR,
};


class VisionMainFrame : public QMainWindow
{
	Q_OBJECT

public:
	VisionMainFrame(QWidget *parent = 0);
	~VisionMainFrame();


	//输出信息
	void out_info(PZ_TIP_LEVEL level, const QString &out_info);


protected:

	//!整个UI处理
	void setup_ui();

	//!菜单处理
	void setup_action();

	//!程序退出
	void exit_appliaction();

public:

	//设置实例指针
	static void set_instance(VisionMainFrame *inst);
	//获取实例指针
	static VisionMainFrame *instance();

	//!读取配置
	static bool read_config(const QString &section,
							const QString &key,
							const QString &defult_data,
							QString &data);

	//!写入配置
	static bool write_config(const QString &section,
							 const QString &key,
							 const QString &data);

	//实例指针
	static VisionMainFrame *instace_;

private:

	//!Action
	QAction *act_new_ = NULL;
	QAction *act_open_ = NULL;
	QAction *act_save_ = NULL;
	QAction *act_saveas_ = NULL;
	QAction *act_undo_ = NULL;
	QAction *act_redo_ = NULL;
	QAction *act_statist_ = NULL;
	QAction *act_statresult_ = NULL;
	QAction *act_checkdir = NULL;
	QAction *act_surprise_ = NULL;
	QAction *act_compare_ = NULL;
	QAction *act_compresult_ = NULL;
	QAction *act_special_ = NULL;
	QAction *act_fill_ = NULL;
	QAction *act_exit_ = NULL;


	//!分割线
	QSplitter *main_splitter_ = NULL;
	//!主窗口的TAB
	QTabWidget *main_tab_widget_ = NULL;

	//!告警声音
	QSound *m_alarm = NULL;

	//!菜单的menu bar
	QMenuBar *m_menubar;
	//!主菜单
	QMenu *m_mainmenu;

	//!
	QToolBar *m_toolbar = NULL;


	//!List（TABLE）窗口，用于输出信息提示用户。
	QTableWidget *m_info_widget;

	//!当前的信息行
	int cur_info_row_ = 0;
};

#endif // VISIONMAINWND_H
