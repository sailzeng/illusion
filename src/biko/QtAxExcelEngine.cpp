
#include "biko_predefine.h"
#include "QtAxExcelEngine.h"


QtAxExcelEngine::QtAxExcelEngine()
{
}

QtAxExcelEngine::~QtAxExcelEngine()
{
    finalize();
}


//初始化EXCEL OLE对象，打开EXCEL 进程，
bool QtAxExcelEngine::initialize(bool visible)
{

    HRESULT r = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (r == S_OK )
    {
        com_init_byself_ = true;
    }
    else
    {
        fprintf(stderr, "QtActiveX: Could not initialize OLE CoInitializeEx (ret %x error %x) .\n",
                (unsigned int)r,
                ::GetLastError());
        if (r == S_FALSE || r == RPC_E_CHANGED_MODE)
        {
            //如果是这个错误，放弃，没啥忍了
        }
    }
    is_visible_ = visible;
    //
    if (NULL == excel_instance_)
    {
        //OLE启动EXCEL 很容易被其他东西破坏，实在没有法子。比如你安装了WPS,
        excel_instance_ = new QAxObject("Excel.Application");
        if (excel_instance_->isNull())
        {
            fprintf(stderr, "QtActiveX: new OLE Excel.Application fail, ensure your computer install EXCEL.\n"
                   );
            is_open_ = false;
            return is_open_;
        }
        //在部分机器的DEBUG版本发现有断言错误，参数数量不一致，原因未知，
        excel_instance_->dynamicCall("SetVisible(bool )", is_visible_);
    }
    return true;
}


//注销
void QtAxExcelEngine::finalize()
{
    if (excel_instance_ )
    {
        if (excel_instance_->isNull() == false)
        {
            if (is_open_)
            {
                close();
            }
            excel_instance_->dynamicCall("Quit()");
        }

        delete excel_instance_;
        excel_instance_ = NULL;

        is_open_ = false;
        is_newfile_ = false;

        row_count_ = 0;
        column_count_ = 0;
        start_row_ = 1;
        start_column_ = 1;

        xls_file_.clear();
    }
    //不是自己初始化的，步销毁，否则有问题
    if (com_init_byself_)
    {
        ::CoUninitialize();
    }

}

//打开EXCEL文件
bool QtAxExcelEngine::open(const QString &xls_file,
                           bool not_exist_new)
{
    bool new_file = false;
    //如果指向的文件不存在，则需要新建一个
    QFileInfo fi(xls_file);
    if ( false == fi.exists())
    {
        if (false == not_exist_new)
        {
            return false;
        }
        new_file = true;
    }

    //得到绝对路径，因为ActiveX只支持绝对路径，包括Open，包括Saveas,不光必须用绝对路径，还需要实用
    // 原生的路径分割符号 '\'
    xls_file_ = QDir::toNativeSeparators(fi.absoluteFilePath());

    return opennew_internal(new_file);
}

//新建一个XLS文件
bool QtAxExcelEngine::newOne()
{
    return opennew_internal(true);
}

//!打开（新建）文件的内部具体实现
bool QtAxExcelEngine::opennew_internal(bool new_file)
{
    if (excel_instance_ == NULL || excel_instance_->isNull())
    {
        return false;
    }

    if (is_open_)
    {
        close();
    }

    work_books_ = excel_instance_->querySubObject("WorkBooks");
    if (!new_file)
    {
        //这儿原来的代码是下面这样的，但是在我的两台机器都出现了返回为NULL的问题，而且会出现提示错误
        //其中一台，我反复折腾后正常，但另外一台死活不对，
        //QAxBase: error calling idispatch member open: unknown error
        //出错代码如下，
        //active_book_ = excel_instance_->querySubObject("Open(const QString&)",xls_file_);
        //但其实，应该问题不大,或者说带病可以运行下去，我们把代码改成如下，规避了这个问题，但仍然有提示

        //打开xls对应的，获取工作簿,注意，要用绝对路径
        work_books_->dynamicCall("Open(const QString&)",
                                 xls_file_);
        active_book_ = excel_instance_->querySubObject("ActiveWorkBook");
    }
    else
    {
        //新建一个xls，添加一个新的工作薄
        work_books_->dynamicCall("Add()");
        active_book_ = excel_instance_->querySubObject("ActiveWorkBook");
        is_newfile_ = true;
    }

    if (!active_book_)
    {
        return false;
    }
    work_sheets_ = active_book_->querySubObject("Worksheets");
    is_open_ = true;
    return true;
}

