# ILLUSION

**illusion** 是一个使用 [Protobuf](https://developers.google.com/protocol-buffers/)（Google  Protocol Buffers 2.0）文件作为配置描述，然后读取 Excel 文件里面相关的配置，然后将配置信息生产配置数据文件的工具。

**illusion** 主要用于游戏配置管理等。程序开发可以快速的在PB文件里面定义配置结构信息，以及配置结构和 Excel 表直接的关系（生成 Excel 配置模版），策划可以方便的直接使用 Excel 进行数据配置。

希望通过这个工具提高游戏开发团队的开发效率。

## 工具简介

**illusion** 的图形化工具 **vision** 采用典型的左树右图 UI 界面。编译运行 `bin` 目录下的 `vision-release.exe` 程序，打开图形化工具。左边是配置表集合的导航树。右边是配置文件打开预览区，相应目录的 PB(.proto) 文件，Excel(.xls,.xlsx) 文件都可以在这个区域形成预览。方便您迅速的了解相关配置信息。工具界面如下图：

![](http://i1.piimg.com/588926/f0486520c653ec7e.png)

使用工具初始化有 2 种方式：

1. 一种是分别配置不同的子目录，**illusion** 有 4 种数据，Protobuf `.proto` 文件放置的目录；Excel `.xls,.xlsx` 文件放置的目录；Protobuf import 文件放置的目录（**illusion** 使用，所以也有自己的 import 目录），熟悉 Protobuf 的同学应该知道用途，以及生产文件 `.bin` 文件放置的目录。配置的时候选择 Toolbar 工具条上的第二个按钮。打开界面如下：

  ![](http://i2.buimg.com/588926/0a907b1d32e16eb5.png)
  
2. 一种是 All In One 的模式，要求所有的配置目录放到一个子目录下，只配置打开一个目录。这种情况要求这个目录下分别有 4 个子目录放置不同的数据，分别是 `/import`, `/proto`, `/outer`, `/excel` 目录，（就是上面分别配置 4 种目录）。配置的时候选择 Toolbar 工具条上的第二个按钮：

  ![](http://i1.piimg.com/588926/1addce2493ee43ed.png)

配置好目录后，**illusion** 会分析 proto 文件目录，得到对应信息，同时检查相应的 Excel 文件和最后生成的配置文件是否存在，那个更新。然后会根据这些信息生产配置导航树。然后就可以在导航树上选择需要导出的配置表了。导航树默认的 Sheet 导航是用 Excel 的 Sheet 表名称作为树的根节点的。
只有相应有新的 Excel 配置的表格，程序会自动勾选。

## 使用流程说明

### 1. 程序开发定义PB文件

使用 **illusion** 进行配置，你需要先定义一个 proto 文件，其使用 [google protobuf Options](https://developers.google.com/protocol-buffers/docs/proto#options) 的扩展功能，来描述你的配置结构和 Excel 表格（Sheet）之间的对应关系。所有的PB描述文件都需要包含 **illusion.proto** 文件，这个文件对相应的字段域，Message，枚举，做了一些扩展选项。这些选项会辅助我们建立我们自己的PB描述和EXCEL配置表格之间的关系。**illusion.proto** 文件很简单，如下：

```
//illusion.proto会引入google/protobuf/descriptor.proto文件
import "google/protobuf/descriptor.proto";

package illusion;

//MessageOptions的扩展，
extend google.protobuf.MessageOptions 
{
    //这个message对应的list message名称
    optional bool  cfg_message = 50001 [default = false];

	//配置注释，中文名称
	optional string cfg_comment = 50002;

    //读取的EXCEL配置文件名称
    optional string excel_file = 50003;
    //读取的表格SHEET名称
    optional string excel_sheet = 50004;
    //输出的proto配置文件名称
    optional string outer_file = 50005;
    
    //字段名称写在第几行，导成EXCEL时实用
    optional int32  fieldsname_line = 50006[default = 1];
    //字段的FULLNAME
    optional int32  fullname_line = 50007 [default = 2];
    //读取的行
    optional int32  read_line = 50008 [default = 3];
  
}

//对应字段域的扩展选项
extend google.protobuf.FieldOptions 
{
    //这个message对应的list message名称
    optional bool  cfg_field = 51001 [default = true];
    //域选项的名字，
    optional string fields_name = 51002;
    //域选项，如果是一个repeat的字段，其长度
    optional int32 repeat_size = 51003;
}

//对应枚举的扩展选项，可以给枚举起中文名称，在EXCEL中使用
extend google.protobuf.EnumValueOptions 
{
    //枚举值的选项
    optional string enum_name = 50004;
}

```

然后我们定义我们自己的PB文件，我们用example的002的例子讲解一下。如下：

```
//option optimize_for = LITE_RUNTIME;
//必须引入illusion.proto文件，这样我们才能实用这些选项扩展
import "illusion.proto";

//定义的枚举结构
enum ENUM_COUNTRY
{
	AUSTRALIA =          61;         // Australia
	CHILE =              56;         // Chile
	EGYPT =              20;         // Egypt
	FINLAND =            358;        // Finland
	FRANCE =             33;         // France
	GERMANY =            49;         // Germany
	HONG_KONG =          852;        // Hong Kong S.A.R., P.R.C.
	ICELAND =            354;        // Iceland
	INDIA =              91;         // India
	IRAN =               981;        // Iran
	IRAQ =               964;        // Iraq
	IRELAND =            353;        // Ireland
	ITALY =              39;         // Italy
	JAPAN =              81;         // Japan
	MALAYSIA =           60;         // Malaysia
	NORWAY  =            47;         // Norway
	PRCHINA =            86;         // People's Republic of China
	POLAND =             48;         // Poland
	RUSSIA =             7;          // Russia
	SOUTH_KOREA =        82;         // Korea
	SPAIN =              34;          // Spain
	TAIWAN =             886;         // Taiwan
	UNITED_STATES =      1;           // United States
}

//子结构A，illusion.fields_name对应EXCEL Sheet表中的字段名称
message SUB_STRUCT_A
{
	optional double sub_a_1 = 1 [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置A1"];
	optional double sub_a_2 = 2 [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置A2"];
	optional string sub_a_3 = 3 [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置A3"];
	optional bytes sub_a_4 = 4  [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置A4"];
}

//子结构B，
message SUB_STRUCT_B
{
	optional int32 sub_b_1 = 1  [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置B1"];
	optional int64 sub_b_2 = 2  [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置B2"];
	optional uint32 sub_b_3 = 3 [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置B3"];
	optional int64 sub_b_4 = 4  [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置B4"];
    //注意这是一个false，这个字段不作为配置实用
    optional int64 sub_b_5 = 5  [(illusion.cfg_field)=false];
}

//配置结构的一行对应的结构的描述，
message GAME_CFG_STRUCT_2
{
    //枚举定义，其中illusion.enum_name定义对应的中文或者EXCEL表格中操作的别名，
	enum NESTED_ENUM 
	{
        MON = 1    [(illusion.enum_name)="星期一"];
        TUE = 2    [(illusion.enum_name)="星期二"];  
        WED = 3    [(illusion.enum_name)="星期三"];
        THU = 4    [(illusion.enum_name)="星期四"];
        FRI = 5    [(illusion.enum_name)="星期五"];
        SAT = 6    [(illusion.enum_name)="星期六"];
        SUN = 7    [(illusion.enum_name)="星期日"];
	}
	
	message NESTED_STRUCT_C
	{
		optional sint32  nested_c_1 = 1      [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置C1"];
		optional sint64  nested_c_2 = 2      [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置C2"];
		optional fixed32  nested_c_3 = 3     [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置C3"];
		optional fixed64  nested_c_4 = 4     [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置C4"];
		optional NESTED_ENUM nested_c_5 =5   [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置C5"];
	}                                        
	                                         
    message NESTED_STRUCT_D                  
	{                                        
		optional sfixed32  nested_d_1 = 1     [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置D1"];
		optional sfixed64  nested_d_2 = 2     [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置D2"];
		optional string  nested_d_3 = 3       [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置D3"];
		optional bytes  nested_d_4 = 4        [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置D4"];
	}
	
	optional int32 s2_a = 1                   [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置S2-1"];
	optional int32 s2_b = 2                   [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置S2-2"];
	optional uint32 s2_c = 3                  [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置S2-3"];
	repeated uint64 s2_d = 4                  [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置S2-4",  (illusion.repeat_size) =2];
	optional SUB_STRUCT_A  s2_e = 5           [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置S2-5"];
	repeated SUB_STRUCT_B  s2_f = 6           [(illusion.cfg_field)=true,  (illusion.repeat_size) =2];
	optional NESTED_STRUCT_C s2_g = 7         [(illusion.cfg_field)=true];
	repeated NESTED_STRUCT_D s2_h = 8         [(illusion.cfg_field)=true,  (illusion.repeat_size) =4];; 
	optional NESTED_ENUM s2_i = 9             [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置S2-9"];
	required ENUM_COUNTRY s2_j = 10           [(illusion.cfg_field)=true,  (illusion.fields_name) ="配置S2-10",  (illusion.repeat_size) =1];
}

//配置对应EXCEL Sheet的描述，他就是一个repeated GAME_CFG_STRUCT_2的数组，最后生产的配置结构也就是这个结构
message LIST_OF_GAME_CFG_STRUCT_2
{
    //message的选项信息
    option (illusion.cfg_message) = true;
    option (illusion.cfg_comment) = "游戏配置测试结构2";
    option (illusion.excel_file) = "game_config_02.xlsx";
    option (illusion.excel_sheet) = "GAME_CFG_STRUCT_2";
    option (illusion.outer_file) = "GAME_CFG_STRUCT_2.bin";
    option (illusion.fieldsname_line) = 1;
    option (illusion.fullname_line) = 2;
    option (illusion.read_line) = 3;
    //这个字段的名字必须是 list_data，注意，再注意
    repeated GAME_CFG_STRUCT_2 list_data = 1;
}

```

proto 文件里面可以定义N个表结构，每个表结构都是一个包含 repeated 行结构成员字段名称为 **list_data** 的结构。注意其中的字段名称必须是**list_data**，不能是其他名称。而这个结构里面有若干 message 扩展选项信息，包括 **illusion.cfg_message** 决定这个结构是否是配置的 Message 信息，**illusion.cfg_comment** 是这个结构的备注名称。**illusion.excel_file** 定义对应的EXCEL文件名称，**illusion.excel_sheet** 定义对应的Excel Sheet 名称。**illusion.outer_file** 对应相应的输出文件名称。**illusion.fieldsname_line** 定义相应的字段名称在 Sheet 表中间的第几行（你可以自由的控制格式），注意 **illusion** 是根据字段名称去查询对应的字段信息的，所以其实 Excel Sheet表格中，你可以自己控制跳过若干列用于自己的配置数据管理。**illusion.fullname_line** 是字段的 full name 对应的行，这个主要是为了方便程序员对应查询相应的问题。非必须字段。**illusion.read_line** 标识从几行开始进行读取操作。

上面例子里面 **LIST_OF_GAME_CFG_STRUCT_2** 对应相应的 Sheet 信息。其 repeated 结构是 **GAME_CFG_STRUCT_2** 而， **GAME_CFG_STRUCT_2** 里面包含了 **ENUM_COUNTRY**，**SUB_STRUCT_A**，**SUB_STRUCT_B**，以及内嵌的结构枚举，**NESTED_STRUCT_C**，**NESTED_STRUCT_D** 等。这些结构的 fields 字段有扩展选项，其中 **illusion.cfg_field** 扩展选项标识这个字段是否是配置需要字段。**illusion.fields_name** 字段标识在Sheet中对应的列标题，从Message **LIST_OF_GAME_CFG_STRUCT_2** 里面标识的 **illusion.fields_line** 行里面的字段查找对应的名称。枚举值可以有枚举值的扩展选项 **illusion.enum_name** ，用于给枚举值取一个别名，在EXCEL中可以用别名标识对应的值，方便我们进行某些转换。

### 2. 使用工具生产 Excel

#### 填写EXCEL文件

#### 导出bin文件

## 代码说明

### 1. 代码目录说明

GitHub上存放有 **illusion** 的主要源代码代码目录，包括 ProtoBuf 2.6 的代码，但不包括 Qt 的代码，所以请自己下载。目前还不包括可执行程序的代码代码目录，等更加稳定一点，估计会开始打包正式版本。

```
illusion
   |--------<bin>  可执行程序目录
   |--------<lib>  编译的库存放目录
   |--------<example> 导出的例子目录，有若干个例子，下面说明一个
   |          |---------<001>
   |                     |------<excel> 例子001的EXCEL文件存放目录
   |                     |------<import>
   |                     |        |------illusion.proto  PB扩展的描述proto文件，
   |                     |        |------<google>  
   |                     |                  |----<protobuf>
   |                     |                              |----descriptor.proto  protobufextend的依赖
   |                     |-------<proto> 例子001的PB描述文件存放目录
   |                     |-------<outer> 例子001生产的配置文件bin文件的存放目录
   |--------<depend>
   |         |----------<protobuf-2.6.1> 外部依赖库 protobuf 2.6.1的目录
   |--------src  源代码目录
             |---------<biko> 读取PB，EXCEL的基础代码目录，基本库，生成static lib
             |---------<dreams> 命令行工具的代码，命令行工具用于编译自动转换等方式
             |---------<import> import的PB文件illusion.proto 生成的基础代码，生产库文件
             |---------<vision> vision为图形化工具的代码目录
             |---------illusion.sln 工程文件,Visual Studio 2015 C++工程
```

### 2. 外部库

**illusion** 使用了一些外部库提供辅助功能。

#### Protobuf V2

**illusion** 目前实用 Protobuf V2 作为，一方面因为目前版本 2 还是主流。一方面是部分实用我工具的团队目前是用 Protobuf V3 有问题。所以暂未升级。但支持起来应该没啥困难。

#### Qt5

**illusion** 主要实用 Qt 完成工具界面的开发，以及相应的读取 Excel 部分的功能。**illusion** 目前实用的是 Qt 5.6 版本进行的编译，

## 感谢

**illusion** 的设计思路来自 yingqiliu，而我自己当时的设计思路，PB 文件不使用扩展，是在 Excel 里面配置和 Proto 的对应关系完成配置，使用 PB 的反射生产 bin 配置文件。yingqiliu 的方法的优势来自在于策划和程序开发的工作可以分开，程序在编写完成 PB 后，不需要再折腾 Excel 文件。如何使用 PB 扩展完成的相应的功能 andrewli 给出过一些例子参考。

