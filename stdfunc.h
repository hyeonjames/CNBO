#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include <iostream>
#include <conio.h>
#include<winsock2.h>
#include <string>
#define ID_YES 87
#define ID_NO 88
using namespace std;
void title(char *change);
void SetColor(int color);
void sysclear();
void SetConsoleSize(int lines,int cols){
	char tm[255];
	sprintf(tm,"mode con lines=%d cols=%d",lines,cols);
	system(tm);
}
int YesNoMenu2(int x,int y,char* descip);
int MenuFirst(int count,int menutype,char* descip,string *menuname,int* menuport,int stx);

void gotoxy(int x, int y)
{
	COORD pos={x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void SetColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void title(char *change){
	char *tm=new char[sizeof(change)+7];
	sprintf(tm,"title %s",change);
	system(tm);
} 

int MenuFirst(int count,int menutype,char* descip,string *menuname,int *menuport,int stx){
    int i,j,k;
	sysclear();
	SetConsoleSize(count+10+menutype,85);
    if(menutype<2){
		while(1){
			gotoxy(10,3);
			if(menutype==1){
				SetColor(14);
				printf("%s\n",descip);
			}
			gotoxy(10,4);
			SetColor(12);
			printf("┏Menu━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
			for(i=0;i<count;i++){
				SetColor(12);
				gotoxy(10,i+4+menutype);
				printf("┃");  
				if(stx==i){
					SetColor(42);
					printf("  ☞");
					cout << menuname[i];
					for(j=0;j<58-menuname[i].size();j++){
						gotoxy(menuname[i].size()+16+j,i+4+menutype);
						printf(" ");
					}
				}
				else{
					SetColor(13);
					printf("  　");
					cout << menuname[i];
					for(j=0;j<58-menuname[i].size();j++){
						gotoxy(menuname[i].size()+16+j,i+4+menutype);
						printf(" ");
					}
				}
				SetColor(12);
				gotoxy(72,i+4+menutype);
				printf("┃\n");
			}
			SetColor(12);
			gotoxy(10,count+4+menutype);
			printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
			SetColor(10);
			gotoxy(10,count+5+menutype);
			printf("made by qoo\n");
			SetColor(7);
			k=0;
			while(1){
				k=getch();
				if(k==72)
					if(stx>0){
						stx--;
						break;}
					if(k==80)
						if(stx<count-1){
							stx++;
							break;}
					if(k==13) return menuport[stx];
			}
		}
    }
	return 0;
}
void sysclear(){
	SetColor(7);
	system("cls");
}

int YesNoMenu2(int x,int y,char* descip)
{
	int width=strlen(descip);
	int nw=ID_YES,key=0;
	gotoxy(x,y);
	printf("%s",descip);
	while(1){
		if(nw==ID_YES) SetColor(23);
		else SetColor(15);
		gotoxy(x,y+1);
		printf("예");
		if(nw==ID_NO) SetColor(23);
		else SetColor(15);
		gotoxy(x+width-6,y+1);
		printf("아니오");
		key=getch();
		if(key==75) nw=ID_YES;
		else if(key==77) nw=ID_NO;
		else if(key==13)
		{
			SetColor(15);
			return nw;
		}
	}
}