//保存表格数据，把数据写入文件
void QtAxExcelEngine::save()
{
    if (!active_book_)
    {
        return;
    }
    if (xls_file_.isEmpty())
    {
        return;
    }
    //如果已经保存
    if (isSaved())
    {
        return;
    }
    if (is_newfile_)
    {
        active_book_->dynamicCall("SaveAs(const QString&)", xls_file_);
        is_newfile_ = false;
    }
    else
    {
        active_book_->dynamicCall("Save()");
    }
}

//另存为
void QtAxExcelEngine::saveAs(const QString &xls_file)
{
    QFileInfo fi(xls_file);

    //得到绝对路径，因为ActiveX只支持绝对路径，包括Open，包括Saveas,不光必须用绝对路径，还需要实用
    // 原生的路径分割符号 '\'
    xls_file_ = QDir::toNativeSeparators(fi.absoluteFilePath());
    /*如果该文档是新建出来的，则使用另存为COM接口*/
    active_book_->dynamicCall("SaveAs(const QString&)", xls_file_);
    is_newfile_ = false;
}

//关闭打开的EXCEL,
void QtAxExcelEngine::close()
{
    //关闭前先保存数据
    save();
    if ( excel_instance_ && active_book_ )
    {
        active_book_->dynamicCall("Close(bool)", true);

        is_open_     = false;
        xls_file_.clear();
    }
}

//
int QtAxExcelEngine::sheetsCount()
{
    return work_sheets_->property("Count").toInt();
}

//得到某个sheet的名字
bool QtAxExcelEngine::sheetName(int sheet_index, QString &sheet_name)
{
    QAxObject *sheet_tmp = active_book_->querySubObject("WorkSheets(int)", sheet_index);
    if (!sheet_tmp)
    {
        return false;
    }
    sheet_name = sheet_tmp->property("Name").toString();
    return true;
}

bool QtAxExcelEngine::loadSheet(int sheet_index,
                                bool pre_read)
{
    active_sheet_ = active_book_->querySubObject("WorkSheets(int)", sheet_index);

    //如果没有打开，
    if (!active_sheet_)
    {
        return false;
    }
    loadSheet_internal(pre_read);
    return true;
}



//按照sheet名字加载Sheet表格,
bool QtAxExcelEngine::loadSheet(const QString &sheet_name,
                                bool pre_read)
{
    active_sheet_ = active_book_->querySubObject("WorkSheets(QString)", sheet_name);
    //如果没有打开，
    if (!active_sheet_)
    {
        return false;
    }
    loadSheet_internal(pre_read);
    return true;
}

//
bool QtAxExcelEngine::hasSheet(const QString &sheet_name)
{
    QAxObject *temp_sheet = active_book_->querySubObject("WorkSheets(QString)",
                                                         sheet_name);
    if (!temp_sheet)
    {
        return false;
    }
    return true;
}

void QtAxExcelEngine::loadSheet_internal(bool pre_read)
{
    //获取该sheet的使用范围对象
    QAxObject *used_range = active_sheet_->querySubObject("UsedRange");
    if (used_range)
    {
        QAxObject *rows = used_range->querySubObject("Rows");
        QAxObject *columns = used_range->querySubObject("Columns");

        //因为excel可以从任意行列填数据而不一定是从1,1开始，因此要获取首行列下标
        //第一行的起始位置
        start_row_ = used_range->property("Row").toInt();
        //第一列的起始位置
        start_column_ = used_range->property("Column").toInt();

        //获取行数，便于理解，也算上了空行，否则各种地方坐标理解还不一致。
        //rows->property("Count").toInt()返回的是真实使用了的行数
        row_count_ = rows->property("Count").toInt() + start_row_ - 1;
        //获取列数
        column_count_ = columns->property("Count").toInt() + start_column_ - 1;

        preload_sheet_data_.clear();
        preload_sheet_data_.reserve(row_count_ * column_count_);
        //预加载的数据，存放
        if (pre_read)
        {
            //Value2 and Value 的区别是
            QVariantList row_list = used_range->property("Value").toList();
            //第一次转换得到的行数据，需要再取一次.实际测试，如果只有1,1一个数据，也不会进入预加载代码
            for (int i = 0; i < row_list.size(); ++i)
            {
                preload_sheet_data_ += row_list.at(i).toList();
            }
        }

        delete rows;
        delete columns;
        delete used_range;
    }
    return;
}

