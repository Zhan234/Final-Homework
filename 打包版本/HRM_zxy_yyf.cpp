#include<iostream>
#include<fstream>
#include<istream>
#include<cmath>
#include<algorithm>
#include<vector>
#include<cstring>
#include<windows.h>
#include <conio.h>
#include<iomanip>
#include<stdlib.h>
#include<Mmsystem.h>
#pragma comment (lib,"winmm.lib")
using namespace std;


void gotoxy(int x, int y);//按坐标输出
void Guangbiao(bool showFlag);//开关光标
void rgb(int type);
void makebox(int x, int y);
void embox(int x, int y);
void numbox(int x, int y, int num);
void basic(int n,string A,int *input, int insize);//按照空地数量生成空地,把A中数字初始化到inbox


void takebox(int *input,int insize);//输入传送带拿走方块
void putbox(vector<int> trueout, int outwei, int num);//输出传送带放下方块
void success(vector<int> trueout, int outwei);//输出传送带清空动画
void finishall();
void robot(int x, int y, int num);//生成向右的小人
void robotback(int x, int y,int num);//生成向左的小人
void moverobot(int a, int b,int num);//拿着num的小人，从a移动到b
void clearrobot(int x,int y);//用空格清空机器人所在位置

string SelectPage();//输入模式
void setting();//设置界面
void option();//开头界面

int  readnumber(string bazi);
int  checklevel(int outnum, int em ,int outsize, int trueliner, int onum ,int p , vector<int> trueout , int *tarout);
int  singlelevel(int om, int em,string fi,int n, int *input,int *tarout,int insize, int outsize, string info, vector<int> usable);
//从左向右分别是最大指令数，执行数文件路径，空地数，输入数组，输出数组，输入数组大小，输出数组大小，关卡提示
void levellas(int om, int em,string fi,int n, int *input,int *tarout,int insize, int outsize, string info, vector<int> usable);
int  restart(int p);
int  restart_fi(int p);

void writein();
void seti(int k);

int robdata[2]={0,-100};//记录机器人所在位置和手持数字的数组
int op[]={1,1};//开始界面的选择
int levelinfo[6]={1,0,0,0,0,0};//可用的关卡信息
int lenum=6;//关卡总数
float sp[9]={0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 5.0, 100};//分别对应0.5,1,1.5,2倍速
int scoi=2;//运行选项，保证每次打开set界面显示的速度为当前速度
float speed=sp[scoi];//运行速度
bool achievement=0;
int detect = 0;


int main(){
	ifstream fin;
	fin.open("levelinfo.txt");
	if(fin){
		int temp=0;
		while(!fin.eof()){
			int x=0;
			fin>>x;
			levelinfo[temp]=x;
			temp ++;
		}
	}
	else  writein();
	levelinfo[0]=1;
	fin.close();
	
	seti(1);//读入速度和是否首次通关信息
	system("mode con cols=100 lines=35");
	//finishall();
	Guangbiao(false);
	
	while(1){
		Guangbiao(false);
		PlaySound(TEXT("title.wav"),NULL,SND_FILENAME|SND_ASYNC|SND_LOOP);
		option();
		if(op[0]==0) return 0;
		int choice = op[0];
		
		if(op[1]==1){
			string fi = SelectPage();
			if(choice==1){
				int input[]=  {1,2};//输入
				int tarout[]= {1,2};//目标输出
				vector usable={1,1,0,0,0,0,0,0,1};//可用指令，第9个为关卡数
				string inf = "Please move every box to right side.";
				levellas(4,4,fi,0,input,tarout,2,2, inf,usable);
			}
			else if(choice==2){
				int input[]=  {3,9,5,1,-2,-2,9,-9};//输入
				int tarout[]= {-6,6,4,-4,0,0,18,-18};//目标输出
				vector usable={1,1,1,1,1,1,1,1,2};//可用指令，第9个为关卡数
				string inf ="  For each two box A and B,  output A-B and B-A. Repeat.";
				levellas(15,60,fi,3,input,tarout,8,8, inf,usable);
			}
			else if(choice==3){
				int input[]=  {6,2,7,7,-9,3,-3,-3};//输入
				int tarout[]= {7,-3};//目标输出
				vector usable={1,1,1,1,1,1,1,1,3};//可用指令，第9个为关卡数
				string inf ="  Take 2 box, if they are equal, output one of them,\n otherwise throw them away.Repeat.";
				levellas(11,35,fi,3,input,tarout,8,2, inf,usable);
			}
			else if(choice==4){
				int input[]=  {6,2,0,7,0,-2,0,0};//输入
				int tarout[]= {0,0,0,0};//目标输出
				vector usable={1,1,0,0,0,0,1,1,4};//可用指令，第9个为关卡数
				string inf ="  Just output 0";
				levellas(5,25,fi,0,input,tarout,8,4, inf,usable);
			}
			else if(choice==5){
				int input[]=  {1,3,2,5,4,-2,-3,-4};//输入
				int tarout[]= {2,1,0,5,4,3,2,1,0,4,3,2,1,0,1};//目标输出
				vector usable={1,1,1,1,1,1,1,1,5};//可用指令，第9个为关卡数
				string inf ="  Give you 1 and M. For M numbers later, eg:give 3,\n output 3,2,1,0. When finish, output a 1.";
				levellas(20,91,fi,3,input,tarout,8,15, inf,usable);
				if(!achievement && levelinfo[5]==1){
					achievement = 1;
					seti(2);
					finishall();
				}
			}
		}
	}
	return 0;
}

