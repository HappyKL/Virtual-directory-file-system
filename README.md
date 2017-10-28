# Virtual-directory-file-system
Virtual directory file system

## 虚拟文件目录系统：

- 本设计需完成两部分工作：一个是定义并实现一称为CatalogTree的ADT，用它来表达字符集合组成的有序树；另一个是shell的应用程序，用它来模拟文件目录系统，并提供模拟操作界面。

CatalogTree 的组织结构如下图（带父节点指针的儿子-兄弟链树）：


![](http://github.com/HappyKL/Virtual-directory-file-system/raw/master/a.png)


针对于目录系统，CatalogTree的结点存放的数据内容为字符串，每个结点对应一个目录项，该目录项可以是目录，也可以是文件，如果是目录就可以再存放其他目录或文件，即非叶节点；如果是文件就是叶节点。从根节点到该节点路径所有结点的字符串用‘/‘进行组合就是该目录项的绝对路径，用来唯一的标识该目录，例如：/usr/li/email/student/。
目录系统具有如下的基本操作：
1)	dir            列出当前目录下所有目录项
2)	cd            打出当前目录的绝对路径
3)	cd ..          当前目录变为当前目录的父目录
4)	cd str         当前目录变为str所表示路径的目录
5)	mkdir str      在（当前目录下）创建一个子目录（名为str）
6)	mkfile str      在（当前目录下）创建一个文件（名为str）
7)	delete str      删除（当前目录下）名为str的目录或文件

### 基本要求

1.	描述并实现CatalogTree的ADT，包括其上的基本操作：如插入一个结点，寻找一个节点，返回一个结点的最左儿子等(具体情况依据应用自定)；
2.	应用CatalogTree的ADT实现一个完成文件目录系统的shell应用程序；
3.	该Shell是一个不断等待用户输入命令的解释程序，根据用户输入的命令来完成相关操作，直到退出(quit),命令名及其含义如上所述。
4.	目录树结构可以保存(save)到文件中，也可以从文件中读出(load *.dat);
5.	Dir命令的结果应能够区分是子目录还是文件；
6.	应对命令4)-7)中的str区分是绝对路径还是相对路径。
