# ILLUSION

**illusion**是一个使用[Protobuf](https://developers.google.com/protocol-buffers/)（Google  Protocol Buffers 2.0）文件作为配置描述，然后读取EXCEL文件里面相关的配置，然后将配置信息生产配置数据文件的工具。
**illusion**主要用于游戏配置管理等。程序开发可以快速的在PB文件里面定义配置结构信息，以及配置结构和EXCEL表直接的关系（ 生成EXCEL配置模版），策划可以方便的直接使用EXCEL进行数据配置。
希望通过这个工具提高游戏开发团队的开发效率。
###工具简介
**illusion**工具采用典型的左树右图UI界面。左边是配置表集合的导航树。右边是配置文件打开预览区，相应目录的PB(.proto)文件,EXCEL(.xls,.xlsx)文件都可以在这个区域形成预览。方便您迅速的了解相关配置信息。工具界面如下图：
![](http://i1.piimg.com/588926/f0486520c653ec7e.png)

使用工具初始化有2种方式，
一种是分别配置不同的子目录，**illusion**有4种数据，Protobuf .proto文件放置的目录；EXCEL .xls,.xlsx文件放置的目录；Protobuf import 文件放置的目录（**illusion**使用，所以也有自己的import目录），熟悉Protobuf的同学应该知道用途，以及生产文件.bin文件放置的目录。配置的时候选择Toolbar工具条上的第二个按钮。打开界面如下。
![](http://i2.buimg.com/588926/0a907b1d32e16eb5.png)

一种是ALL IN ONE的模式，要求所有的配置目录放到一个子目录下，只配置打开一个目录。这种情况要求这个目录下分别有4个子目录放置不同的数据，分别是/import，/proto,/outer,/excel目录，（就是上面分别配置4种目录）。配置的时候选择Toolbar工具条上的第二个按钮。
![](http://i1.piimg.com/588926/1addce2493ee43ed.png)

配置好目录后，**illusion**会分析proto文件目录，得到对应信息，同时检查相应的EXCEL文件和最后生成的配置文件是否存在，那个更新。然后会根据这些信息生产配置导航树。然后就可以在导航树上选择需要导出的配置表了。导航树默认的SHEET导航是用EXCEL的SHEET表名称作为树的根节点的。
只有相应有新的EXCEL配置的表格，程序会自动勾选。

##使用说明
###1.程序开发定义PB文件

###2.使用工具生产EXCEL

###3.导出bin文件

##代码说明

###1.代码目录说明


```
illusion

   |--------bin  可执行程序
   |--------lib  编译的库存放目录
   |--------src  源代码目录
             |---------
             |---------
             |---------

```

###2.外部库

**illusion**使用了一些外部库提供功能

####Protobuf 2




####Qt

主要实用Qt 完成工具界面的开发，以及相应的读取EXCEL部分的功能。

##感谢
**illusion**的设计思路来自yingqiliu，而我自己当时的设计思路，PB文件不使用扩展，是在EXCEL里面配置和Proto的对应关系完成配置，使用PB的反射生产bin配置文件。yingqiliu的方法的优势来自在于策划和程序开发的工作可以分开，程序在编写完成PB后，不需要再折腾EXCEL文件。如何实用PB扩展完成的思路。














