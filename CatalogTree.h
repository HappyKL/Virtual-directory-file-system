#pragma once
#include<iostream>
#include<string.h>
#include <stdio.h>
using namespace std;

struct TreeNode {//树节点
	struct TreeNode *parent;//父指针
	struct TreeNode *FirstChild;//第一个儿子指针
	struct TreeNode *xiongdi;//兄弟指针
	bool flag_file;//true表示文件，false表示目录
	char fileName[100];//文件名
	int depth;//深度
	int size;//子文件数目
};
typedef struct TreeNode *Position;//为了使用方便
typedef struct TreeNode *Tree;
typedef struct TreeNode *ptr;

class CatalogTree;
void cd_Position(CatalogTree *a, Position x);//根据位置寻找路径
void deletePtr(CatalogTree *a, ptr t);//根据位置删除

class CatalogTree {

public:
	TreeNode *root;
	ptr currentPosition;
public:
	CatalogTree();//构造函数
	~CatalogTree() {//析构函数
		deletePtr(this, root);
	};
	void mkdir(char *name, Position t);//创建目录
	void mkfile(char *name, Position t);//创建文件
	void ListDir();//列出当前目录下的文件
	void Delete(char *str);//删除文件或目录
	void cd();//打印当前路径
	void cdStr(char *str);//跳到指定路径
	void cdPre();//跳到父路径
	void save(char *filename);//将目录结构保存至文件
	void load(char *filename);//将目录结构从文件载入
	void ListDirToFile(Position D, int Depth, FILE *file);//从文件打印出目录结构
	void size(char *dirName);//打印当前目录下的文件个数

};


CatalogTree::CatalogTree()
{ //构造方法
	ptr m_root = (struct TreeNode*)malloc(sizeof(struct TreeNode));
	m_root->FirstChild = NULL;
	memset(m_root->fileName, 0, sizeof(m_root->fileName));
	m_root->fileName[0] = '/';
	m_root->flag_file = false;
	m_root->parent = NULL;
	m_root->xiongdi = NULL;
	m_root->size = 0;
	root = m_root;
	currentPosition = root;

};


void CatalogTree::size(char *dirName) {//打印出当前路径下某目录的文件数
	
	Position t;
	bool flag = false;
	for (t = currentPosition->FirstChild; t != NULL; t = t->xiongdi) {
		if (strcmp(t->fileName, dirName) == 0) {
			flag = true;
			break;
		}
	}

	if (strcmp(dirName, "/") == 0) flag = true;

	if (flag == false) {
		printf("	没有该目录或文件\n");
		return;
	}
	
	if (strcmp(dirName, "/") == 0)//打印根目录的文件数
		printf("size of %s : %d\n", dirName, root->size);
	else
		printf("size of %s : %d\n", dirName, t->size);
	
}

void  CatalogTree::ListDirToFile(Position D, int Depth , FILE *file)//从文件打印出目录结构
{
	ptr temp;
	if (D!=NULL) {
		for (int i = 0; i < Depth; i++) {
			fprintf(file , "\t");
		}
		if (D->flag_file == true) {
			//printf("%s .f\n", D->fileName);
			fprintf(file, "%s .f\n", D->fileName);
		}
			
		else {
			//printf("%s .d\n", D->fileName);
			fprintf(file, "%s .d\n", D->fileName);
		}
			
		if (D->flag_file == false)
			for (temp = D->FirstChild; temp != NULL; temp = temp->xiongdi)
				ListDirToFile(temp, Depth + 1 , file);
	}
}


void CatalogTree::save(char *filename) {//将目录结构保存至文件
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("	文件打开失败\n");
		return;
	}
	ListDirToFile(this->root, 0 , file);//将目录结构存入文件
	fclose(file);
	//printf("	保存文件成功\n");
}



