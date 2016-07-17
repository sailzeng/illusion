#include "biko_predefine.h"
#include "biko_qt_excel_engine.h"


BikoQtExcelEngine::BikoQtExcelEngine()
{
    excel_instance_     = NULL;
    work_books_ = NULL;
    active_book_  = NULL;
    active_sheet_ = NULL;

    xls_file_     = "";

    curr_sheet_ = 1;
    row_count_ = 0;
    column_count_ = 0;
    start_row_    = 0;
    start_column_ = 0;

    is_open_     = false;
    is_valid_    = false;
    is_a_newfile_ = false;
    is_save_already_ = false;

}

BikoQtExcelEngine::~BikoQtExcelEngine()
{
    if ( is_open_ )
    {
        //析构前，先保存数据，然后关闭workbook
        close();
    }
    finalize();
}


//初始化EXCEL OLE对象，打开EXCEL 进程，
bool BikoQtExcelEngine::initialize(bool visible)
{

    HRESULT r = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (r != S_OK && r != S_FALSE)
    {
        qDebug("Qt: Could not initialize OLE (error %x)", (unsigned int)r);
        return false;
    }
    is_visible_ = visible;
    //
    if (NULL == excel_instance_)
    {
        excel_instance_ = new QAxObject("Excel.Application");
        if (excel_instance_->isNull())
        {
            is_valid_ = false;
            is_open_ = false;
            return is_open_;
        }
        else
        {
            is_valid_ = true;
        }

        excel_instance_->dynamicCall("SetVisible(bool)", is_visible_);
    }
    return true;
}


//
void BikoQtExcelEngine::finalize()
{
    if (excel_instance_ )
    {

        excel_instance_->dynamicCall("Quit()");

        delete excel_instance_;
        excel_instance_ = NULL;

        is_open_ = false;
        is_valid_ = false;
        is_a_newfile_ = false;
        is_save_already_ = true;
    }

    ::CoUninitialize();
}


//打开EXCEL文件
bool BikoQtExcelEngine::open(const QString &xls_file, int  sheet_index)
{
    xls_file_ = xls_file;
    curr_sheet_ = sheet_index;

    if (is_open_)
    {
        close();
    }

    curr_sheet_ = sheet_index;

    if (!is_valid_)
    {
        is_open_ = false;
        return is_open_;
    }

    //如果指向的文件不存在，则需要新建一个
    QFileInfo fi(xls_file_);
    if (!fi.exists())
    {
        is_a_newfile_ = true;
    }
    else
    {
        is_a_newfile_ = false;
    }

    work_books_ = excel_instance_->querySubObject("WorkBooks");
    if (!is_a_newfile_)
    {
        //打开xls对应的，获取工作簿
        active_book_ = work_books_->querySubObject("Open(const QString&)", xls_file_);
    }
    else
    {
        //新建一个xls，添加一个新的工作薄
        work_books_->dynamicCall("Add()");
        active_book_ = excel_instance_->querySubObject("ActiveWorkBook");
    }

    work_sheets_ = active_book_->querySubObject("WorkSheets");

    //至此已打开
    loadSheet(curr_sheet_);

    is_open_ = true;
    return is_open_;
}

/**
  *@brief 保存表格数据，把数据写入文件
  */
void BikoQtExcelEngine::save()
{
    if ( active_book_ )
    {
        if (is_save_already_)
        {
            return ;
        }

        if (!is_a_newfile_)
        {
            active_book_->dynamicCall("Save()");
        }
        else     /*如果该文档是新建出来的，则使用另存为COM接口*/
        {
            active_book_->dynamicCall("SaveAs (const QString&,int,const QString&,const QString&,bool,bool)",
                                      xls_file_, 56, QString(""), QString(""), false, false);

        }

        is_save_already_ = true;
    }
}

/**
  *@brief 关闭前先保存数据，然后关闭当前Excel COM对象，并释放内存
  */