//插入一个SHEET
void QtAxExcelEngine::insertSheet(const QString &sheet_name)
{
    active_sheet_ = work_sheets_->querySubObject("Add()");
    if (!active_sheet_)
    {
        return;
    }
    active_sheet_->setProperty("Name", sheet_name);

    row_count_ = 0;
    column_count_ = 0;
    start_row_ = 1;
    start_column_ = 1;
}

//打开的xls文件名称
QString QtAxExcelEngine::openFilename() const
{
    return xls_file_;
}

//改名
void QtAxExcelEngine::renameSheet(const QString &new_name)
{
    active_sheet_->setProperty("Name", new_name);
}


//把tableWidget中的数据保存到excel中
bool QtAxExcelEngine::writeTableWidget(QTableWidget *table_widget)
{
    if (NULL == table_widget)
    {
        return false;
    }
    if (!is_open_)
    {
        return false;
    }
    int table_row = table_widget->rowCount();
    int table_column = table_widget->columnCount();

    //写把数据读取到data_table里面
    QVariantList data_table;
    QVariantList header_list;
    data_table.reserve(table_row + 1);
    header_list.reserve(table_column);
    //先读取标题栏
    for (int i = 0; i < table_column; i++)
    {
        if (table_widget->horizontalHeaderItem(i) != NULL)
        {
            header_list.push_back(table_widget->horizontalHeaderItem(i)->text()) ;
        }
        else
        {
            header_list.push_back(QVariant());
        }
    }
    data_table.push_back(header_list);
    //再读取数据区
    for (int i = 0; i < table_row; i++)
    {
        QVariantList data_list;
        data_list.reserve(table_column);
        for (int j = 0; j < table_column; j++)
        {
            if (table_widget->item(i, j) != NULL)
            {
                data_list.push_back(table_widget->item(i, j)->text());
            }
            else
            {
                data_list.push_back(QVariant());
            }
        }
        data_table.push_back(data_list);
    }

    //调试代码
    //qDebug("table has row=%d colum=%d", data_table.size(), header_list.size());
    //for (int i = 0; i < data_table.size(); i++)
    //{
    //    QVariantList data_list = data_table.at(i).toList();
    //    for (int j = 0; j < data_list.size(); j++)
    //    {
    //        qDebug("table has row colum[%d,%d] data %s", i,j,data_list.at(j).toString().toStdString().c_str());
    //    }
    //}

    //写入EXCEL
    setRangeCell(1, 1, table_row + 1, table_column , data_table);
    return true;
}

/**
  *@brief 从指定的xls文件中把数据导入到tableWidget中
  *@param tableWidget : 执行要导入到的tablewidget指针
  *@return 导入成功与否 true : 成功
  *                   false: 失败
  */
bool QtAxExcelEngine::readTableWidget(QTableWidget *table_widget)
{
    if ( NULL == table_widget )
    {
        return false;
    }

    //先把table的内容清空
    int tableColumn = table_widget->columnCount();
    table_widget->clear();
    for (int n = 0; n < tableColumn; n++)
    {
        table_widget->removeColumn(0);
    }

    //获取excel中的第一行数据作为表头
    QStringList headerList;
    for (int n = start_column_; n <= column_count_; n++ )
    {
        headerList << getCell(start_row_, n).toString();
    }

    //重新创建表头
    table_widget->setColumnCount(column_count_ - start_column_ + 1);
    table_widget->setHorizontalHeaderLabels(headerList);

    //插入新数据
    //行
    for (int i = start_row_ + 1, r = 0; i <= row_count_; i++, r++ )
    {
        //插入新行
        table_widget->insertRow(r);
        //列
        for (int j = start_column_, c = 0; j <= column_count_; j++, c++ )
        {
            table_widget->setItem(r, c, new QTableWidgetItem(getCell(i, j).toString()));
        }
    }

    return true;
}

//Dim rng As Range : Set rng = Application.Range("$A$1")
//Debug.Print rng.Value             0.1429
//Debug.Print rng.Value2            0.142857142857143
//Debug.Print rng.Text              $0.14


