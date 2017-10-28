#include"CatalogTree.h"
#include<stdio.h>

int main() {
	CatalogTree T;

	printf("***********************************************************\n");
	printf("�˵� �����ļ�Ŀ¼ϵͳ\n");
	printf("dir         �г���ǰĿ¼������Ŀ¼��\n");
	printf("cd          �鿴��ǰ·��\n");
	printf("cd dir      ��ǰĿ¼��Ϊstr����ʾ·����Ŀ¼\n");
	printf("cd ..       ��ǰĿ¼��Ϊ��ǰĿ¼�ĸ�Ŀ¼\n");
	printf("mkdir str   �ڵ�ǰĿ¼�´���һ����Ϊstr����Ŀ¼\n");
	printf("mkfile str  �ڵ�ǰĿ¼�´���һ����Ϊstr���ļ�\n");
	printf("delete str  ɾ����ǰĿ¼����Ϊstr��Ŀ¼���ļ�\n");
	printf("save *.dat  ��������Ŀ¼��*.dat�ļ���\n");
	printf("load *.dat  ����*.dat�ļ��е�����Ŀ¼\n");
	printf("size str    �鿴��ǰĳ��Ŀ¼�µ��ļ���\n");
	printf("quit        �˳�\n");
	printf("***********************************************************\n");

	char s[1000];
	char subs[1000];
	while (true) {
		T.cd();
		printf("->");
		memset(s, 0, sizeof(s));
		memset(subs, 0, sizeof(subs));
		gets_s(s);
		if (strlen(s) > 3) {
			if (s[0] == 'c'&&s[1] == 'd') {//cd ..
				strncpy(subs, s + 3, strlen(s)-3);
				if (strcmp(subs, "..") == 0) {
					T.cdPre();
				}
				else {//cd str
					T.cdStr(subs);
				}
			}
			else if (strlen(s)>6 && s[0] == 'm'&&s[1] == 'k'&&s[2] == 'd'&&s[3] == 'i'&&s[4] == 'r') {//mkdir str
				strncpy(subs, s + 6, strlen(s) - 6);
				T.mkdir(subs,T.currentPosition);
			}
			else if (strlen(s)> 7 && s[0] == 'm'&&s[1] == 'k'&&s[2] == 'f'&&s[3] == 'i'&&s[4] == 'l'&&s[5] == 'e') {//mkfile str
				strncpy(subs, s + 7, strlen(s) - 7);
				T.mkfile(subs,T.currentPosition);
			}
			else if (strlen(s) > 7 && s[0] == 'd'&&s[1] == 'e'&&s[2] == 'l'&&s[3] == 'e'&&s[4] == 't'&&s[5] == 'e') {//delete str
				strncpy(subs, s + 7, strlen(s) - 7);
				T.Delete(subs);
			}
			else if (strlen(s) == 4 && strcmp(s,"quit")==0) {//quit
				//T.save("F:\\mulu.dat");
				return 0;
			}
			else if (strlen(s) == 4 && strcmp(s, "help") == 0) {//help
				printf("***********************************************************\n");
				printf("dir         �г���ǰĿ¼������Ŀ¼��\n");
				printf("cd          �鿴��ǰ·��\n");
				printf("cd dir      ��ǰĿ¼��Ϊstr����ʾ·����Ŀ¼\n");
				printf("cd ..       ��ǰĿ¼��Ϊ��ǰĿ¼�ĸ�Ŀ¼\n");
				printf("mkdir str   �ڵ�ǰĿ¼�´���һ����Ϊstr����Ŀ¼\n");
				printf("mkfile str  �ڵ�ǰĿ¼�´���һ����Ϊstr���ļ�\n");
				printf("delete str  ɾ����ǰĿ¼����Ϊstr��Ŀ¼���ļ�\n");
				printf("save *.dat  ��������Ŀ¼��*.dat�ļ���\n");
				printf("load *.dat  ����*.dat�ļ��е�����Ŀ¼\n");
				printf("size str    �鿴��ǰĳ��Ŀ¼�µ��ļ���\n");
				printf("quit        �˳�\n");
				printf("***********************************************************\n");
			}
			else if (strlen(s) > 5 && s[0] == 's'&&s[1] == 'a'&&s[2] == 'v'&&s[3] == 'e') {//save
				strncpy(subs, s + 5, strlen(s) - 5);
				char filePath[100] = "F:\\";
				strcat(filePath, subs);
				T.save(filePath);
				T.ListDirToFile(T.root, 0, stdout);
				printf("����ɹ�\n");
				//T.mkfile(subs, T.currentPosition);
			}
			else if (strlen(s) > 5 && s[0] == 'l'&&s[1] == 'o'&&s[2] == 'a'&&s[3] == 'd') {//load *.dat
				strncpy(subs, s + 5, strlen(s) - 5);
				char filePath[100] = "F:\\";
				strcat(filePath, subs);
				T.load(filePath);
				T.ListDirToFile(T.root, 0, stdout);
				printf("����ɹ�\n");
			}
			else if (strlen(s) > 5 && s[0] == 's'&&s[1] == 'i'&&s[2] == 'z'&&s[3] == 'e') {//size str
				strncpy(subs, s + 5, strlen(s) - 5);
				
				T.size(subs);
			}
			else {
				printf("	û�д�����\n");
			}
		}
		else {
			if (strlen(s) == 3) {
				if (strcmp(s, "dir") == 0) {//dir
					T.ListDir();
				}
				else {
					printf("	û�д�����\n");
				}
			}
			else if (strlen(s) == 2) {//cd
				if (strcmp(s, "cd") == 0) {
					T.cd();
					printf("\n");
				}
				else
					printf("	û�д�����\n");
			}
			else if (strlen(s) == 1) {//p
				if (s[0] == 'p') {//��ӡĿ¼�ṹ
					T.ListDirToFile(T.root, 0, stdout);
				}
				else {
					printf("	û�д�����\n");
				}
			}
			else {
				printf("	û�д�����\n");
			}
		}
	}
	return 0;
}