void BikoQtExcelEngine::close()
{
    //关闭前先保存数据
    save();
    if ( excel_instance_ && active_book_ )
    {
        active_book_->dynamicCall("Close(bool)", true);

        is_open_     = false;
        is_valid_    = false;
        is_a_newfile_ = false;
        is_save_already_ = true;
    }
}

//
int BikoQtExcelEngine::sheetsCount()
{
    return work_books_->property("Count").toInt();
}

//得到某个sheet的名字
bool BikoQtExcelEngine::sheetName(int sheet_index, QString &sheet_name)
{
    QAxObject *sheet_tmp = active_book_->querySubObject("WorkSheets(int)", sheet_index);
    if (!sheet_tmp)
    {
        return false;
    }
    sheet_name = sheet_tmp->property("Name").toString();
    return true;
}

bool BikoQtExcelEngine::loadSheet(int sheet_index)
{
    active_sheet_ = active_book_->querySubObject("WorkSheets(int)", sheet_index);

    //如果没有打开，
    if (!active_sheet_)
    {
        return false;
    }
    load_sheet_internal();
    return true;
}



//按照序号加载Sheet表格,
bool BikoQtExcelEngine::loadSheet(const QString &sheet_name)
{
    active_sheet_ = active_book_->querySubObject("WorkSheets(QString)", sheet_name);
    //如果没有打开，
    if (!active_sheet_)
    {
        return false;
    }
    load_sheet_internal();
    return true;
}

bool BikoQtExcelEngine::hasSheet(const QString &sheet_name)
{
    QAxObject *temp_sheet = active_book_->querySubObject("WorkSheets(QString)", sheet_name);
    if (!temp_sheet)
    {
        return false;
    }
    return true;
}

void BikoQtExcelEngine::load_sheet_internal(bool pre_load)
{
    //获取该sheet的使用范围对象
    QAxObject *used_range = active_sheet_->querySubObject("UsedRange");
    QAxObject *rows = used_range->querySubObject("Rows");
    QAxObject *columns = used_range->querySubObject("Columns");

    //因为excel可以从任意行列填数据而不一定是从0,0开始，因此要获取首行列下标
    //第一行的起始位置
    start_row_ = used_range->property("Row").toInt();
    //第一列的起始位置
    start_column_ = used_range->property("Column").toInt();
    //获取行数
    row_count_ = rows->property("Count").toInt();
    //获取列数
    column_count_ = columns->property("Count").toInt();

	//预加载的数据，存放	
	if (pre_load)
	{
		pre_load_data_ = used_range->dynamicCall("Value2()").toList();
	}
	delete used_range;
	delete rows;
    return;
}


//!打开的xls文件名称
QString BikoQtExcelEngine::open_filename() const
{
    return xls_file_;
}

/**
  *@brief 把tableWidget中的数据保存到excel中
  *@param tableWidget : 指向GUI中的tablewidget指针
  *@return 保存成功与否 true : 成功
  *                  false: 失败
  */
bool BikoQtExcelEngine::writeTableWidget(QTableWidget *tableWidget)
{
    if ( NULL == tableWidget )
    {
        return false;
    }
    if ( !is_open_ )
    {
        return false;
    }

    int tableR = tableWidget->rowCount();
    int tableC = tableWidget->columnCount();

    //获取表头写做第一行
    for (int i = 0; i < tableC; i++)
    {
        if ( tableWidget->horizontalHeaderItem(i) != NULL )
        {
            this->set_cell(1, i + 1, tableWidget->horizontalHeaderItem(i)->text());
        }
    }

    //写数据
    for (int i = 0; i < tableR; i++)
    {
        for (int j = 0; j < tableC; j++)
        {
            if ( tableWidget->item(i, j) != NULL )
            {
                this->set_cell(i + 2, j + 1, tableWidget->item(i, j)->text());
            }
        }
    }

    //保存
    save();

    return true;
}