void gotoxy(int x, int y){
	COORD pos;
	pos.X = x - 1;
	pos.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Guangbiao(bool showFlag) {//光标不闪烁
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = showFlag; 
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

string SelectPage(){
	Guangbiao(false);//光标不显示
	system("cls");
	gotoxy(30, 8);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
	cout << "Please select your instructions input method";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
	
	gotoxy(37, 14);cout <<"+---------+"<<"      "<<"+---------+";
	gotoxy(37, 15);cout <<"| By File |"<<"      "<<"| By Type |";
	gotoxy(37, 16);cout <<"+---------+"<<"      "<<"+---------+";
	gotoxy(42, 13);cout <<"↓";
	int currentX=53;
	int Select;
	while(true){
		char keyPressed = _getch();
		if (currentX == 53) {
			if (keyPressed == 'D' || keyPressed == 'd') {
				currentX = 69;
				gotoxy(42,13);printf(" ");
				gotoxy(59,13);
				cout <<"↓";
			}
		}
		if (currentX == 69) {
			if (keyPressed == 'A' || keyPressed == 'a') {
				currentX = 53;
				gotoxy(59, 13);printf(" ");
				gotoxy(42, 13);
				cout <<"↓";
			}
		}
		if(keyPressed ==13 && currentX ==53){
			Select = 0; break;
		} 
		if(keyPressed ==13 && currentX ==69){
			Select = 1; break;
		}
	}
	system("cls");
	if(Select==0){
		string fileway;
		gotoxy(39, 10);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "Please enter the file location." << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		gotoxy(34, 15);
		cout<<"________________________________________";
		gotoxy(34, 14);
		cin.sync();
		Guangbiao(true);
		getline(cin,fileway);
		Guangbiao(false);
		return fileway;//选择文件输入return 文件路径；
	}   
	else return "Null";//如果选择键入return Null
}

void makebox(int x, int y){
	gotoxy(x,y);    printf("+---+");
	gotoxy(x,y+1);	printf("| X |");
	gotoxy(x,y+2);	printf("+---+");
}

void numbox(int x, int y, int num){
	makebox(x,y);
	gotoxy(x+1,y+1);
	if(num==-100)  printf(" X ");
	else  printf("%2d",num);
}

void takebox(int *input,int insize){
	if(insize==0)
		return;
	int coun=insize-1;
	for(int i=0;i<insize-1;i++)
		input[i]=input[i+1];
	input[insize-1]=-100;
	
	for(int p=0;p<insize;p++)
		if(input[p]==-100) {
		coun = p;
		break;
	}
	
	for(int p=0;p<3;p++){//覆盖第一个
		gotoxy(5,5+p);printf("     ");
	}
	for(int p=8;p>=5;p--){
		int x=coun;
		if(p+3*coun>=23)  x=(23-p)/3+1;
		for(int q=0;q<x;q++)
			numbox(5,p+3*q,input[q]);
		int si=23;
		if(p+3*coun<=23) si=p+3*coun;
		for(int p=si;p<=28;p++){
			gotoxy(5,p);printf("     ");
		}
		Sleep((int)150/speed);
	}
}

void putbox(vector<int> trueout, int outwei, int num){
	for(int p=5;p<=7;p++){
		gotoxy(51,p); printf("     ");
		for(int q=0;q<min(outwei,7);q++)
			numbox(51,p+1+3*q,trueout[outwei-1-q]);
		for(int p=23;p<=28;p++){
			gotoxy(51,p);printf("     ");
		}
		Sleep((int)150/speed);
	}
	numbox(51,5,num);
	for(int p=0;p<3;p++){
		gotoxy(43,5+p);printf("     ");
	}
}

void success(vector<int> trueout, int outwei){
	for(int p=5;p<=22;p++){
		gotoxy(51,p); printf("     ");
		int x = (p+3*outwei<=23)? outwei:(23-p)/3+1;
		for(int q=0;q<x;q++){
			numbox(51,p+1+3*q,trueout[outwei-1-q]);
		}
		for(int p=23;p<=28;p++){
			gotoxy(51,p);printf("     ");
		}
		rgb(p);
	}
}

void finishall(){
	PlaySound(NULL,NULL,SND_PURGE);
	PlaySound(TEXT("achieve.wav"),NULL,SND_FILENAME|SND_ASYNC|SND_LOOP);
	system("cls");
	Guangbiao(false);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN);
	gotoxy(41,3); printf("Human Resource Machine");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	for(int p=0;p<=15;p++){//生成输出传送带
		gotoxy(30,7+p);
		printf("|                                            |");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
	string lis[]=
	{"       Congratulations on Clearance!      ",
     "                                          ",
	 "+----------------------------------------+",
	 "|       /___ /    +---+    \\ ___\\        |",
	 "|       (^_^)     |WIN|     (^_^)        |",
	 "|        |#|      +---+      |#|         |",
	 "|        / |        ∞        | \\         |",
	 "+----------------------------------------+",
	 "                                          ",
	 "        Producer    : Zhan xiaoyu         ",
	 "  ___                 Yuan yefeng.        ",
	 " (-_-)|                                   ",
	 " /#/--| Page design : Yuan yefeng.        ",
	 " | |  |                              ___  ",
	 "        Testing     : Yuan yefeng.  (^_^) ",
	 "                                    /|#|\\",
	 "        Robot design: Zhan xiaoyu.   | |  ",
	 "+---+                                     ",
	 "| 6 |   Music       : Zhan xiaoyu.        ",
	 "+---+                                     ",
	 "        Animation   : Zhan xiaoyu.|  ___  ",
	 "                                  | (0.0) ",
	 "        Structure   : Zhan xiaoyu.|  |?| ",
	 "                                     / \\ ",
	 "        Optimizing  : Zhan xiaoyu.       ",
	 "                                         ",
	 "      Refer to Human Resoure Machine     ",
	 "-----------------------------------------",
	 "                                         ",
	 "                                         ",
     "                                         ",
	 "     +------------------------------+    ",
	 "     |     Thanks for Playing !     |    ",
	 "     +------------------------------+    ",
	 "                                         ",
	 "                   END                   ",
	 "-----------------------------------------"};//37行
	
	for(int i=22;i>=7;i--){
		for(int j=0;j<=22-i;j++){
			gotoxy(32,i+j);cout<<lis[j];
		}
		Sleep(500);
	}
	
	for(int i=9;i<29;i++){
		gotoxy(32,15);printf("                                          ");
		for(int j = i ; j<i+8 ;j++){
			gotoxy(32,15+j-i);cout<<lis[j];
		}
		Sleep(500);
	}
	gotoxy(32,24); printf("Press any keys to contine...");
	while(true){
		if(_kbhit()){
			_getch();
			break;
		}
	}
	return;
}

void embox(int x, int y){
	gotoxy(x,y);    printf("·---·");
	gotoxy(x,y+1);	printf("|   |");
	gotoxy(x,y+2);	printf("·---· ");
}

void basic(int n,string A, int *input, int insize){
	gotoxy(1,1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout<<"Level imformation:";
	gotoxy(91,1);printf("Modify:");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	for(int p=0;p<=17;p++){//输出传送带,最下侧22行
		gotoxy(4,5+p);
		printf("|     |");
	}
	gotoxy(7,4); printf("IN");
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	for(int p=0;p<=17;p++){//生成输出传送带
		gotoxy(50,5+p);
		printf("|     |");
	}
	gotoxy(52,4);printf("OUT");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
	
	int x=(insize<6)?insize:6;
	for(int p=0;p<x;p++)
		numbox(5,5+3*p,input[p]);
	gotoxy(1,2);cout<<A;
	for(int p=0;p<n;p++){//生成空地
		embox(19+6*p,12);
		gotoxy(21+6*p,15);
		printf("%d",p);
	}
	
	
	for(int p=1;p<=28;p++){
		gotoxy(59,p);
		printf("|");
		if(p<=6){
			gotoxy(89,p);printf("|");
		}
	}

	gotoxy(91,2);printf("1.del X:Delet line X");
	gotoxy(91,3);printf("2.ins X:Insert after line X");
	gotoxy(91,4);printf("1.end  :Start now");
	gotoxy(62,7);
	printf("================== CODE ==============================");
	
}

void robot(int x, int y,int num){
	for(int p=0;p<3;p++){
		gotoxy(x+1,y+p);printf("     ");
	}
	if(num!=-100){
		makebox(x+1,y);
		gotoxy(x+2,y+1);
		printf("%2d",num);
	}
	gotoxy(x,y+3);printf("/___ /");
	gotoxy(x,y+4);printf("(^_^) ");
	gotoxy(x,y+5);printf(" |#|  ");
	gotoxy(x,y+6);printf(" / |  ");
}

void robotback(int x, int y,int num){
	for(int p=0;p<3;p++){
		gotoxy(x+1,y+p);printf("     ");
	}
	if(num!=-100){
		makebox(x,y);
		gotoxy(x+1,y+1);
		printf("%2d",num);
	}
	gotoxy(x,y+3);printf("\\ ___\\");
	gotoxy(x,y+4);printf(" (^_^)");
	gotoxy(x,y+5);printf("  |#| ");
	gotoxy(x,y+6);printf("  | \\ ");
}

void clearrobot(int x,int y){
	for(int i=0;i<=6;i++){
		gotoxy(x,y+i);
		printf("      ");
	}
}

void moverobot(int a, int b,int num){
	if(a<b){
		for(int i=0;i<=(b-a)*6;i++){
			clearrobot(11+6*a+i,5);
			robot(12+6*a+i,5,num);
			Sleep((int)70/speed);
		}
	}
	if(a>b){
		clearrobot(12+6*a,5);
		for(int i=(a-b)*6;i>=0;i--){
			clearrobot(14+6*b+i,5);
			robotback(13+6*b+i,5,num);
			Sleep((int)70/speed);
		}
	}
	if(a==b){
		clearrobot(11+6*a,5);
		clearrobot(13+6*a,5);
		robot(12+6*a,5,num);
	}
}

int restart(int p){
	gotoxy(81,p+1);printf("Restart this level or not?(Y/N)");
	string ch;
	Guangbiao(true);
	gotoxy(81,p+2);cin>>ch;
	Guangbiao(false);
	if(ch=="Y") return -1;//表示重开
	else return 0;//表示结束
}

int restart_fi(int p){
	gotoxy(64,p+1);printf("Restart this level or not?(Y/N)");
	string ch;
	Guangbiao(true);
	gotoxy(64,p+2);cin>>ch;
	Guangbiao(false);
	if(ch=="Y") return -1;//表示重开
	else return 0;//表示结束
}

int readnumber(string bazi){
	string num="";
	int num2=0;
	if(bazi=="") return -2;
	for(int i=0;i<(int)bazi.size();i++){
		if(bazi[i]>'9'||bazi[i]<'0'||(bazi[0]=='0'&&i>=1))
			return -2;
		else num = num + bazi[i];
	}
	
	int big = (int)num.length();
	for(int i=1;i<=big;i++){
		num2 += pow(10,big-i)*(int)((num[i-1])-48);
	}
	return num2;
}

void setting(){
	system("cls");
	int coi=1;char ch;
	int u=4;
	int b[4][2]={{0,0},{20+u,13},{46+u,13},{72+u,13}};
	//速度为第二个
	gotoxy(b[coi][0],b[coi][1]);printf("↓");
	gotoxy(36,11);printf("Use AD and enter to select...");
	gotoxy(36,12);printf("   Press esc to go back...   ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	gotoxy(47,9);printf("SETTING");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN);
	gotoxy(40,8); printf("Human Resource Machine");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
	gotoxy(11+u, 14);cout <<"+-----------------+      +-------------------+      +-------------------+";
	gotoxy(11+u, 15);cout <<"|  Empty Archive  |      | Change Speed      |      | Step Tracking     |";
	gotoxy(11+u, 16);cout <<"+-----------------+      +-------------------+      +-------------------+";
	gotoxy(54+u,15);cout<<"x";
	if(sp[scoi]!=100){
		gotoxy(51+u,15);cout<<fixed<<setprecision(1)<<sp[scoi];
	}
	else{
		gotoxy(51+u,15);cout<<fixed<<setprecision(0)<<sp[scoi];
	}
	gotoxy(80+u,15);
	if(detect==0)printf("N");
	else printf("Y");
	
	while(1){
		if(_kbhit())
		{
			ch = _getch();
			if (ch == 27){
				seti(2);
				return;
			}
				
				
			if(ch == 97 || ch == 100 || ch == '\r'){
				gotoxy(b[coi][0],b[coi][1]);printf(" ");
				if     (ch == 97)  coi--; //左
				else if(ch == 100) coi++; //右
				else if(ch=='\r'){
					if(coi==1){
						char tem;
						gotoxy(14+u,17);printf("Are you sure?(Y/N)");
						Guangbiao(true);
						gotoxy(14+u,18);cin>>tem;
						Guangbiao(false);
						if(tem == 'Y'){
							for(int i=1;i<lenum;i++) levelinfo[i]=0;
							scoi = 2;
							speed = sp[scoi];
							achievement = 0;
							seti(2);
							writein();
							return;
						}
						gotoxy(14+u,17);printf("                             ");
						gotoxy(14+u,18);printf("                               ");
					}
					else if(coi==2){
						gotoxy(50+u,15);cout<<'<';gotoxy(55+u,15);cout<<">";
						while(1){
							ch = _getch();
							if(ch == 97 || ch == 100 || ch == '\r'){
								if     (ch == 97)  scoi--; //左
								else if(ch == 100) scoi++; //右
								if(scoi<1) scoi=1;
								if(scoi>8) scoi=8;
								if(sp[scoi]!=100){
									gotoxy(41+u,17);printf("               ");
									gotoxy(51+u,15);cout<<fixed<<setprecision(1)<<sp[scoi];
								}		
								else{
									gotoxy(51+u,15);cout<<fixed<<setprecision(0)<<sp[scoi];
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
									gotoxy(41+u,17);printf("Are you crazy?!");
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
										FOREGROUND_GREEN | FOREGROUND_BLUE);
								}
								if(ch=='\r'){
									gotoxy(50+u,15);cout<<' ';gotoxy(55+u,15);cout<<" ";
									speed = sp[scoi];
									break;
								}
							}
						}
					}
					else if(coi==3){
						gotoxy(79+u,15);cout<<'<';gotoxy(81+u,15);cout<<">";
						while(1){
							ch = _getch();
							if(ch == 97 || ch == 100 || ch == '\r'){
								if     (ch == 97)  detect--; //左
								else if(ch == 100) detect++; //右
								if(detect<0) detect=0;
								if(detect>1) detect=1;
								gotoxy(80+u,15);
								if(detect==0)printf("N");
								else printf("Y");
								if(ch=='\r'){
									gotoxy(79+u,15);cout<<' ';gotoxy(81+u,15);cout<<" ";
									break;
								}
							}
						}
					}
				}
				if(coi<1) coi=1;
				if(coi>3) coi=3;
				gotoxy(b[coi][0],b[coi][1]);printf("↓");
			}
		}
	}
}

void option(){
	system("cls");
	gotoxy(41,10);printf("Use AWSD to select...");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
	gotoxy(40,8); printf("Human Resource Machine");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
	string c[3]={"-Set","-Help","-Quit"};
	for(int i=0;i<5;i++){
		if(levelinfo[i]!=1){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			numbox(33+8*i,13,i+1);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
				FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else numbox(33+8*i,13,i+1);
	}
	for(int i=0;i<3;i++){
		gotoxy(41+8*i,19);
		cout<<c[i];
	}
	char ch;
	
	int a[6][3][2]={{{0,0},{0,0},{0,0}},
		{{0,0},{35,12},{0,0}},
		{{0,0},{43,12},{43,18}},
		{{0,0},{51,12},{51,18}},
		{{0,0},{59,12},{59,18}},
		{{0,0},{67,12},{0,0}}};
	gotoxy(a[op[0]][op[1]][0],a[op[0]][op[1]][1]);printf("↓");
	while (true)
	{
		if(_kbhit())
		{
			ch = _getch(); 
			if (ch == 27){//esc返回特殊值
				op[0]=0;op[1]=0;
				return;
			}
			
			if (ch == 97 || ch == 100 || ch == '\r' || ch == 115 || ch == 119)
			{
				gotoxy(a[op[0]][op[1]][0],a[op[0]][op[1]][1]);printf(" ");
				if     (ch == 97)  op[0]--; //左
				else if(ch == 100) op[0]++; //右
				else if(ch == 115) op[1]++; //下
				else if(ch == 119) op[1]--; //上
				
				if (op[0] < 1)	op[0]=1;
				if (op[0] > 5)  op[0]=5;
				if (op[1] < 1)  op[1]=1;
				if (op[1] > 2)  op[1]=2;
				if(op[1]==2){
					if(op[0] < 2)	op[0] = 2;
					if(op[0] > 4)   op[0] = 4;
				}
				gotoxy(a[op[0]][op[1]][0],a[op[0]][op[1]][1]);
				printf("↓");
				if (ch == '\r'){
					if(op[1]==1){
						if(levelinfo[op[0]-1]==1)	return;
						else{
							gotoxy(a[op[0]][op[1]][0]-2,a[op[0]][op[1]][1]+4);
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
							printf("Lock");
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
							FOREGROUND_GREEN | FOREGROUND_BLUE);
							Sleep(500);
							option();
							return;
						}
					} 
					else{
						if(op[0]==2){
							setting();
							option();
							return;
						}
						if(op[0]==3){
							system("cls");
							
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
								FOREGROUND_GREEN);
							gotoxy(40,8); printf("Human Resource Machine");
							gotoxy(21,16);printf("            Feedback E-mail : 17319255985@163.com               ");
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
								FOREGROUND_GREEN | FOREGROUND_BLUE);
							gotoxy(41,9);printf("Press esc to quit...");
							gotoxy(21,10);printf("+--------------------------------------------------------------+");
							gotoxy(21,11);printf("|    This is a game that you can give instructions to a robot. |");
							gotoxy(21,12);printf("| At each level, your boss gives the robot a task. You program |");
							gotoxy(21,13);printf("| the robot to automate its tasks. If you succeed, you'll be   |");
							gotoxy(21,14);printf("| promoted to the next floor of this huge office building.     |");
							gotoxy(21,15);printf("+--------------------------------------------------------------+");
							while(true){
								ch = _getch();
								if(ch == 27){
									op[0]=1;op[1]=1;
									option();
									return;
								}
							}
						}
						if(op[0]==4){
							op[0]=0;op[1]=0;
							system("cls");
							return;
						}
					}
				}
			}
		}
	}
}

void writein(){
	levelinfo[0]=1;
	ofstream fout;
	fout.open("levelinfo.txt");
	fout<<levelinfo[0];
	for(int i=1;i<lenum;i++)
		fout<<endl<<levelinfo[i];
	fout.close();
}

void seti(int k){
	if(k==1){
		ifstream setin;
		setin.open("setmem.txt");
		setin>>scoi;
		speed=sp[scoi];
		setin>>achievement;
		setin.close();
		return;
	}
	if(k==2){
		ofstream setof;
		setof.open("setmem.txt");
		setof<<scoi<<endl<<achievement;
		setof.close();
		return;
	}
	
}

void login(string fi,string se,int *A,int *B,int i,int x){
	if(fi=="inbox" && se=="&!?"){
		A[i]=1;B[i]=-1;
	}
	else if(fi=="outbox" && se=="&!?"){
		A[i]=2;B[i]=-1;
	}
	else if(fi=="add" && se!="&!?"){
		A[i]=3;B[i]=x;
	}
	else if(fi=="sub" && se!="&!?"){
		A[i]=4;B[i]=x;
	}
	else if(fi=="copyto" && se!="&!?"){
		A[i]=5;B[i]=x;
	}
	else if(fi=="copyfrom" && se!="&!?"){
		A[i]=6;B[i]=x;
	}
	else if(fi=="jump" && se!="&!?"){
		A[i]=7;B[i]=x;
	}
	else if(fi=="jumpifzero" && se!="&!?"){
		A[i]=8;B[i]=x;
	}
	else{
		A[i]=-2;B[i]=x;
	}
	return;
}

int ordercheck(int onum,int om){
	cin.sync();
	if(onum>om){
		string temp;
		int tem = onum;
		Guangbiao(false);
		for(int i=8;i<=28;i++){gotoxy(62,i);printf("                                                 ");}
		
		gotoxy(64,8);printf("  I doubt your ability, because there ");
		gotoxy(64,9);printf("is NO need for so such instructions!");
		gotoxy(64,11);printf("  Give me a proper instructions-number");
		gotoxy(64,12);printf("and PROVE your competence!");
		Guangbiao(true);
		while(1){
			gotoxy(66,13);printf("                                                 ");
			gotoxy(66,13);getline(cin,temp);
			tem = readnumber(temp);
			if(tem<=0) tem = onum;
			if(tem>0 && tem<onum) break;
		}
		Guangbiao(false);
		for(int i=8;i<=28;i++){gotoxy(62,i);printf("                                                 ");}
		return tem;
	}
	return onum;
}

void rgb(int type){
	if(type%3==0){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		gotoxy(18,19); printf("Success!");
		Sleep(70);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		gotoxy(18,19); printf("Success!");
		Sleep(70);
		
	}
	
	if(type%3==1){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_GREEN);
		gotoxy(18,19); printf("Success!");
		Sleep(70);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_BLUE);
		gotoxy(18,19); printf("Success!");
		Sleep(70);
		
	}
	
	if(type%3==1){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE |FOREGROUND_BLUE);
		gotoxy(18,19); printf("Success!");
		Sleep(70);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		gotoxy(18,19); printf("Success!");
		Sleep(70);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
	gotoxy(18,19); printf("Success!");
}

