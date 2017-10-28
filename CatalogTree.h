#pragma once
#include<iostream>
#include<string.h>
#include <stdio.h>
using namespace std;

struct TreeNode {//���ڵ�
	struct TreeNode *parent;//��ָ��
	struct TreeNode *FirstChild;//��һ������ָ��
	struct TreeNode *xiongdi;//�ֵ�ָ��
	bool flag_file;//true��ʾ�ļ���false��ʾĿ¼
	char fileName[100];//�ļ���
	int depth;//���
	int size;//���ļ���Ŀ
};
typedef struct TreeNode *Position;//Ϊ��ʹ�÷���
typedef struct TreeNode *Tree;
typedef struct TreeNode *ptr;

class CatalogTree;
void cd_Position(CatalogTree *a, Position x);//����λ��Ѱ��·��
void deletePtr(CatalogTree *a, ptr t);//����λ��ɾ��

class CatalogTree {

public:
	TreeNode *root;
	ptr currentPosition;
public:
	CatalogTree();//���캯��
	~CatalogTree() {//��������
		deletePtr(this, root);
	};
	void mkdir(char *name, Position t);//����Ŀ¼
	void mkfile(char *name, Position t);//�����ļ�
	void ListDir();//�г���ǰĿ¼�µ��ļ�
	void Delete(char *str);//ɾ���ļ���Ŀ¼
	void cd();//��ӡ��ǰ·��
	void cdStr(char *str);//����ָ��·��
	void cdPre();//������·��
	void save(char *filename);//��Ŀ¼�ṹ�������ļ�
	void load(char *filename);//��Ŀ¼�ṹ���ļ�����
	void ListDirToFile(Position D, int Depth, FILE *file);//���ļ���ӡ��Ŀ¼�ṹ
	void size(char *dirName);//��ӡ��ǰĿ¼�µ��ļ�����

};


CatalogTree::CatalogTree()
{ //���췽��
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


void CatalogTree::size(char *dirName) {//��ӡ����ǰ·����ĳĿ¼���ļ���
	
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
		printf("	û�и�Ŀ¼���ļ�\n");
		return;
	}
	
	if (strcmp(dirName, "/") == 0)//��ӡ��Ŀ¼���ļ���
		printf("size of %s : %d\n", dirName, root->size);
	else
		printf("size of %s : %d\n", dirName, t->size);
	
}

void  CatalogTree::ListDirToFile(Position D, int Depth , FILE *file)//���ļ���ӡ��Ŀ¼�ṹ
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


void CatalogTree::save(char *filename) {//��Ŀ¼�ṹ�������ļ�
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("	�ļ���ʧ��\n");
		return;
	}
	ListDirToFile(this->root, 0 , file);//��Ŀ¼�ṹ�����ļ�
	fclose(file);
	//printf("	�����ļ��ɹ�\n");
}



void  ListFileToTree(CatalogTree *T , Position D ,char *preDir, int preDepth, FILE *file)//���ļ�����������
{
	char buf[120];
	char type[3];
	char subBuf[100];
	memset(buf, 0, sizeof(buf));
	memset(type, 0, sizeof(type));
	memset(subBuf, 0, sizeof(subBuf));
	int i;
	int t_n = 0;
	fgets(buf, sizeof(buf), file);//һ��һ�л�ȡ����
	if (strlen(buf) == 0) return;//��������ļ�ĩβ������
	strncpy(type, buf + strlen(buf) - 3, 2);//��ȡ�����������ļ�����Ŀ¼
	for (i = 0; buf[i] == '\t'; i++) {//����������ж��������������жϳ���һ�����ļ�
		t_n++;
	}
	if (i == 0) ListFileToTree(T , D , "/", 0 , file);//���û����������֤���ǵ�һ�и�·��
	
	else
	{

		strncpy(subBuf, buf + t_n, strlen(buf) - 4 - t_n);//��ȡ�������ļ���Ŀ¼���ļ�
		if (t_n > preDepth) {//�����������������һ�ж֤࣬�������ļ�/Ŀ¼Ϊ��һ��Ŀ¼�����ļ�
			T->cdStr(preDir);//�ı䵱ǰָ��Ϊ��һ��Ŀ¼
		}
		else if (t_n < preDepth) {//�������С����һ�У�ÿСһ�У���ǰָ����һ�Ρ��ص���һ·��������
			for (int j = t_n; j < preDepth; j++) {
				T->cdPre();
			}
		}
		//Ĭ���������һ���ļ�/Ŀ¼����ͬһ���

		if (strcmp(type, ".d") == 0) {//�����Ŀ¼
			T->mkdir(subBuf, T->currentPosition);
			//T->cdStr(subBuf);
		}
		else {//������ļ�
			T->mkfile(subBuf, T->currentPosition);
		}
		ListFileToTree(T, T->currentPosition,subBuf, t_n, file);//������һ��

	}
}


void CatalogTree::load(char *filename) {//������ǰλ�õ�·����ʹ�õݹ�
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		printf("	�ļ���ʧ�ܣ������ļ����Ƿ���ȷ\n");
		return;
	}
	ListFileToTree(this, currentPosition,"/" , 0, file);
	fclose(file);
	//printf("	����ɹ�\n");
	cdStr("/");
}