/**
  *@brief 从指定的xls文件中把数据导入到tableWidget中
  *@param tableWidget : 执行要导入到的tablewidget指针
  *@return 导入成功与否 true : 成功
  *                   false: 失败
  */
bool BikoQtExcelEngine::readTableWidget(QTableWidget *tableWidget)
{
    if ( NULL == tableWidget )
    {
        return false;
    }

    //先把table的内容清空
    int tableColumn = tableWidget->columnCount();
    tableWidget->clear();
    for (int n = 0; n < tableColumn; n++)
    {
        tableWidget->removeColumn(0);
    }

    int rowcnt    = start_row_ + row_count_;
    int columncnt = start_column_ + column_count_;

    //获取excel中的第一行数据作为表头
    QStringList headerList;
    for (int n = start_column_; n < columncnt; n++ )
    {
        QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", start_row_, n);
        if ( cell )
        {
            headerList << cell->dynamicCall("Value2()").toString();
        }
    }

    //重新创建表头
    tableWidget->setColumnCount(column_count_);
    tableWidget->setHorizontalHeaderLabels(headerList);


    //插入新数据
    for (int i = start_row_ + 1, r = 0; i < rowcnt; i++, r++ )   //行
    {
        tableWidget->insertRow(r); //插入新行
        for (int j = start_column_, c = 0; j < columncnt; j++, c++ )   //列
        {
            QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", i, j ); //获取单元格

            //在r新行中添加子项数据
            if ( cell )
            {
                tableWidget->setItem(r, c, new QTableWidgetItem(cell->dynamicCall("Value2()").toString()));
            }
        }
    }

    return true;
}


QVariant BikoQtExcelEngine::get_cell(int row, int column)
{
    QVariant data;

    QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", row, column); //获取单元格对象
    if ( cell )
    {
        data = cell->dynamicCall("Value2()");
    }

    return data;
}

/**
  *@brief 修改指定单元格的数据
  *@param row : 单元格的行号
  *@param column : 单元格指定的列号
  *@param data : 单元格要修改为的新数据
  *@return 修改是否成功 true : 成功
  *                   false: 失败
  */
bool BikoQtExcelEngine::set_cell(int row, int column, const QVariant &data)
{
    bool op = false;

    QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", row, column); //获取单元格对象
    if ( cell )
    {
        QString strData = data.toString(); //excel 居然只能插入字符串和整型，浮点型无法插入
        cell->dynamicCall("SetValue(const QVariant&)", strData); //修改单元格的数据
        op = true;
    }
    else
    {
        op = false;
    }

    return op;
}

/**
  *@brief 清空除报表之外的数据
  */
void BikoQtExcelEngine::clear()
{
    xls_file_     = "";
    row_count_    = 0;
    column_count_ = 0;
    start_row_    = 0;
    start_column_ = 0;
}

/**
  *@brief 判断excel是否已被打开
  *@return true : 已打开
  *        false: 未打开
  */
bool BikoQtExcelEngine::is_open()
{
    return is_open_;
}

/**
  *@brief 判断excel COM对象是否调用成功，excel是否可用
  *@return true : 可用
  *        false: 不可用
  */
bool BikoQtExcelEngine::is_valid()
{
    return is_valid_;
}

/**
  *@brief 获取excel的行数
  */
int BikoQtExcelEngine::row_count()const
{
    return row_count_;
}

/**
  *@brief 获取excel的列数
  */
int BikoQtExcelEngine::column_count()const
{
    return column_count_;
}

//
void BikoQtExcelEngine::insertSheet(const QString &sheet_name)
{
    work_sheets_->querySubObject("Add()");
    QAxObject *a = work_sheets_->querySubObject("Item(int)", 1);
    a->setProperty("Name", sheet_name);
    active_sheet_ = a;

    load_sheet_internal();
}


//取得列的名称，比如27->AA
char *BikoQtExcelEngine::column_name(int column_no)
{
    static char column_name[64];
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