void  ListFileToTree(CatalogTree *T , Position D ,char *preDir, int preDepth, FILE *file)//将文件中内容载入
{
	char buf[120];
	char type[3];
	char subBuf[100];
	memset(buf, 0, sizeof(buf));
	memset(type, 0, sizeof(type));
	memset(subBuf, 0, sizeof(subBuf));
	int i;
	int t_n = 0;
	fgets(buf, sizeof(buf), file);//一行一行获取数据
	if (strlen(buf) == 0) return;//如果读到文件末尾，结束
	strncpy(type, buf + strlen(buf) - 3, 2);//获取改行数据是文件还是目录
	for (i = 0; buf[i] == '\t'; i++) {//计算改行中有多少缩进，可以判断出哪一级的文件
		t_n++;
	}
	if (i == 0) ListFileToTree(T , D , "/", 0 , file);//如果没有缩进，则证明是第一行根路径
	
	else
	{

		strncpy(subBuf, buf + t_n, strlen(buf) - 4 - t_n);//获取改行中文件或目录的文件
		if (t_n > preDepth) {//如果改行中缩进比上一行多，证明改行文件/目录为上一行目录的子文件
			T->cdStr(preDir);//改变当前指标为上一行目录
		}
		else if (t_n < preDepth) {//如果缩进小于上一行，每小一行，则当前指针做一次“回到上一路径”操作
			for (int j = t_n; j < preDepth; j++) {
				T->cdPre();
			}
		}
		//默认深度与上一行文件/目录属于同一深度

		if (strcmp(type, ".d") == 0) {//如果是目录
			T->mkdir(subBuf, T->currentPosition);
			//T->cdStr(subBuf);
		}
		else {//如果是文件
			T->mkfile(subBuf, T->currentPosition);
		}
		ListFileToTree(T, T->currentPosition,subBuf, t_n, file);//进入下一行

	}
}


void CatalogTree::load(char *filename) {//搜索当前位置的路径，使用递归
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		printf("	文件打开失败，请检查文件名是否正确\n");
		return;
	}
	ListFileToTree(this, currentPosition,"/" , 0, file);
	fclose(file);
	//printf("	载入成功\n");
	cdStr("/");
}

void cd_Position(CatalogTree *a , Position x) {//搜索当前位置的路径，使用递归
	if (x == a->root) {
		printf("/");
		return;
	}
	else
	{
		cd_Position(a, x->parent);
		printf("%s/", x->fileName);
	}
}

void  CatalogTree::cd()//输出当前路径
{
	
	ptr x = currentPosition;
	cd_Position(this,x);
	//printf("\n");
}


void  CatalogTree::cdStr(char *str)//根据路径改变currentPosition
{
	ptr temp;
	if (str[0] == '/') {//如果第一个字符为'/',证明是绝对路径
		ptr t = root;
		const char *d = "/";
		char *p;
		p = strtok(str, d);//分隔字符串
		bool flag;
		while (p)
		{
			flag = false;
			for (temp = t->FirstChild; temp != NULL; temp = temp->xiongdi) {
				if (strcmp(temp->fileName, p) == 0&&temp->flag_file == false) {
					t = temp;//不断修改t的值
					flag = true;
					break;
				}
			}
			if (flag == false ) {//如果为false，则用户输入的路径应该有错
				printf("	没有该命令\n");
				return;
			}
			//printf("%s\n", p);
			p = strtok(NULL, d);
		}
		currentPosition = t;

	}
	else {//相对路径

		const char *d = "/";
		char *p;
		ptr t = currentPosition;
		p = strtok(str, d);
		bool flag;
		while (p)
		{
			flag = false;
			for (temp = t->FirstChild; temp != NULL; temp = temp->xiongdi) {
				if (strcmp(temp->fileName, p) == 0&&temp->flag_file == false) {
					t = temp;//不断更新当前路径
					flag = true;
					break;
				}
			}
			if (flag == false) {
				printf("	没有该命令\n");
				return;
			}
			
			p = strtok(NULL, d);
		}
		currentPosition = t;
	}
	
}

void  CatalogTree::cdPre() {//cd..
	if (currentPosition == root) {
		printf("	已经到根路径\n");
		return;
	}
	currentPosition = currentPosition->parent;
	//this->cd();
}

