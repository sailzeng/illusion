
#include "QtAxExcelEngine.h"


QtAxExcelEngine::QtAxExcelEngine()
{
    excel_instance_     = NULL;
    work_books_ = NULL;
    active_book_  = NULL;
    active_sheet_ = NULL;
}

QtAxExcelEngine::~QtAxExcelEngine()
{
    if ( is_open_ )
    {
        //析构前，先保存数据，然后关闭workbook
        close();
    }
    finalize();
}


//初始化EXCEL OLE对象，打开EXCEL 进程，
bool QtAxExcelEngine::initialize(bool visible)
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


//注销
void QtAxExcelEngine::finalize()
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

		row_count_ = 0;
		column_count_ = 0;
		start_row_ = 1;
		start_column_ = 1;

		xls_file_ = "";
    }

    ::CoUninitialize();
}


//打开EXCEL文件
bool QtAxExcelEngine::open(const QString &xls_file)
{
    xls_file_ = xls_file;
    
    if (is_open_)
    {
        close();
    }

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
        //打开xls对应的，获取工作簿,注意，这儿用的不是xls_file_，要用绝对路径
        active_book_ = work_books_->querySubObject("Open(const QString&,QVariant)", 
												   fi.absoluteFilePath(),
												   0);
    }
    else
    {
        //新建一个xls，添加一个新的工作薄
        work_books_->dynamicCall("Add()");
        active_book_ = excel_instance_->querySubObject("ActiveWorkBook");
    }

	if (!active_book_)
	{
		return false;
	}

    is_open_ = true;
    return is_open_;
}

/**
  *@brief 保存表格数据，把数据写入文件
  */
void QtAxExcelEngine::save()
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
void QtAxExcelEngine::close()
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
int QtAxExcelEngine::sheetsCount()
{
    return work_books_->property("Count").toInt();
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
								bool pre_load)
{
    active_sheet_ = active_book_->querySubObject("WorkSheets(int)", sheet_index);

    //如果没有打开，
    if (!active_sheet_)
    {
        return false;
    }
    loadSheet_internal(pre_load);
    return true;
}



//按照序号加载Sheet表格,
bool QtAxExcelEngine::loadSheet(const QString &sheet_name,
								bool pre_load)
{
    active_sheet_ = active_book_->querySubObject("WorkSheets(QString)", sheet_name);
    //如果没有打开，
    if (!active_sheet_)
    {
        return false;
    }
    loadSheet_internal(pre_load);
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

void QtAxExcelEngine::loadSheet_internal(bool pre_load)
{
    //获取该sheet的使用范围对象
    QAxObject *used_range = active_sheet_->querySubObject("UsedRange");
    QAxObject *rows = used_range->querySubObject("Rows");
    QAxObject *columns = used_range->querySubObject("Columns");

    //因为excel可以从任意行列填数据而不一定是从1,1开始，因此要获取首行列下标
    //第一行的起始位置
    start_row_ = used_range->property("Row").toInt();
    //第一列的起始位置
    start_column_ = used_range->property("Column").toInt();
    
	//获取行数，便于理解，也算上了空行，否则各种地方坐标理解还不一致。
    row_count_ = rows->property("Count").toInt() + start_row_ -1;
    //获取列数
    column_count_ = columns->property("Count").toInt() + start_column_ -1;

	preload_sheet_data_.clear();
	preload_sheet_data_.reserve(row_count_ * column_count_);
	//预加载的数据，存放
	if (pre_load)
	{
		QVariantList row_list = used_range->property("Value2").toList();
		//第一次转换得到的行数据，需要再取一次.实际测试，如果只有1,1一个数据，也不会进入预加载代码
		for (int i = 0; i < row_list.size(); ++i)
		{
			preload_sheet_data_ += row_list.at(i).toList();
		}
	}
	
	delete rows;
	delete columns;
	delete used_range;
    return;
}


//打开的xls文件名称
QString QtAxExcelEngine::openFilename() const
{
    return xls_file_;
}

//改名
void QtAxExcelEngine::renameSheet(const QString & new_name)
{
	active_sheet_->setProperty("Name", new_name);
}


//把tableWidget中的数据保存到excel中
bool QtAxExcelEngine::writeTableWidget(QTableWidget *tableWidget)
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
            this->setCell(1, i + 1, tableWidget->horizontalHeaderItem(i)->text());
        }
    }

    //写数据
    for (int i = 0; i < tableR; i++)
    {
        for (int j = 0; j < tableC; j++)
        {
            if ( tableWidget->item(i, j) != NULL )
            {
                this->setCell(i + 2, j + 1, tableWidget->item(i, j)->text());
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
bool QtAxExcelEngine::readTableWidget(QTableWidget *tableWidget)
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

    //获取excel中的第一行数据作为表头
    QStringList headerList;
    for (int n = start_column_; n <= column_count_; n++ )
    {
        QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", start_row_, n);
        if ( cell )
        {
            headerList << cell->dynamicCall("Value2()").toString();
        }
    }

    //重新创建表头
    tableWidget->setColumnCount(column_count_ - start_column_ +1);
    tableWidget->setHorizontalHeaderLabels(headerList);


    //插入新数据
	//行
    for (int i = start_row_ + 1, r = 0; i <= row_count_; i++, r++ )   
    {
		//插入新行
        tableWidget->insertRow(r); 
		//列
        for (int j = start_column_, c = 0; j <= column_count_; j++, c++ )   
        {
            QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", i, j ); //获取单元格

            //在r新行中添加子项数据
            if ( cell )
            {
                tableWidget->setItem(r, c, new QTableWidgetItem(cell->property("Value2").toString()));
            }
        }
    }

    return true;
}

//得到某个cell的数据
QVariant QtAxExcelEngine::getCell(int row, int column)
{
	//如果预加载了数据,
	if (preload_sheet_data_.size() > 0)
	{
		//超出范围返回空数据
		if (row > row_count_ || column > column_count_ || row < start_row_ 	|| column < start_column_)
		{
			return QVariant();
		}
		else
		{
			return preload_sheet_data_.at((row - start_row_)*(column_count_ - start_column_+1)
										  + column - start_column_);
		}
	}
	//如果没有预加载数据
	else
	{
		//获取单元格对象
		QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", row, column);
		if (cell)
		{
			return cell->property("Value2");
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
	bool op = false;
	//获取单元格对象
	QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)",
													row,
													column);
	//excel 居然只能插入字符串和整型，浮点型无法插入
	if (cell)
	{
		QString strData = data.toString();
		cell->dynamicCall("SetValue(const QVariant&)", strData); //修改单元格的数据
		op = true;
	}
	else
	{
		op = false;
	}

	return op;
}



//判断excel是否已被打开
bool QtAxExcelEngine::is_open()
{
    return is_open_;
}

/**
  *@brief 判断excel COM对象是否调用成功，excel是否可用
  *@return true : 可用
  *        false: 不可用
  */
bool QtAxExcelEngine::is_valid()
{
    return is_valid_;
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

//
void QtAxExcelEngine::insertSheet(const QString &sheet_name)
{
	work_books_->querySubObject("Add()");
    QAxObject *a = work_books_->querySubObject("Item(int)", 1);
    a->setProperty("Name", sheet_name);
    active_sheet_ = a;

    loadSheet_internal(false);
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