int checklevel(int outnum, int em ,int outsize, int trueliner, int onum ,int p , vector<int> trueout , int *tarout){
	int check=0;
	for(int j=0;j<outnum;j++){//检测是否满足目标输出，若不满足则循环中断
		if(tarout[j]!=trueout[j]){
			check ++;
			break;
		}
	}
	if(check==0 && outnum==outsize && em>=trueliner){//判定过关条件
		gotoxy(18,19); printf("Success!");
		gotoxy(18,20); printf("Total instructions:%3d",onum);
		gotoxy(18,21); printf("Total  executions :%3d",trueliner);
		success(trueout,outnum);
		gotoxy(18,23); printf("Press any keys to contine...");
		while(true){
			if(_kbhit()){
				_getch();
				break;
			}
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_GREEN | FOREGROUND_BLUE);
		return 2;
	}
	else{
		gotoxy(18,19); printf("Fail!");
		gotoxy(18,20); printf("Maybe faster and more precise!");
		gotoxy(18,21); printf("Total instructions:%3d",onum);
		gotoxy(18,22); printf("Total  executions :%3d",trueliner);
		return restart(min(p,20));
	}
}

int  singlelevel(int om, int em, string fil,int n, int *input2,int *tarout,int insize, int outsize, string info, vector<int> usable){
	PlaySound(NULL,NULL,SND_PURGE);
	PlaySound(TEXT("inlevel.wav"),NULL,SND_FILENAME|SND_ASYNC|SND_LOOP);
	cin.sync();
	Guangbiao(false);
	string order[25]={""};
	int * input = new int [insize];
	for(int i=0;i<insize;i++)
		input[i]=input2[i];
	
	system("mode con cols=120 lines=28");
	robdata[0]=0; robdata[1]=-100;
	vector<int> trueout;
	basic(n,info,input,insize);
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	gotoxy(18,16);printf("Use ≤ %d instructions.",om);
	gotoxy(18,17);printf("Finish in %d executions.",em);
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	gotoxy(61,1); printf("Usable orders:");
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
	
	int coun=0;
	
	for(int p=0;p<8;p++){
		if(usable[p]==1){
			coun ++;
			int c=63,b=1+coun;
			if(coun>4){c=76;b=coun-3;}
			gotoxy(c,b);printf("%d.",coun);
			gotoxy(c+2,b);
			if(p==0) printf("inbox");
			if(p==1) printf("outbox");
			if(p==2) printf("add");
			if(p==3) printf("sub");
			if(p==4) printf("copyto");
			if(p==5) printf("copyfrom");
			if(p==6) printf("jump");
			if(p==7) printf("jumpifzero");
		}
	}

	int A[100]={0};//将指令转换成数组
	memset(A, -1, sizeof A); 
	int B[100]={0};//A是文字种类，B是操作数
	memset(B, -1, sizeof B); 
	int C[4]={-100,-100,-100,-100};//用于记录空地中的数字类型
	
	robot(11,5,-100);
	string temp="";
	int onum=0;//指令总数阅读
	
	if(fil=="Null"){
		
		//获得总指令行数，打印指令行数字
		gotoxy(62,8);
		printf("How many orders will be included?");
		Guangbiao(true);
		gotoxy(63,9);
		cin.sync();
		getline(cin,temp);
		onum = readnumber(temp);
		if(onum<0){
			gotoxy(63,10);printf("Is that a number ?!");
			return restart_fi(10);
		}
		onum = ordercheck(onum,om);
		cin.sync();
		gotoxy(62,8);printf("                                 ");
		gotoxy(63,9);printf("   ");
		for(int i=0;i<onum;i++){//打印指令行数
			gotoxy(62,8+i);
			printf("%2d",i+1);
		}
		gotoxy(65,8);
		int gun = 0;
		bool insert = 0;
		Guangbiao(true);
		for(int i=1;i<=onum;i++){//将指令转化为数组储存
			int x=-4;//单步操作数
			gotoxy(65,7+i);
			getline(cin,order[i]);//读取整行

			int check=0;
			while(check<(int)order[i].length()){
				if(order[i][check]==' ') break;
				check += 1;
			}
			string fi(order[i],0,check);
			string se="";
			if(check==(int)order[i].length()) {
				se="&!?";
				x=-3;
			}
			else {
				string se0(order[i],check+1);
				string xp ="";int cp=0;
				while(cp<(int)se0.length()){
					if(se0[cp]!=' ')
						xp=xp+se0[cp];
					cp += 1;
				}
				se = xp;
				if(se.size()>2 || se.size()==0)  x=-2;
				else  x=readnumber(se);
			}//以空格划分字符串，如果有空格则记录后面那串，没空格则se为&!?
			
			if(fi=="del" && se!="&!?"){
				if(x<1 || x>i-1){
					gotoxy(75,7+i);printf("<- Can't find the line!");
					Sleep(400);
					gotoxy(65,7+i);printf("                                      ");
					i--;
					continue;
				}
				for(int k=x+1 ; k<i ;k++){
					gotoxy(65,7+k-1);printf("                             ");
					gotoxy(65,7+k-1);cout<<order[k];
				}
				for(int k = x;k<i;k++){
					order[k] = order[k+1];
					A[k] = A[k+1];
					B[k] = B[k+1];
				}
				gotoxy(65,7+i-1);printf("                               ");
				gotoxy(65,7+i);printf("                               ");
				i -= 2;
			}
			else if(fi =="end" && se =="&!?"){
				onum = i-1;
				Guangbiao(false);
				for(int k=i+7;k<=28;k++){
					gotoxy(62,k);printf("  ");
				}
				for(int k=7+i;k<=28;k++){
					gotoxy(62,k);printf("                                        ");
				}
				break;
			}
			else if(fi=="ins" && se!="&!?"){
				if(gun!=0){
					gotoxy(75,7+i);printf("Haven't finish this insert!");
					Sleep(800);
					gotoxy(65,7+i);printf("                                        ");
					i--;
					continue;
				}
				if(x<1 || x>i-1){
					gotoxy(75,7+i);printf("<- Can't find the line!");
					Sleep(800);
					gotoxy(65,7+i);printf("                                        ");
					i--;
					continue;
				}
				insert = 1;
				gun = i;
				for(int k = x+1;k<i;k++){
					gotoxy(65,7+k+1);printf("                                 ");
					gotoxy(65,7+k+1);cout<<order[k];
				}
				for(int k = i;k>=x+2;k--){
					order[k] = order[k-1];
					A[k] = A[k-1];
					B[k] = B[k-1];
				}
				gotoxy(65,7+x+1);printf("                                 ");
				cin.sync();
				i = x;
				continue;
			}
			
			else if(fi=="inbox" && se=="&!?"){
				A[i]=1;B[i]=-1;
			}
			else if(fi=="outbox" && se=="&!?"){
				A[i]=2;B[i]=-1;
			}
			else if(fi=="add" && se!="&!?"){
				A[i]=3;B[i]=x;
			}
			else if(fi=="sub" && se!="&!?"){
				A[i]=4;B[i]=x;
			}
			else if(fi=="copyto" && se!="&!?"){
				A[i]=5;B[i]=x;
			}
			else if(fi=="copyfrom" && se!="&!?"){
				A[i]=6;B[i]=x;
			}
			else if(fi=="jump" && se!="&!?"){
				A[i]=7;B[i]=x;
			}
			else if(fi=="jumpifzero" && se!="&!?"){
				A[i]=8;B[i]=x;
			}
			else{
				A[i]=-2;B[i]=x;
			}
			if(insert==1){
				i = gun;
				gun = 0;
				insert = 0;
			}
		}
	}
	
	else{
		gotoxy(62,8); printf("  The input file consists of M+1 lines,with M ");
		gotoxy(62,9); printf("on first line shows the number of instructions.");
		gotoxy(62,10);printf("Lines 2 to M+1,each containing one instruction.");
		gotoxy(62,12);printf("  When finished the file, ");
		gotoxy(62,13);printf("save it and press Enter to contine...");
		while(1){
			char ch = _getch();
			if(ch=='\r') break;
		}
		for(int i=8;i<=13;i++){gotoxy(62,i);printf("                                               ");}//覆盖提示
		ifstream fin;
		ifstream fin_1;
		int lines=0;
		string tmp;
		fin.open(fil);
		fin_1.open(fil,ios::in);//ios::in 表示以只读的方式读取文件
		while(getline(fin_1,tmp,'\n')) lines++;
		fin_1.close();
		if(fin){
			string k;
			getline(fin,k);
			string xp = "";int cp=0;
			while(cp<(int)k.length()){
				if(k[cp]!=' ')
					xp=xp+k[cp];
				cp += 1;
			}
			onum = readnumber(xp);
			if(onum == -2){//第一行数字不合法
				gotoxy(62,8); printf("  The First line in your file ");
				gotoxy(62,9); printf("isn't a proper number which is");
				gotoxy(62,10);printf("expected to show the number of" );
				gotoxy(62,11);printf("instructions!");
				return restart_fi(11);
			}
			
			onum = ordercheck(onum,om);
			
			for(int i=0;i<onum;i++){//打印指令行数
				gotoxy(62,8+i);
				printf("%2d",i+1);
			}
			
			bool stop=0;
			
			for(int i=1;i<=onum;i++){//将指令转化为数组储存
				int x=-4;//单步操作数
				string order;
				
				getline(fin,order);//文件读取整行
				if(order=="" || fin.eof()){
					stop=1;
					break;
				}
				gotoxy(65,7+i);cout<<order;
				int check=0;
				while(check<(int)order.length()){
					if(order[check]==' ') break;
					check += 1;
				}
				string fi(order,0,check);
				string se="";
				if(check==(int)order.length()) {
					se="&!?";
					x=-3;
				}
				else {
					string se0(order,check+1);
					string xp ="";int cp=0;
					while(cp<(int)se0.length()){
						if(se0[cp]!=' ')
							xp=xp+se0[cp];
						cp += 1;
					}
					se = xp;
					if(se.size()>2 || se.size()==0)  x=-2;
					else  x=readnumber(se);
				}//以空格划分字符串，如果有空格则记录后面那串，没空格则se为&!?
				
				login(fi,se,A,B,i,x);
			}
			if(lines!=onum+1 || stop==1){
				fin.close();
				gotoxy(62,8+onum); printf("  The number of instructions does");
				gotoxy(62,9+onum); printf("not match the number of lines. ");
				gotoxy(62,10+onum);printf("Check for extra line breaks." );
				return restart_fi(10+onum);
			}
			fin.close();
		}
		else{
			fin.close();
			gotoxy(62,8); printf("Can't find the file!");
			gotoxy(62,9);cout<<"In "<<fil;
			return restart_fi(9);
		}
	}
	
	Guangbiao(false);
	
	int liner=1;//记录当前指令在第几行
	int outnum=0;
	int trueliner = 0;
	while(1){
		int p=7+liner;
		bool che=(B[liner]>=n);
		gotoxy(61,p);
		printf(">");//显示操作行
		
		if(liner == onum+1){
			return checklevel(outnum, em ,outsize, trueliner, onum ,p , trueout ,tarout);
		}
		
		else if(A[liner]>0 && A[liner]<9 && usable[A[liner]-1]==0){
			gotoxy(78,p);printf("<- An unusable order!");
			return restart(p);
		}
		
		else if(A[liner]==-2 || B[liner]<-1){
			gotoxy(78,p);printf("<- An illegal line!");
			gotoxy(81,p+1);
			int ci=0;
			if(B[liner]==-4 || B[liner]==-2)  {printf("Robot can't move to there");ci++;}
			if(B[liner]==-3)  {printf("What are you talking about?");ci++;}
			string ch;
			return restart(p+ci);
			
		}
		else if(A[liner]==1 && usable[0]==1){
			if(insize==0){
				gotoxy(78,p);printf("<- No box can be taken!");
				return restart(p);
			}
			robdata[1] = -100;
			moverobot(robdata[0],0,robdata[1]);
			robdata[0]=0;
			robdata[1] = input[0];
			moverobot(robdata[0],0,robdata[1]);
			takebox(input,insize);
			insize--;
			Sleep((int)500/speed);
		}
		else if(A[liner]==2 && usable[1]==1){
			moverobot(robdata[0],5,robdata[1]);
			if(robdata[1]==-100){
				gotoxy(78,p);printf("<- Robot's hand is empty!");
				return restart(p);
			}
			putbox(trueout,outnum,robdata[1]);
			trueout.push_back(robdata[1]);
			outnum +=1;
			robdata[0]=5;
			robdata[1]=-100;
			if(outnum==outsize){
				return checklevel(outnum, em ,outsize, trueliner+1, onum ,p , trueout ,tarout);
			} 
			
			Sleep((int)500/speed);
		}
		else if(A[liner]==3 && usable[2]==1){
			moverobot(robdata[0],min(B[liner]+1,n),robdata[1]);
			if(che){
				gotoxy(78,p);printf("<- Out of range!");
				return restart(p);
			}
			if(C[B[liner]]==-100){
				gotoxy(78,p);printf("<- The box is empty!");
				return restart(p);
			}
			
			if(robdata[1]==-100){
				gotoxy(78,p);printf("<- Robot's hand is empty!");
				return restart(p);
			}
			robdata[1]+=C[B[liner]];
			robdata[0]=B[liner]+1;
			moverobot(robdata[0],robdata[0],robdata[1]);
			Sleep((int)500/speed);
		}
		else if(A[liner]==4 && usable[3]==1){
			moverobot(robdata[0],min(B[liner]+1,n),robdata[1]);
			if(C[B[liner]]==-100  || che){
				gotoxy(78,p);printf("<- This box is empty!");
				return restart(p);
			}
			if(robdata[1]==-100){
				gotoxy(78,p);printf("<- Robot's hand is empty!");
				return restart(p);
			}
			robdata[1]-=C[B[liner]];
			robdata[0]=B[liner]+1;
			moverobot(robdata[0],robdata[0],robdata[1]);
			Sleep((int)500/speed);
		}
		else if(A[liner]==5 && usable[4]==1){
			moverobot(robdata[0],min(B[liner]+1,n),robdata[1]);
			robdata[0]=B[liner]+1;
			if(che){
				gotoxy(78,p);printf("<- Can't find this box!");
				return restart(p);
			}
			if(robdata[1]==-100){
				gotoxy(78,p);printf("<- Robot's hand is empty!");
				return restart(p);
			}
			C[B[liner]]=robdata[1];
			numbox(19+6*B[liner],12,C[B[liner]]);
			moverobot(robdata[0],B[liner]+1,robdata[1]);
			Sleep((int)500/speed);
		}
		else if(A[liner]==6 && usable[5]==1){
			moverobot(robdata[0],min(B[liner]+1,n),robdata[1]);
			if(che){
				gotoxy(78,p);printf("<- Can't find this box!");
				return restart(p);
			}
			if(C[B[liner]]==-100){
				gotoxy(78,p);printf("<- Nothing on this box!");
				return restart(p);
			}
			robdata[0]=B[liner]+1;
			robdata[1]=C[B[liner]];
			moverobot(robdata[0],B[liner]+1,robdata[1]);
		}
		else if(A[liner]==7 && usable[6]==1){
			if(B[liner]>onum){
				gotoxy(78,p);printf("<- You can't jump to there!");
				return restart(p);
			}
			if(B[liner]==liner){
				gotoxy(78,p);printf("<- Your program can't move on!");
				return restart(p);
			} 
			liner = B[liner]-1;
			Sleep((int)300/speed);
		}
		else if(A[liner]==8 && usable[7]==1){
			if(B[liner]>onum){
				gotoxy(78,p);printf("<- You can't jump to there!");
				return restart(p);
			}
			if(B[liner]==liner){
				gotoxy(78,p);printf("<- Your program can't move on!");
				return restart(p);
			} 
			if(robdata[1]==0){
				liner = B[liner]-1;
			}
			Sleep((int)500/speed);
		}
		else{
			gotoxy(78,p);printf("<- An unusable order!");
			return restart(p);
		}
		
		
		
		
		if(detect==1){
			gotoxy(18,23);printf("Press any keys to contine...");
			while(true){
				if(_kbhit()){
					gotoxy(18,23); printf("                            ");
					_getch();
					break;
				}
			}
		}
		gotoxy(61,p);
		printf(" ");//覆盖显示
		liner+=1;
		trueliner ++;
	}
	return 0;
}

void levellas(int om, int em,string fi,int n, int *input,int *tarout,int insize, int outsize, string info, vector<int> usable){
	int checker=-1;
	while(checker==-1){
		checker = singlelevel(om,em,fi,n,input,tarout,insize,outsize,info,usable);
		PlaySound(NULL,NULL,SND_PURGE);
		system("cls");
		if(checker==2){
			levelinfo[usable[8]]=1;
			writein();
			break;
		}
	}
	return;
}
