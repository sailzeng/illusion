
/*!
* @copyright  2004-2016  Apache License, Version 2.0 FULLSAIL
* @filename   QtAxExcelEngine.h
* @author     yaoboyuan 254200341@qq.com 原创    Sailzeng <sailerzeng@gmail.com> 改写
* @version
* @date       2016年6月10日
* @brief      这是一个使用 Qt's ActiveX(OLE)，QAxObject 读写EXCEL的封装类，便于操作在使用Qt
*             的代码操作EXCEL，既然是实用OLE,你当然必须先安装EXCEL。
*             
*             这是一个便于Qt读写excel封装的类，同时，便于把excel中的数据
*             显示到界面上，或者把界面上的数据写入excel中，同GUI进行交互，关系如下：
*             Qt tableWidget <--> ExcelEngine <--> xls file.
* @details
*
*
*
* @note       ExcelEngine类只负责读/写数据，不负责解析，做中间层
*
* @history
* @author     yaoboyuan 254200341@qq.com
* @date       2012-4-12
* @note       原创
* @author     Sailzeng 改写 <sailerzeng@gmail.com>
* @data       2016-6-7 端午节前夕
* @note       对原有的代码做了一些改动，更加符合Qt的编码格式等。
*             增加了一些常用函数，更加方便实用
*
*/


#ifndef QT_EXCELENGINE_H
#define QT_EXCELENGINE_H


/*!
* @brief
*
* note       代码风格我打算全部采用Qt的代码风格。
*/
class QtAxExcelEngine : protected QObject
{
public:

    //构造函数和析构函数
    QtAxExcelEngine();
    ~QtAxExcelEngine();

public:


    /*!
    * @brief      初始化EXCEL OLE对象，打开EXCEL 进程，
    * @return     bool  返回是否成功打开
    * @param      visible 是否让EXCEL可见
    */
    bool initialize(bool visible);

    //!释放退出，关闭EXCEL进程
    void finalize();

    //打开一个XLS文件
    bool open(const QString &xls_file, int  sheet_index = 1);

    /*!
    * @brief      插入一个sheet，同时加载这个表格，
    * @return     void
    * @param      sheet_name sheet的表名
    */
    void insertSheet(const QString &sheet_name);

    //保存xls报表
    void save();

    //关闭xls报表
    void close();

    //得到sheet的总数
    int sheetsCount();

    //得到某个sheet的名字
    bool sheetName(int sheet_index, QString &sheet_name);

    /*!
    * @brief      根据索引加载sheet，
    * @return     bool 返回是否成功加载
    * @param      sheet_index sheet索引，从1开始
    */
    bool loadSheet(int sheet_index);

    /*!
    * @brief      根据sheet表格表名加载sheet，
    * @return     bool 返回是否成功加载
    * @param      sheet_name 要加载的sheet 的名字
    */
    bool loadSheet(const QString &sheet_name);


    /*!
    * @brief      检查是否有这个名字的sheet
    * @return     bool 有返回true，否则返回false
    * @param      sheet_name
    */
    bool hasSheet(const QString &sheet_name);

    //保存数据到xls
    bool writeTableWidget(QTableWidget *table_widget);

    //从xls读取数据到ui
    bool readTableWidget(QTableWidget *table_widget);

    /*!
    * @brief      获取指定单元数据
    * @return     QVariant  单元格对应的数据
    * @param      row  单元格的行号,注意行号，列号都是从1开始
    * @param      column 单元格的列号
    * @note
    */
    QVariant get_cell(int  row, int  column);

    //修改指定单元数据
    bool  set_cell(int  row, int  column, const QVariant &data);

    //!打开的xls文件名称
    QString open_filename() const;

    //!行数
    int  row_count()const;
    //!列数
    int  column_count()const;


    bool is_open();
    bool is_valid();

protected:

    void clear();

    //!加载，内部函数，以后可以考虑增加一个预加载，加快读取速度。
    void load_sheet_internal(bool pre_load);

public:

    static char *QtAxExcelEngine::column_name(int column_no);

private:

    //!指向整个excel应用程序
    QAxObject *excel_instance_ = NULL;

    //!指向工作簿集,excel有很多工作簿，你可以简单理解EXCEL会打开很多个文件
    QAxObject *work_books_ = NULL;

    //!指向sXlsFile对应的工作簿
    QAxObject *active_book_ = NULL;

    //!
    QAxObject *work_sheets_ = NULL;

    //指向工作簿中的某个sheet表单
    QAxObject *active_sheet_ = NULL;

    //!xls文件路径
    QString xls_file_;

    //!当前打开的第几个sheet
    int curr_sheet_ = 1;
    //!excel是否可见
    bool is_visible_ = false;
    //行数
    int row_count_ = 0;
    //!列数
    int column_count_ = 0;

    //!开始有数据的行下标值
    int start_row_ = 0;
    //!开始有数据的列下标值
    int start_column_ = 0;

    //!是否已打开
    bool is_open_ = false;
    //!是否有效
    bool is_valid_ = false;
    //!是否是一个新建xls文件，用来区分打开的excel是已存在文件还是有本类新建的
    bool is_a_newfile_ = false;
    //!防止重复保存
    bool is_save_already_ = false;

	//预加载的表的数据
	QVariantList pre_load_data_;

};

#endif // QT_EXCELENGINE_H