//得到某个cell的数据
QVariant QtAxExcelEngine::getCell(int row, int column)
{
    //如果预加载了数据,
    if (preload_sheet_data_.size() > 0)
    {
        //超出范围返回空数据
        if (row > row_count_ || column > column_count_ || row < start_row_  || column < start_column_)
        {
            return QVariant();
        }
        else
        {
            return preload_sheet_data_.at((row - start_row_) * (column_count_ - start_column_ + 1)
                                          + column - start_column_);
        }
    }
    //如果没有预加载数据
    else
    {
        //获取单元格对象
        QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", row, column);
        std::unique_ptr<QAxObject> del_it(cell);
        if (cell)
        {
            return cell->property("Value");
        }
        else
        {
            return QVariant();
        }
    }
}


//修改指定单元格的数据
bool QtAxExcelEngine::setCell(int row, int column, const QVariant &data)
{

    //获取单元格对象
    QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)",
                                                    row,
                                                    column);
    if (!cell)
    {
        return false;
    }

    //修改单元格的数据
    QString strData = data.toString();
    cell->dynamicCall("SetValue(const QVariant&)", strData);
    return true;
}

//判断excel是否已被打开
bool QtAxExcelEngine::isOpen()
{
    return is_open_;
}

bool QtAxExcelEngine::isSaved()
{
    return active_book_->property("Saved").toBool();
}

//获取excel的行数,包括空行
int QtAxExcelEngine::rowCount()const
{
    return row_count_ ;
}


//获取excel的列数,包括空行
int QtAxExcelEngine::columnCount()const
{
    return column_count_;
}

//!当前的Sheet的起始行数，如果第1,2行是空，没有数据，那么返回3
int QtAxExcelEngine::startRow() const
{
    return start_row_;
}

//!当前的Sheet的起始列数，如果第1,2,3列是空，没有数据，那么返回4
int QtAxExcelEngine::startColumn() const
{
    return start_column_;
}


//取得列的名称，比如27->AA
QString QtAxExcelEngine::columnName(int column_no)
{
    char column_name[64];
    size_t str_len = 0;

    while (column_no > 0)
    {
        int num_data = column_no % 26;
        column_no /= 26;
        if (num_data == 0)
        {
            num_data = 26;
            column_no--;
        }
        //不知道这个对不，
        column_name[str_len] = (char)((num_data - 1) + ('A'));
        str_len++;
    }
    column_name[str_len] = '\0';
    //反转
    strrev(column_name);

    return column_name;
}

//取得cell的名称。比如第一个cell是A1
QString QtAxExcelEngine::cellsName(int row_no, int column_no)
{
    return QString("%1%2")
           .arg(QtAxExcelEngine::columnName(column_no))
           .arg(row_no);
}

//从一个RANGE中读取sheet的数据，QVariantList是一个table，也就是List套List
void QtAxExcelEngine::getRangecell(int cell1_row,
                                   int cell1_column,
                                   int cell2_row,
                                   int cell2_column,
                                   QVariantList &data_table)
{
    data_table.clear();
    QAxObject *range = active_sheet_->querySubObject("Range(const QString&, const QString&)",
                                                     QtAxExcelEngine::cellsName(cell1_row, cell1_column),
                                                     QtAxExcelEngine::cellsName(cell2_row, cell2_column));

    if (!range)
    {
        return;
    }
    data_table.reserve((cell2_row - cell1_row + 1));
    data_table = range->property("Value").toList();
}

//向一个RANGE中写入sheet的数据，QVariantList是一个table，也就是List套List
bool QtAxExcelEngine::setRangeCell(int cell1_row,
                                   int cell1_column,
                                   int cell2_row,
                                   int cell2_column,
                                   const QVariantList &data_table)
{
    qDebug("Cell1 name %s Cell2 name %s ,data size %d",
           QtAxExcelEngine::cellsName(cell1_row, cell1_column).toStdString().c_str(),
           QtAxExcelEngine::cellsName(cell2_row, cell2_column).toStdString().c_str(),
           data_table.size());
    QAxObject *range = active_sheet_->querySubObject("Range(const QString&, const QString&)",
                                                     QtAxExcelEngine::cellsName(cell1_row, cell1_column),
                                                     QtAxExcelEngine::cellsName(cell2_row, cell2_column));
    if (!range)
    {
        return false;
    }
    range->dynamicCall("SetValue(const QVariant&)", QVariant(data_table));
    return false;
}