void  CatalogTree::ListDir()//列出当前目录下所有文件
{
	Position t = currentPosition;
	ptr temp;
	for (temp = t->FirstChild; temp != NULL; temp = temp->xiongdi) {
		if (temp->flag_file) {
			printf("	%s  .f\n", temp->fileName);
		}
		else {
			printf("	%s  .d\n", temp->fileName);
		}
	}
}


void deletePtr(CatalogTree *a , ptr t) {//删除某个文件或目录
	ptr temp;
	if (t->flag_file) {//如果是文件
		temp = t->parent->FirstChild;
		if (temp == t) {//如果删除的文件是父亲的第一个儿子，将父亲的儿子指针指向该文件的兄弟，然后释放
			t->parent->FirstChild = temp->xiongdi;
			free(t);
			return;
		}
		for (temp = t->parent->FirstChild; temp != NULL; temp = temp->xiongdi) {//如果不是父亲的第一个儿子，则找到该节点的位置，让该节点的前驱的兄弟指针指向删除结点的兄弟，然后释放
			if (t == temp->xiongdi) {
				temp->xiongdi = t->xiongdi;
				free(t);
				return;
			}
		}
	}
	else {//如果是目录
		if (t->FirstChild == NULL) {//如果该目录没有文件，则直接删除
			if (t == a->root) return;
			if (t->parent->FirstChild == t) {
				t->parent->FirstChild = t->xiongdi;//如果该目录位于父亲节点的第一个儿子，则将父亲的儿子指针为空，然后删除
			}
			else
				for (temp = t->parent->FirstChild; temp != NULL; temp = temp->xiongdi) {//如果不是第一个儿子，则找到该节点的前驱，将前驱的兄弟指针指向该节点的兄弟，然后释放该节点
					if (temp->xiongdi == t) {
						temp->xiongdi = t->xiongdi;
						break;
					}
				}
			free(t);
		}
		else {//如果该目录下有文件，则进行递归删除
			while (t->FirstChild != NULL) {
				for (temp = t->FirstChild; temp != NULL; temp = temp->xiongdi) {//删除时应
					if (temp->xiongdi == NULL) {
						deletePtr(a , temp);
						break;
					}

				}
			}
			deletePtr(a , t);///////////不用在释放
		}
	}
}

void CatalogTree::Delete(char *str) {//删除
	
	//删除操作
	Position t;
	bool flag = false;
	for (t = currentPosition->FirstChild; t != NULL; t = t->xiongdi) {
		if (strcmp(t->fileName, str) == 0) {
			flag = true;
			break;
		}
	}
	if (flag == false) {
		printf("	没有该目录或文件\n");
		return;
	}
	deletePtr(this, t);
	
}

void CatalogTree::mkdir(char *name, Position t) {//创建文件夹

	ptr temp;
	for (temp = t->FirstChild; temp != NULL; temp = temp->xiongdi) {
		if (strcmp(temp->fileName, name) == 0) {//&&temp->flag_file == false
			cout << "	不能产生相同名字的目录或文件,创建失败" << endl;
			return;
		}
	}

	temp = (ptr)malloc(sizeof(struct TreeNode));//创建树节点
	temp->parent = t;
	temp->FirstChild = NULL;
	temp->flag_file = false;//false表示该节点为目录
	temp->size = 0;
	strcpy(temp->fileName, name);
	temp->xiongdi = t->FirstChild;
	t->FirstChild = temp;

}

void CatalogTree::mkfile(char *name, Position t) {//创建文件
	ptr temp;

	for (temp = t->FirstChild; temp != NULL; temp = temp->xiongdi) {
		if (strcmp(temp->fileName, name) == 0 ) {//&& temp->flag_file == true
			cout << "	不能产生相同名字的文件,创建失败" << endl;
			return;
		}
	}

	temp = (ptr)malloc(sizeof(struct TreeNode));//创建树结点
	temp->parent = t;
	temp->FirstChild = NULL;
	temp->flag_file = true;
	temp->size = 1;
	strcpy(temp->fileName, name);
	temp->xiongdi = t->FirstChild;
	t->FirstChild = temp;
	for (temp = t; t != NULL; t = t->parent) {
		t->size++;
	}
}



