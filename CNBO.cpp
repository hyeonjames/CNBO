#include "stdfunc.h"
#include<time.h>
#define CONNECT 13
#define LISTEN 14
#define TESTSTAGE 15
#define EXIT 50
#define STPORT 33213
#define SERVER 88
#define CLIENT 89
#define MAXXY 5
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define ENTER 13
#define SPACE 32
using namespace std;
SOCKET sClient,sListen;
sockaddr_in addr,clnt;
int clntsize,me;
int win=0,lose=0,suv=SERVER;
WSADATA wsaData;
void ErrorHading(char *s);
void connecting();
void listenning();
void makebingo(int bingo[MAXXY][MAXXY],int x,int y);
void PrintFor(char *s,int k);
void Game();
void Welcome();
bool strcmpindex(int start,int end,char *tmp1,char *tmp2);
void DrawPan(int x,int y);
void SetNotice(char *s);
void SetNoticeSleep(char *s);
char *RecvMsg();
bool chkbingo(bool bingo[MAXXY][MAXXY]);
void SendMsg(char *s);
void Random(int t[MAXXY][MAXXY]);
int main(void)
{
	string mainmenu[]={"�����ϱ�","��������","Exit"};
	int port[]={CONNECT,LISTEN,EXIT};
	int select=0;
	title("CNBO 0.0.2  Made by Qoo : http://cyworld.com/qoohk");
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0) ErrorHading("Wsa Start Error");
	srand(time(NULL));
	select = MenuFirst(3,1,"Select Menu.",mainmenu,port,0);
	switch(select)
	{
	case CONNECT:
		me=CLIENT;
		connecting();
		break;
	case LISTEN:
		me=SERVER;
		listenning();
		break;
	case EXIT:
		WSACleanup();
		exit(0);
		break;
	}
	WSACleanup();
	return EXIT_SUCCESS;
}
void connecting()
{
	SetColor(10);
	char ip[20];
	sysclear();
	printf("���� �ʱ�ȭ���Դϴ�..");
	sClient = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sClient == INVALID_SOCKET) ErrorHading("���� �ʱ�ȭ ����.");
	SetColor(11);
	printf("\n�����Ͻ� �����Ǹ� �Է����ֽʽÿ�: ");
	scanf("%s",ip);
	memset(&addr,0,sizeof(addr));
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_family = AF_INET;
	addr.sin_port = STPORT;
	SetColor(12);
	printf("�������Դϴ�...");
	if(connect(sClient,(SOCKADDR*)&addr,sizeof(addr))==SOCKET_ERROR)
	{
		printf("   ���ӽ���..!\n");
		SetColor(13);
		if(YesNoMenu2(0,3,"�ٽ� ���ӽõ� �Ͻðڽ��ϱ�?")==ID_YES)
		{
			connecting();
			return ;
		}
		else
		{
			main();
			return ;
		}
	}
	SetColor(13);
	printf("\n���� ����.\n");
	Sleep(500);
	SetColor(14);
	printf("Please Wait 3 Seconds.");
	Sleep(3000);
	Game();
}
void listenning()
{
	SetColor(10);
	sysclear();
	printf("���� �ʱ�ȭ��..\n");
	sListen = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sListen == INVALID_SOCKET) ErrorHading("���� �ʱ�ȭ ����");
	SetColor(11);
	memset(&addr,0,sizeof(addr));
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = STPORT;
	addr.sin_family = AF_INET;
	printf("���� bind ��..\n");
	if(bind(sListen,(SOCKADDR*)&addr,sizeof(addr))==SOCKET_ERROR) ErrorHading("Bind ����.");
	SetColor(12);
	printf("���� Listen �� ..\n");
	if(listen(sListen,5)==SOCKET_ERROR) ErrorHading("Listen ����.");
	SetColor(13);
	printf("Ŭ���̾�Ʈ ���� ��û��..\n");
	memset(&clnt,0,sizeof(clnt)); clntsize = sizeof(clnt);
	sClient = accept(sListen,(SOCKADDR*)&clnt,&clntsize);
	SetColor(14);
	printf("���� ��û ���� : IP %s\n",inet_ntoa(clnt.sin_addr));
	Sleep(500);
	printf("Please Wait 3 Seconds.");
	Sleep(3000);
	Game();
}
void ErrorHading(char *s)
{
	MessageBox(NULL,TEXT(s),TEXT("CNBO"),MB_OK);
	exit(0);
}
void Game()
{
	int nx,ny,i,j,tx,ty,key=0,selected=0;
	int bingo[MAXXY][MAXXY]={{0}};
	bool mebingo[MAXXY][MAXXY]={{false}};
	bool enmbingo[MAXXY][MAXXY]={{false}};
	SetConsoleSize(14,25);
	char msg[255];
	//���� �����
	makebingo(bingo,0,0);
	sysclear();
	SetColor(15);
	printf("�غ� �Ǿ����ϴ�..\n���� �غ� ��ٸ�����..");
	SendMsg("READY");
	while(!strcmp(RecvMsg(),"READY"));
	sysclear();
	SetConsoleSize(16,60);
	SetColor(10);
	DrawPan(4,2);
	SetColor(9);
	DrawPan(34,2);
	gotoxy(2,14);
	printf("���� ���� : %d �� %d ��",win,lose);
	if(win+lose==0) Welcome();
	while(1)
	{
		nx=ny=2;
		SetColor(15);
		for(i=0;i<MAXXY;i++)
		{
			for(j=0;j<MAXXY;j++)
			{
				gotoxy(4+j*4+2,2+i*2+1);
				if(me==suv && nx==j && ny==i)
				{
					SetColor(14);
					if(mebingo[i][j]) printf("��");
					else printf("%.2d",bingo[i][j]);
					SetColor(15);
				}
				else
				{
					if(mebingo[i][j]) printf("��");
					else printf("%.2d",bingo[i][j]);
				}
				if(enmbingo[i][j])
				{
					gotoxy(34+j*4+2,2+i*2+1);
					printf("��");
				}
			}
		}
		if(me==suv)
		{
			SetNotice("����� �Ͻ� �����Դϴ�.  ");
			nx=ny=2;
			while(1)
			{
				tx=nx; ty=ny;
				key=getch();
				switch(key)
				{
				case UP:
					if(ny>0) ny--;
					else ny=MAXXY-1;
					break;
				case DOWN:
					if(ny<MAXXY-1) ny++;
					else ny=0;
					break;
				case LEFT:
					if(nx>0) nx--;
					else nx=MAXXY-1;
					break;
				case RIGHT:
					if(nx<MAXXY-1) nx++;
					else nx=0;
					break;
				}
				if(key=='b' || key=='B')
				{
					if(chkbingo(mebingo))
					{
						SendMsg("BINGO");
						win++;
						SetNoticeSleep("�Ϳ� ~ �̱�̽��ϴ�.     ");
						SetNotice("�ٽ� �ϰ�����ø� R, �������� X�� �����ּ���~");
						while(1)
						{
							key=getch();
							if(key=='x' || key=='X')
							{
								SendMsg("NO");
								WSACleanup();
								main();
								return ;
							}
							else if(key=='r' || key=='R')
							{
								SendMsg("YES");
								if(strcmpindex(0,3,RecvMsg(),"YES")) Game();
								else
								{
									MessageBox(NULL,TEXT("��밡 ������ ���� �ʾҽ��ϴ�."),TEXT("�ȳ���"),MB_OK);
									WSACleanup();
									main();
								}
								return ;
							}
						}
					}
					else
					{
						SetNotice("���� ���� �Ҽ� �����ϴ�.");
					}
				}
				else if(key==SPACE)
				{
					if(mebingo[ny][nx])
					{
						SetNotice("������ ���� ���Դϴ�.    ");
					}
					else break;
				}
				else {
					gotoxy(4+tx*4+2,2+ty*2+1);
					SetColor(15);
					if(mebingo[ty][tx]) printf("��");
					else printf("%.2d",bingo[ty][tx]);
					gotoxy(4+nx*4+2,2+ny*2+1);
					SetColor(14);
					if(mebingo[ny][nx]) printf("��");
					else printf("%.2d",bingo[ny][nx]);
				}
			}
			mebingo[ny][nx]=true;
			sprintf(msg,"%d %d %d",bingo[ny][nx],ny,nx);
			SendMsg(msg);
			sscanf(RecvMsg(),"%d %d",&ny,&nx);
			enmbingo[ny][nx]=true;
			suv= suv==SERVER ? CLIENT : SERVER;
		}
		else
		{
			SetNotice("������ �Ͻ� �����Դϴ�.");
			char *rcv;
			rcv=RecvMsg();
			if(strcmpindex(0,5,rcv,"BINGO"))
			{
				SetNoticeSleep("�̷� ���̱���!..              ");
				SetNotice("�ٽ� �ϰ�����ø� R, �������� X�� �����ּ���~");
				while(1)
				{
					key=getch();
					if(key=='x' || key=='X')
					{
						SendMsg("NO");
						WSACleanup();
						main();
						return ;
					}
					else if(key=='r' || key=='R')
					{
						SendMsg("YES");
						if(strcmpindex(0,3,RecvMsg(),"YES")) break;
						else
						{
							MessageBox(NULL,TEXT("��밡 ������ ���� �ʾҽ��ϴ�."),TEXT("�ȳ���"),MB_OK);
							WSACleanup();
							main();
						}
						return ;
					}
				}
				lose++;
				break;
			}
			else
			{
				sscanf(rcv,"%d %d %d",&selected,&ny,&nx);
				if(enmbingo[ny][nx])
				{
					sysclear();
					printf("������ ���׸� ���˽��ϴ�.\n3���� �ǵ��� ���ϴ�.");
					Sleep(3000);
					WSACleanup();
					main();
				}
				enmbingo[ny][nx]=true;
				for(i=0;i<MAXXY;i++)
				{
					for(j=0;j<MAXXY;j++)
					{
						if(selected==bingo[i][j])
							break;
					}
					if(j!=MAXXY) break;
				}
				mebingo[i][j]=true;
				sprintf(msg,"%d %d",i,j);
				SendMsg(msg);
				suv = suv==SERVER ? CLIENT : SERVER;
			}
		}
	}
	Game();
}
bool strcmpindex(int start,int end,char *tmp1,char *tmp2)
{
	int i;
	for(i=start;i<end;i++) if(tmp1[i]!=tmp2[i]) return false;
	return true;
}
void Welcome()
{
	SetNoticeSleep("�ȳ��ϼ���!");
	SetNoticeSleep("�ֳܼ�Ʈ��ũ ���� CNBO�� ���Ű��� �������� ȯ���մϴ�.");
	SetNoticeSleep("���� ������ �ص帮�ڽ��ϴ�.");
	SetNoticeSleep("��������� �������� �ƽ���?");
	SetNoticeSleep("�� ������ 25������ ���ڸ� ����Ѵ�ϴ�.");
	SetNoticeSleep("�׸��� �� ������ �����̽��� �����ø� �˴ϴ�.");
	SetNoticeSleep("����~ ���� �ɼ������� �绡�� b�� �����ּžߵǿ�!.");
	SetNoticeSleep("�׷� �����Ͻʽÿ�!");
}
void SetNotice(char *s)
{
	SetColor(12);
	gotoxy(30-(strlen(s)/2),13);
	printf("%s",s);
}
void SetNoticeSleep(char *s)
{
	SetNotice(s);
	Sleep((strlen(s)*50)>=800 ? (strlen(s)*50) : 800);
	gotoxy(30-(strlen(s)/2),13);
	PrintFor(" ",strlen(s));
}
void SendMsg(char *s)
{
	int len=strlen(s);
	char *t;
	t=new char[len+2];
	sprintf(t,"%.2d%s",len,s);
	if(send(sClient,t,strlen(t),0)==SOCKET_ERROR) ErrorHading("������ ���������ϴ�.");
}
char *RecvMsg()
{
	char t[2];
	int len=0;
	char *rsult;
	if(recv(sClient,t,2,0)==SOCKET_ERROR)
		ErrorHading("������ ���������ϴ�.");
	len = atoi(t);
	rsult = new char[len];
	if(recv(sClient,rsult,len,0)==SOCKET_ERROR)
		ErrorHading("������ ���������ϴ�.");

	return rsult;
}
void DrawPan(int x,int y)
{
	int i=0,sum=0;
	for(i=0;i<MAXXY+1;i++)
	{
		if(i==0)
		{
			gotoxy(x,y+sum++);
			printf("����");
			PrintFor("����",MAXXY-1);
			printf("��");
		}
		else if(i==MAXXY)
		{
			gotoxy(x,y+sum++);
			printf("��");
			PrintFor("����",MAXXY);
			gotoxy(x,y+sum++);
			printf("����");
			PrintFor("����",MAXXY-1);
			printf("��");
		}
		else
		{
			gotoxy(x,y+sum++);
			printf("��");
			PrintFor("����",MAXXY);
			gotoxy(x,y+sum++);
			printf("����");
			PrintFor("����",MAXXY-1);
			printf("��");
		}
	}
}
void makebingo(int bingo[MAXXY][MAXXY],int x,int y)
{
	int i,j;
	bool is[25]={false};

	int key=0;
	sysclear();
	SetColor(12);
	DrawPan(x,y);
	SetColor(11);
	printf("\n������ : SPACE\n���� : ENTER");
	Random(bingo);
	SetColor(15);
	while(1)
	{
		for(i=0;i<MAXXY;i++)
		{
			for(j=0;j<MAXXY;j++)
			{
				gotoxy(x+j*4+2,y+i*2+1);
				printf("%.2d",bingo[i][j]);
			}
		}
		key=getch(); 
		switch(key)
		{
		case SPACE:
			Random(bingo);
			break;
		case ENTER:
			return ;
			break;
		}
	}
}
void Random(int t[MAXXY][MAXXY])
{
	bool p[MAXXY*MAXXY]={false};
	int a,i,j;
	for(i=0;i<MAXXY;i++)
	{
		for(j=0;j<MAXXY;j++)
		{
			a=(rand()%(MAXXY*MAXXY))+1;
			while(p[a-1]){
				a=(rand()%(MAXXY*MAXXY))+1;}
			t[i][j]=a;
			p[a-1]=true;
		}
	}
}
void PrintFor(char *s,int k)
{
	for(int i=0;i<k;i++) printf("%s",s);
}
bool chkbingo(bool bingo[MAXXY][MAXXY])
{
	int count=0,i,j;
	int garo[MAXXY]={0},saro[MAXXY]={0},daegak[2]={0};
	for(i=0;i<MAXXY;i++)
	{
		for(j=0;j<MAXXY;j++)
		{
			if(bingo[i][j]) garo[i]++;
			if(bingo[j][i]) saro[j]++;
		}
		if(bingo[i][i]) daegak[0]++;
		if(bingo[i][MAXXY-1]) daegak[1]++;
	}
	for(i=0;i<MAXXY;i++){
		if(garo[i]>=MAXXY) count++;
		if(saro[i]>=MAXXY) count++;}
	if(daegak[0]>=MAXXY) count++;
	if(daegak[1]>=MAXXY) count++;
	return count>=MAXXY;
}