void cd_Position(CatalogTree *a , Position x) {//������ǰλ�õ�·����ʹ�õݹ�
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

void  CatalogTree::cd()//�����ǰ·��
{
	
	ptr x = currentPosition;
	cd_Position(this,x);
	//printf("\n");
}


void  CatalogTree::cdStr(char *str)//����·���ı�currentPosition
{
	ptr temp;
	if (str[0] == '/') {//�����һ���ַ�Ϊ'/',֤���Ǿ���·��
		ptr t = root;
		const char *d = "/";
		char *p;
		p = strtok(str, d);//�ָ��ַ���
		bool flag;
		while (p)
		{
			flag = false;
			for (temp = t->FirstChild; temp != NULL; temp = temp->xiongdi) {
				if (strcmp(temp->fileName, p) == 0&&temp->flag_file == false) {
					t = temp;//�����޸�t��ֵ
					flag = true;
					break;
				}
			}
			if (flag == false ) {//���Ϊfalse�����û������·��Ӧ���д�
				printf("	û�и�����\n");
				return;
			}
			//printf("%s\n", p);
			p = strtok(NULL, d);
		}
		currentPosition = t;

	}
	else {//���·��

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
					t = temp;//���ϸ��µ�ǰ·��
					flag = true;
					break;
				}
			}
			if (flag == false) {
				printf("	û�и�����\n");
				return;
			}
			
			p = strtok(NULL, d);
		}
		currentPosition = t;
	}
	
}

void  CatalogTree::cdPre() {//cd..
	if (currentPosition == root) {
		printf("	�Ѿ�����·��\n");
		return;
	}
	currentPosition = currentPosition->parent;
	//this->cd();
}

void  CatalogTree::ListDir()//�г���ǰĿ¼�������ļ�
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


void deletePtr(CatalogTree *a , ptr t) {//ɾ��ĳ���ļ���Ŀ¼
	ptr temp;
	if (t->flag_file) {//������ļ�
		temp = t->parent->FirstChild;
		if (temp == t) {//���ɾ�����ļ��Ǹ��׵ĵ�һ�����ӣ������׵Ķ���ָ��ָ����ļ����ֵܣ�Ȼ���ͷ�
			t->parent->FirstChild = temp->xiongdi;
			free(t);
			return;
		}
		for (temp = t->parent->FirstChild; temp != NULL; temp = temp->xiongdi) {//������Ǹ��׵ĵ�һ�����ӣ����ҵ��ýڵ��λ�ã��øýڵ��ǰ�����ֵ�ָ��ָ��ɾ�������ֵܣ�Ȼ���ͷ�
			if (t == temp->xiongdi) {
				temp->xiongdi = t->xiongdi;
				free(t);
				return;
			}
		}
	}
	else {//�����Ŀ¼
		if (t->FirstChild == NULL) {//�����Ŀ¼û���ļ�����ֱ��ɾ��
			if (t == a->root) return;
			if (t->parent->FirstChild == t) {
				t->parent->FirstChild = t->xiongdi;//�����Ŀ¼λ�ڸ��׽ڵ�ĵ�һ�����ӣ��򽫸��׵Ķ���ָ��Ϊ�գ�Ȼ��ɾ��
			}
			else
				for (temp = t->parent->FirstChild; temp != NULL; temp = temp->xiongdi) {//������ǵ�һ�����ӣ����ҵ��ýڵ��ǰ������ǰ�����ֵ�ָ��ָ��ýڵ���ֵܣ�Ȼ���ͷŸýڵ�
					if (temp->xiongdi == t) {
						temp->xiongdi = t->xiongdi;
						break;
					}
				}
			free(t);
		}
		else {//�����Ŀ¼�����ļ�������еݹ�ɾ��
			while (t->FirstChild != NULL) {
				for (temp = t->FirstChild; temp != NULL; temp = temp->xiongdi) {//ɾ��ʱӦ
					if (temp->xiongdi == NULL) {
						deletePtr(a , temp);
						break;
					}

				}
			}
			deletePtr(a , t);///////////�������ͷ�
		}
	}
}

void CatalogTree::Delete(char *str) {//ɾ��
	
	//ɾ������
	Position t;
	bool flag = false;
	for (t = currentPosition->FirstChild; t != NULL; t = t->xiongdi) {
		if (strcmp(t->fileName, str) == 0) {
			flag = true;
			break;
		}
	}
	if (flag == false) {
		printf("	û�и�Ŀ¼���ļ�\n");
		return;
	}
	deletePtr(this, t);
	
}

void CatalogTree::mkdir(char *name, Position t) {//�����ļ���

	ptr temp;
	for (temp = t->FirstChild; temp != NULL; temp = temp->xiongdi) {
		if (strcmp(temp->fileName, name) == 0) {//&&temp->flag_file == false
			cout << "	���ܲ�����ͬ���ֵ�Ŀ¼���ļ�,����ʧ��" << endl;
			return;
		}
	}

	temp = (ptr)malloc(sizeof(struct TreeNode));//�������ڵ�
	temp->parent = t;
	temp->FirstChild = NULL;
	temp->flag_file = false;//false��ʾ�ýڵ�ΪĿ¼
	temp->size = 0;
	strcpy(temp->fileName, name);
	temp->xiongdi = t->FirstChild;
	t->FirstChild = temp;

}

void CatalogTree::mkfile(char *name, Position t) {//�����ļ�
	ptr temp;

	for (temp = t->FirstChild; temp != NULL; temp = temp->xiongdi) {
		if (strcmp(temp->fileName, name) == 0 ) {//&& temp->flag_file == true
			cout << "	���ܲ�����ͬ���ֵ��ļ�,����ʧ��" << endl;
			return;
		}
	}

	temp = (ptr)malloc(sizeof(struct TreeNode));//���������
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



