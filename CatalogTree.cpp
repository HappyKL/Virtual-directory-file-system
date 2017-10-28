#include"CatalogTree.h"
#include<stdio.h>

int main() {
	CatalogTree T;

	printf("***********************************************************\n");
	printf("菜单 虚拟文件目录系统\n");
	printf("dir         列出当前目录下所有目录项\n");
	printf("cd          查看当前路径\n");
	printf("cd dir      当前目录变为str所表示路径的目录\n");
	printf("cd ..       当前目录变为当前目录的父目录\n");
	printf("mkdir str   在当前目录下创建一个名为str的子目录\n");
	printf("mkfile str  在当前目录下创建一个名为str的文件\n");
	printf("delete str  删除当前目录下名为str的目录或文件\n");
	printf("save *.dat  保存虚拟目录到*.dat文件中\n");
	printf("load *.dat  载入*.dat文件中的虚拟目录\n");
	printf("size str    查看当前某子目录下的文件数\n");
	printf("quit        退出\n");
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
				printf("dir         列出当前目录下所有目录项\n");
				printf("cd          查看当前路径\n");
				printf("cd dir      当前目录变为str所表示路径的目录\n");
				printf("cd ..       当前目录变为当前目录的父目录\n");
				printf("mkdir str   在当前目录下创建一个名为str的子目录\n");
				printf("mkfile str  在当前目录下创建一个名为str的文件\n");
				printf("delete str  删除当前目录下名为str的目录或文件\n");
				printf("save *.dat  保存虚拟目录到*.dat文件中\n");
				printf("load *.dat  载入*.dat文件中的虚拟目录\n");
				printf("size str    查看当前某子目录下的文件数\n");
				printf("quit        退出\n");
				printf("***********************************************************\n");
			}
			else if (strlen(s) > 5 && s[0] == 's'&&s[1] == 'a'&&s[2] == 'v'&&s[3] == 'e') {//save
				strncpy(subs, s + 5, strlen(s) - 5);
				char filePath[100] = "F:\\";
				strcat(filePath, subs);
				T.save(filePath);
				T.ListDirToFile(T.root, 0, stdout);
				printf("保存成功\n");
				//T.mkfile(subs, T.currentPosition);
			}
			else if (strlen(s) > 5 && s[0] == 'l'&&s[1] == 'o'&&s[2] == 'a'&&s[3] == 'd') {//load *.dat
				strncpy(subs, s + 5, strlen(s) - 5);
				char filePath[100] = "F:\\";
				strcat(filePath, subs);
				T.load(filePath);
				T.ListDirToFile(T.root, 0, stdout);
				printf("载入成功\n");
			}
			else if (strlen(s) > 5 && s[0] == 's'&&s[1] == 'i'&&s[2] == 'z'&&s[3] == 'e') {//size str
				strncpy(subs, s + 5, strlen(s) - 5);
				
				T.size(subs);
			}
			else {
				printf("	没有此命令\n");
			}
		}
		else {
			if (strlen(s) == 3) {
				if (strcmp(s, "dir") == 0) {//dir
					T.ListDir();
				}
				else {
					printf("	没有此命令\n");
				}
			}
			else if (strlen(s) == 2) {//cd
				if (strcmp(s, "cd") == 0) {
					T.cd();
					printf("\n");
				}
				else
					printf("	没有此命令\n");
			}
			else if (strlen(s) == 1) {//p
				if (s[0] == 'p') {//打印目录结构
					T.ListDirToFile(T.root, 0, stdout);
				}
				else {
					printf("	没有此命令\n");
				}
			}
			else {
				printf("	没有此命令\n");
			}
		}
	}
	return 0;
}