#include<graphics.h>
#include<conio.h>
#include<malloc.h>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#define LEN sizeof(struct Snake)
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define Enter 13
#define TextX 400/*文字开始横坐标*/
#define TextY 300/*文字开始纵坐标*/
float H = 70, S = 0.8, L = 0.7f;/*H表示色相,红色在 0 度，绿色在 120 度，蓝色在 240 度，以此方向过渡。S表示饱和度，等于 0 时为灰色。在最大饱和度 1 时，具有最纯的色光。L表示亮度，等于 0 时为黑色，等于 0.5 时是色彩最鲜明的状态，等于 1 时为白色*/
int food[2] = { 400,300 };
int k = 0;/*控制毒草闪烁*/
int n = 0;/*读取进度*/
int node;/*节数*/
int level;/*难度*/
int map;/*地图选择*/
int mode=1;/*关数*/
int Speed;/*速度*/
IMAGE img[10];/*加载图片*/
int score = 0;/*计分*/
int i,j;/*控制循环*/
int direction;/*方向*/
int life;/*1为生，0为死*/
int Save[52] = { 0 };/*保存进度*/
int load = 1;/*判断是否为进度读取*1表示不是，0表示是进度读取**/
int Finalscore[10] = { 0 };/*记录游戏结束时总分*/
int finalscore = 0;/*最终得分*/
int m;/*智慧草出现剩余时间*/
time_t t, t1, t2, t3, t4;
FILE *fp, *fp1, *fp2, *fp3;
int gameover();
void Rank();/*分数排序*/
void drwall(int x, int y);
void playgame();
void movesnake();
void renewsnake();
void drawsnake();
void checksnake();
int selectlevel();/*选难度*/
void Begin();
void Continue();
void drawmap1();
void drawmap2();
void drawmap3();
void ready();
void mkfood(struct Snake*p);
void help();
void mkgrass();
void drgrass();
void ready() {
	cleardevice();
	setcolor(GREEN);
	settextstyle(40, 40, _T("宋体"));
	outtextxy(300, 250, _T("3"));
	Sleep(1000);
	cleardevice();
	outtextxy(300, 250, _T("2"));
	Sleep(1000);
	cleardevice();
	outtextxy(300, 250, _T("1"));
	Sleep(1000);
	cleardevice();
	outtextxy(60, 250, _T("God bless you!"));
	Sleep(1000);
	cleardevice();
}
struct Snake
{
	int x;
	int y;
	struct Snake *previous;
	struct Snake *next;
}*head, *tail, *p0, *pt, *p1,*p2,*p3;
struct Grass
{
	int x;
	int y;
	int need;
}grass[3],wisegrass;
struct Boom {
	int x;
	int y;
	int need;
}boom[3];
void mkfood(struct Snake*p)
{
	struct Snake*p1;
	p1 = p;
	srand((unsigned)time(NULL));
	food[0] = rand() % 560; while (food[0]% 20 != 0)food[0]++;
	food[1] = rand() % 560; while (food[1]% 20 != 0)food[1]++;
	do {
		if (food[0] == p->x&&food[1] == p->y)
			mkfood(p1);
		p = p->next;
	} while (p != NULL);
	while(((food[0] == boom[0].x) && (food[1] == boom[0].y)) || ((food[0] == boom[2].x) && (food[1] == boom[2].y)) || ((food[0] == boom[1].x) && (food[1] == boom[1].y))
		|| ((food[0]>580)||( food[1]>580)||(food[0]<20 )||(food[1]<20))&&((food[0] == grass[0].x) || (food[1] == grass[0].y)) || ((food[0] == grass[1].x) && (food[1] == grass[1].y)) || ((food[0] == grass[2].x) && (food[1] == grass[2].y)))
		mkfood(p1);
	if (map == 3) {
		for (i = 1,j=1; i <= 30; i++,j++) {
			if ((food[0] == i * 20) && (food[1] == j * 20)) mkfood(p1);
		}
		for (i = 28,j = 1; i>=1; i--, j++) {
			if ((food[0] == i * 20) && (food[1] == j * 20)) mkfood(p1);
		}
	}
}
void help() {
	cleardevice();
	loadimage(&img[0], _T("C:\\Users\\85267\\Documents\\Visual Studio 2015\\Projects\\U201617042范志康\\背景.jpg"));
	putimage(0, 0, &img[0]);
	setcolor(BLACK);
	outtextxy(20, 20, _T("返回主界面"));
	outtextxy(360, 400, _T("制作人：范志康"));
	outtextxy(250, 120, _T("操作说明"));
	outtextxy(250, 170, _T("向上按 ↑，向下按 ↓,左上:q,右上:e"));
	outtextxy(250, 190, _T("向左按 ←，向右按 →,左下:a,右下:d"));
	outtextxy(250, 210, _T("暂停：空格 保存:enter 读取:1"));
	setcolor(RED);
	outtextxy(320, 250, _T("   是食物"));
	outtextxy(320, 290, _T("   是地雷"));
	outtextxy(320, 330, _T("闪烁   是毒草"));
	setfillcolor(GREEN);
	bar3d(300, 250, 320, 270, 3, 1);
	setfillcolor(CYAN);
	bar3d(300, 290, 310, 310, 3, 1);
	setfillcolor(BROWN);
	bar3d(310, 290, 320, 310, 3, 1);
	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (m.x<80 & m.y>10 && m.x>10 && m.y<50)
				Begin(); break;
		default:break;
		}
	}
}
void mkgrass() {
	srand((unsigned)time(NULL));
	for (i = 0; i < 3; i++)
		if (grass[i].need == 1) {
			grass[i].x = rand() %580 ; while(grass[i].x % 20 != 0)grass[i].x++;
			grass[i].y = rand() % 580; while(grass[i].y % 20 != 0)grass[i].y++;
			grass[i].need = 0;
		}
	for (i = 1,j = 1; i <= 30; i++, j++)
		for (int m = 0; m < 3; m++) {
			if (grass[m].x == i * 20 && grass[m].y == j * 20)mkgrass();
		}
}
void drgrass() {
	
	for (i = 0; i <= 2; i++)
		if (grass[i].need == 0) {
			k++;
			if ((k % 2) != 0) {
				setfillstyle(SOLID_FILL, BLUE);
				setfillcolor(LIGHTMAGENTA);
				bar3d(grass[i].x, grass[i].y, grass[i].x + 20, grass[i].y + 20,3,1);
			}
			if ((k % 2 == 0)) {
				setfillcolor(HSLtoRGB(H, S, L));
				bar3d(grass[i].x, grass[i].y, grass[i].x + 20, grass[i].y + 20,3,1);
			}
		}
}
void drwisegrass() {
	setfillcolor(GREEN);
	setcolor(GREEN);
	bar3d(wisegrass.x, wisegrass.y, wisegrass.x + 5, wisegrass.y + 20, 3, 1);
	setfillcolor(WHITE);
	setcolor(WHITE);
	bar3d(wisegrass.x + 5, wisegrass.y, wisegrass.x + 15, wisegrass.y + 20, 3, 1);
	setfillcolor(RED);
	setcolor(RED);
	bar3d(wisegrass.x + 15, wisegrass.y, wisegrass.x + 20, wisegrass.y + 20, 3, 1);
}
void mkboom(){
	srand((unsigned)time(NULL));
	for (i = 0; i < 3; i++)
		if (boom[i].need == 1) {
			boom[i].x = rand() % 580; while (boom[i].x % 20 != 0)boom[i].x++;
			boom[i].y = rand() % 580; while (boom[i].y % 20 != 0)boom[i].y++;
			boom[i].need = 0;
		}
}
void drboom() {
	for(i=0;i<=2;i++)
		if (boom[i].need == 0) {
			setfillcolor(CYAN);
			bar3d(boom[i].x, boom[i].y, boom[i].x + 10, boom[i].y + 20,3,1);
			setfillcolor(BROWN);
			bar3d(boom[i].x+10, boom[i].y, boom[i].x + 20, boom[i].y + 20, 3, 1);
		}
}
void drd(int x, int y)
{
	
	setcolor(HSLtoRGB(head->x, 0.7, 0.7));
	setfillcolor(HSLtoRGB(head->x, 0.7, 0.7));
	bar3d(x, y, x + 20, y + 20, 3, 1);
}
void drfood(int x, int y) {
	
	setfillcolor(GREEN);
	bar3d(x + 1, y + 1, x + 19, y + 19,3,1);
}
void drl(int x1, int y1, int x2, int y2)
{
	int x, y;
	setfillcolor(BROWN);
	do {
		x = x1 * 20;
		y = y1 * 20;
		bar(x + 1, y + 1, x + 19, y + 19);
		x1<x2 ? x1++ : x2;
		y1<y2 ? y1++ : y2;
	} while (x1 != x2 || y1 != y2);
}
void Cleansnake() {
	
	p0= head;
	setcolor(BLACK);
	setfillcolor(BLACK);
	for (i = 0; i < node; i++) {
		bar3d(p0->x, p0->y, p0->x + 20, p0->y + 20, 3, 1);
		p0 = p0->next;
	}
}
void drsnake(struct Snake*p) {

	drd(p->x, p->y);
	p = p->next;
	while (p != NULL) {
		drd(p->x, p->y);
		p = p->next;
	}
}
void Pscore() {
	settextstyle(30, 15, _T("宋体"));
	outtextxy(600, 100, _T("score:"));
	setcolor(LIGHTGREEN);
	char a[5];
	sprintf_s(a,"%d", score);
	outtextxy(700, 100, a[0]);
	outtextxy(715, 100, a[1]);
}
int keydown(int z) {
	char ch;
	if (_kbhit()) {
		ch = _getch();
		switch (ch) {
		case LEFT:if (z != RIGHT)z = LEFT; break;
		case RIGHT:if (z != LEFT)z = RIGHT; break;
		case UP:if (z != DOWN)z = UP; break;
		case DOWN:if (z != UP)z = DOWN; break;
		case 'e':if (z != 'a')z = 'e'; break;
		case 'a':if (z != 'e')z = 'a'; break;
		case 'q':if (z != 'd')z = 'q'; break;
		case 'd':if (z != 'q')z = 'd'; break;
		case ' ':z = ' '; break;
		case Enter: {
			errno_t err;
			err = fopen_s(&fp2, "游戏进度存储", "w");
			Save[0] = food[0]; Save[1] = food[1]; Save[2] = grass[0].x; Save[3] = grass[0].y; Save[4] = grass[1].x; Save[5] = grass[1].y;
			Save[6] = grass[2].x; Save[7] = grass[2].y; Save[8] = boom[0].x; Save[9] = boom[0].y; Save[10] = boom[1].x; Save[11] = boom[1].y;
			Save[12] = boom[2].x; Save[13] = boom[2].y; Save[14] = score; Save[15] = level; Save[16] = direction; Save[17] = map; Save[18] = map;
			Save[50] = wisegrass.x; Save[51] = wisegrass.y;
			for (i = 19; (i<52) && (head->next != 0) && (life == 1); i++)
			{
				Save[i] = head->x;
				i++;
				Save[i] = head->y;
				head = head->next;
			}
			Save[i] = head->x;
			i++;
			Save[i] = head->y;
			for (i++; i<47; i++)
				Save[i] = 0;
			for (i = 0; i<52; i++)
				fwrite(&Save[i], 2, 1, fp2);
			fclose(fp2);
			setcolor(RED);
			settextstyle(60, 0, _T("华文行楷"));
			outtextxy(TextX - 250, TextY, _T("进度已保存，按任意键退出"));
			_getch();
			exit(0);
			break;
		}
		case 49: {
			load = 0;
			errno_t err;
			err = fopen_s(&fp3, "游戏进度存储", "r");
			for (i = 0; i<52; i++)
				fread(&Save[i], 2, 1, fp3);
			fclose(fp3);
			food[0] = Save[0]; food[1] = Save[1]; grass[0].x = Save[2]; grass[0].y = Save[3]; grass[1].x = Save[4]; grass[1].y = Save[5];
			grass[2].x = Save[6]; grass[2].y = Save[7]; boom[0].x = Save[8]; boom[0].y = Save[9]; boom[1].x = Save[10]; boom[1].y = Save[11];
			boom[2].x = Save[12]; boom[2].y = Save[13]; score = Save[14]; level = Save[15]; direction = Save[16]; map = Save[17]; mode = Save[18]; wisegrass.x = Save[50]; wisegrass.y = Save[51];
			cleardevice();
			Sleep(100);
			switch (map) {
			case 1:{setcolor(WHITE);
				line(0, 0, 600, 0);
				line(0, 0, 0, 600);
				line(600, 0, 600, 600);
				line(0, 600, 600, 600); }break;
			case 2:{drl(0, 0, 30, 0);
				drl(0, 1, 0, 30);/*左*/
				drl(0, 29, 30, 29);/*下*/
				drl(29, 1, 29, 29);/*右*/}break;
			case 3:setcolor(WHITE);
				line(0, 0, 600, 0);
				line(0, 0, 0, 600);
				line(600, 0, 600, 600);
				line(0, 600, 600, 600);
				int i, j = 1;
				for (i = 1; i <= 30; i++)
				{
					drwall(i, j);
					j++;
				}
				j = 1;
				for (i = 28; i >= 1; i--)
				{
					drwall(i, j);
					j++;
				}break;
			}
			p1 = p2 = (struct Snake*)malloc(LEN);
			n = 0;
			head = NULL;
			if (Save[19] != 0)
			{
				for (i = 19; (i<52) && (Save[i] != 0); i++)
				{
					p1->x = Save[i];
					i++;
					p1->y = Save[i];
					if (n == 0)
					{
						head = p1; head->previous = 0;
					}
					else
					{
						p2->next = p1;
						p1->previous = p2;
					}
					p2 = p1;
					n++;
					p1 = (struct Snake*)malloc(LEN);
				}
				p2->next = NULL;
				tail = p2; tail->next = NULL;
			}
			if ((wisegrass.x == 440) && (wisegrass.y == 540))
				load = 1;
			setcolor(RED);
			settextstyle(40, 0, _T("华文行楷"));
			outtextxy(TextX - 300, TextY, _T("进度已读取，按任意键继续"));
			_getch();
			setcolor(BLACK);
			settextstyle(40, 0, _T("华文行楷"));
			outtextxy(TextX - 300, TextY, _T("进度已读取，按任意键继续"));
		}
		default:break;
		}
		if (ch == 27)z = 27;
	}
	return z;
}
int gameover() {
	loadimage(&img[2], _T("C:\\Users\\85267\\Documents\\Visual Studio 2015\\Projects\\U201617042范志康\\结束.jpg"));
	putimage(0, 0, &img[2]);
	Rank();
	score = 0;
	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (m.x<550 & m.y>300 && m.x>200 && m.y<400)
				Begin(); break;
		default:break;
		}
	}
	return 0;
}
int selectlevel() {
	cleardevice();
	loadimage(&img[0], _T("C:\\Users\\85267\\Documents\\Visual Studio 2015\\Projects\\U201617042范志康\\背景.jpg"));
	putimage(0, 0, &img[0]);
	settextstyle(40, 30, _T("楷体"));
	outtextxy(260, 200, _T("level1"));
	outtextxy(260, 240, _T("level2"));
	outtextxy(260, 280, _T("level3"));
	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (m.x < 500 && m.y>180 && m.x > 200 && m.y < 230) {
				MessageBox(NULL, _T("您选择了简单"), _T("难度设置"), MB_OK | MB_SETFOREGROUND); return 300;
			}
			else
				if (m.x < 500 && m.x>200 && m.y > 230 && m.y < 270) {
					MessageBox(NULL, _T("您选择了中等"), _T("难度设置"), MB_OK | MB_SETFOREGROUND); return 200;
				}
				else
					if (m.x > 200 && m.x < 500 && m.y>270 && m.y < 310) {
						 MessageBox(NULL, _T("您选择了困难"), _T("难度设置"), MB_OK | MB_SETFOREGROUND); return 100;
					} break;
		default:break;
		}
	}
}
void selectmap() {
	cleardevice();
	loadimage(&img[3], _T("C:\\Users\\85267\\Documents\\Visual Studio 2015\\Projects\\U201617042范志康\\地图1.jpg"));
	putimage(50, 200, &img[3]);
	loadimage(&img[4], _T("C:\\Users\\85267\\Documents\\Visual Studio 2015\\Projects\\U201617042范志康\\地图2.jpg"));
	putimage(300, 200, &img[4]);
	loadimage(&img[5], _T("C:\\Users\\85267\\Documents\\Visual Studio 2015\\Projects\\U201617042范志康\\地图3.jpg"));
	putimage(550, 200, &img[5]);
	settextstyle(40, 30, _T("楷体"));
	outtextxy(80, 450, _T("地图1"));
	outtextxy(330, 450, _T("地图2"));
	outtextxy(580, 450, _T("地图3"));

	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (m.x < 250 && m.y>400 && m.x > 80 && m.y < 600) {
				MessageBox(NULL, _T("您选择了地图1"), _T("地图选择"), MB_OK | MB_SETFOREGROUND); map = 1; ready(); playgame(); break;
			}
			else
				if (m.x < 500 && m.x>300 && m.y > 400 && m.y < 600) {
					MessageBox(NULL, _T("您选择了地图2"), _T("地图选择"), MB_OK | MB_SETFOREGROUND); map = 2; ready(); playgame(); break;
				}
				else
					if (m.x > 550 && m.x < 750 && m.y>400 && m.y < 600) {
						MessageBox(NULL, _T("您选择了地图3"), _T("地图选择"), MB_OK | MB_SETFOREGROUND); map = 3; ready(); playgame(); break;
					}
		default:break;
		}
	}
}
void Pass() {
	settextstyle(60, 0, _T("华文隶书"));
	setcolor(WHITE);
	outtextxy(TextX - 100, TextY - 100, _T("Excellent !  Go on !"));
	settextstyle(40, 0, _T("华文行楷"));
	outtextxy(TextX - 50, TextY + 100, _T("棒极了，继续加油！"));
	_getch();
}

void rankplay() {
	cleardevice();
	setbkcolor(BLUE);
	char str[10];
	settextstyle(60, 0, _T("华文行楷"));
	setcolor(RED);
	outtextxy(TextX - 40, TextY - 300, _T("排行榜"));
	setcolor(BROWN);
	settextstyle(30, 0, _T("华文隶书"));
	sprintf_s(str, "1 :%d", Finalscore[0]);
	outtextxy(TextX, TextY - 240, str[0]);
	outtextxy(TextX, TextY - 225, str[1]);
	sprintf_s(str, "2 :%d", Finalscore[1]);
	outtextxy(TextX, TextY - 190, str[0]);
	sprintf_s(str, "3 :%d", Finalscore[2]);
	outtextxy(TextX, TextY - 140, str[0]);
	sprintf_s(str, "4 :%d", Finalscore[3]);
	outtextxy(TextX, TextY - 90, str[0]);
	sprintf_s(str, "5 :%d", Finalscore[4]);
	outtextxy(TextX, TextY - 40, str[0]);
	sprintf_s(str, "6 :%d", Finalscore[5]);
	outtextxy(TextX, TextY + 10, str[0]);
	sprintf_s(str, "7 :%d", Finalscore[6]);
	outtextxy(TextX, TextY + 60, str[0]);
	sprintf_s(str, "8 :%d", Finalscore[7]);
	outtextxy(TextX, TextY + 110, str[0]);
	sprintf_s(str, "9 :%d", Finalscore[8]);
	outtextxy(TextX, TextY + 160, *str);
	sprintf_s(str, "10 :   %d", Finalscore[9]);
	outtextxy(TextX - 15, TextY + 210, str[0]);
	outtextxy(TextX, TextY + 210, str[1]);
	setcolor(YELLOW);

	settextstyle(40, 0, _T("华文隶书"));
	setcolor(RED);
	setbkmode(TRANSPARENT);
	outtextxy(TextX, TextY + 250, _T("返回"));
	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (m.x<450 & m.y>550 && m.x>400 && m.y<590)
				Begin(); break;
		default:break;
		}
	}
}
void Rank() {
	int i, j, t = 0;
	for (i = 0, j = 0; i<10; i++)
	{
		if (finalscore>Finalscore[i])
		{
			t = Finalscore[i];
			Finalscore[i] = finalscore;
			break;
		}
	}
	j = i;
	if (t != 0 && j != 9)
	{
		for (i = 9; i >= j + 2; i--)
			Finalscore[i] = Finalscore[i - 1];
		Finalscore[j + 1] = t;
	}
	errno_t err;
	err = fopen_s(&fp1, "高分榜记录", "w");
	for (i = 0; i<10; i++)
		fwrite(&Finalscore[i], 2, 1, fp1);
	fclose(fp1);
}

void Begin() {
	initgraph(800, 600);
	/*初始化文件*/
	
	setcolor(WHITE);
	setbkmode(TRANSPARENT);/*透明字体背景*/
	settextstyle(10, 0, 0);
	for (int i = 1; i <= 10; i++) {
		outtextxy( i * 30, 200, _T("聪明的小蛇"));
		outtextxy(20+ i * 30, 250, _T("Play"));
		outtextxy(20 + i * 30, 300, _T("Help"));
		outtextxy(20 + i * 30, 350, _T("Rank"));
		outtextxy(20 + i * 30, 400, _T("ESC"));
		Sleep(100);
		cleardevice();
	}
	loadimage(&img[0], _T("C:\\Users\\85267\\Documents\\Visual Studio 2015\\Projects\\U201617042范志康\\背景.jpg"));
	putimage(0, 0, &img[0]);
	outtextxy(300, 200, _T("聪明的小蛇"));
	outtextxy(320, 250, _T("Play"));
	outtextxy(320, 300, _T("Help"));
	outtextxy(320, 350, _T("Rank"));
	outtextxy(320, 400, _T("ESC"));
	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (m.x < 400 & m.y>230 && m.x > 280 && m.y < 270)
			{
				cleardevice();
				Speed = selectlevel();
				selectmap();
				ready();
				playgame();
			}
			else if (m.x < 400 && m.x>280 && m.y > 280 && m.y < 320)
				help();
			else if (m.x < 400 && m.x>280 && m.y > 320 && m.y < 370)
				rankplay();
			else if (m.x < 400 && m.x>280 && m.y > 370 && m.y < 420)
				exit(0); break;
		default:break;

		}
	}
}

void playgame() {
	
	loop: if (Speed == 100)level = 1;
	if (Speed == 200)level = 2;
	if (Speed == 300)level = 3;
	/*t = (unsigned)time(NULL);/*随机时间*/
	wisegrass.x = 440; wisegrass.y = 540;
	grass[0].x = 60; grass[0].y = 100;
	grass[1].x = 220; grass[1].y = 300;
	grass[2].x = 460; grass[2].y = 500;
	for (i = 0; i <= 2; i++)grass[i].need = 0;
	boom[0].x = 300; boom[0].y = 160;
	boom[1].x = 540; boom[1].y = 80;
	boom[2].x = 520; boom[2].y = 420;
	for (i = 0; i <= 2; i++)boom[i].need = 0;
	head = (struct Snake*)malloc(sizeof(struct Snake));
	tail = (struct Snake*)malloc(sizeof(struct Snake));
	pt = (struct Snake*)malloc(sizeof(struct Snake));
	p1 = (struct Snake*)malloc(sizeof(struct Snake));
	head->x = 300;head->y = 100;
	head->next = tail; head->previous = 0;
	tail->x = 280;tail->y = 100;
	tail->next = NULL; tail->previous = head;
	node = 2;
	life = 1;
	direction= RIGHT;
	while (1) {
		int temp = direction;
		direction = keydown(direction);
		if (direction == 27) Continue();
		if (direction == ' ') {
			outtextxy(300, 400, _T("暂停"));/*暂停*/
			_getch();
			direction = temp;
		}
		switch (map)
		{
		case 1:drawmap1(); break;
		case 2:drawmap2(); break;
		case 3:drawmap3(); 
		}
		Cleansnake();/*清除蛇*/
		renewsnake();/*更新蛇头*/
		movesnake();/*移动*/
		checksnake();
		drawsnake();
		if (life == 0) break; 
		Sleep(Speed/2);
		if (score >= 40)break;
	}
	if (life == 0) { if (mode == 1) finalscore = score;
	if (mode == 2)	finalscore = 40 + score; if (mode == 3) finalscore = 80 + score;	gameover();
	}
	else if (score >= 40) {
		if (mode >= 3) {

			settextstyle(60, 0, _T("华文隶书"));
			setcolor(RED);
			outtextxy(TextX - 300, TextY - 100, _T("Congratulations!you win!"));
			settextstyle(40, 0, _T("华文行楷"));
			outtextxy(TextX - 250, TextY - 20, _T("祝贺您，您赢了(^_^)"));
			settextstyle(40, 0, _T("华文行楷"));
			outtextxy(TextX - 150, TextY + 50, _T("按任意键继续……"));
			
			_getch();
		}
		else {
			mode++;
			score = 0;
			Pass();
			ready();
			goto loop;
		}
	}
}
void movesnake() {
	switch (direction) {
	case LEFT:head->x -= 20; break;
	case RIGHT:head->x += 20;break;
	case UP:head->y -= 20;  break;
	case DOWN:head->y += 20;  break;
	case 'q':head->x -= 10; head->y -= 10; break;
	case 'e':head->x += 10; head->y -= 10; break;
	case 'a':head->y += 10; head->x -= 10;  break;
	case 'd':head->y += 10; head->x += 10; break;
	}
}

void renewsnake() {
	pt = (struct Snake*)malloc(sizeof(struct Snake));//增加蛇体的存储空间
	pt->x = head->x;
	pt->y = head->y;
	pt->previous = NULL;
	pt->next = head;
	head->previous = pt;
	head = pt;
}
void drawsnake() {
	struct Snake*p;
	p = head;
	for(i=0;i<node;i++){
		drd(p->x, p->y);
		p = p->next;
	}
}
void checksnake() {
	switch (map) {
	case 1:if (head->x >= 600) { head->x = 0; break; } //如果越界，从另一边出来 
		   else if (head->x <= 0) { head->x = 600; break; }
		   else if (head->y >= 600) { head->y = 0; break; }
		   else if (head->y <= 0) { head->y = 600; break; }break;
	case 2:if (head->x > 560 || head->y > 560 || head->x < 20 || head->y < 20) { life = 0; break; }break;/*检查撞墙*/
	case 3: for (i = 1, j = 1; i <= 30; i++, j++) {
		if ((head->x == i * 20) && (head->y == j * 20)) { life = 0; break; }
	}
		for (i = 28, j = 1; i >= 1; i--, j++) {
			if ((head->x == i * 20) && (head->y == j * 20)) {life = 0; break;}
		}
		if (head->x >= 600) { head->x = 0; break; } //如果越界，从另一边出来 
		else if (head->x <= 0) { head->x = 600; break; }
		else if (head->y >= 600) { head->y = 0; break; }
		else if (head->y <= 0) { head->y = 600; break; }break;
	}

	/********************检测是否碰到身体********************/
	p1 = head->next;
	for (i = 0; i < node - 1; i++) {
		if (p1->x == head->x&&p1->y == head->y) life = 0;
		else p1 = p1->next;
	}

	/********************检测是否吃到食物********************/
	if (head->x == food[0] && head->y == food[1]) {
		score += 10;
		node++;
		mkfood(head);
	}

	/*吃到智慧草*/
	if ((head->x == wisegrass.x) && (head->y == wisegrass.y))
	{

		/*t4 = (unsigned)time(NULL);*/
		setfillstyle(BLACK, SOLID_FILL);/*吃到智慧草后智慧草消失*/
		setcolor(BLACK);
		bar3d(wisegrass.x, wisegrass.y, wisegrass.x + 20, wisegrass.y + 20, 3, 1);
		wisegrass.x = wisegrass.y = 1001;
	}

	/******************检测是否吃到地雷******************/
	for (i = 0; i<=2; i++)
		if ((boom[i].x == head->x) && (boom[i].y == head->y)) {//踩到地雷
			score = score / 2;
			boom[i].need = 1;
			mkboom();
			if (node <= 2) {//被炸死
				life=0;
				break;
			}
			else node = node - node / 2;
		}
	/**********************检查是否吃到毒草******************/
	for (i = 0; i<=2; i++)
		if ((grass[i].x == head->x) && (grass[i].y == head->y)) {//吃到毒草
			score-=5;
			grass[i].need = 1;
			mkgrass();
			if (node <= 2) {//被毒死
				life = 0;
				break;
			}
			else node = node - 1;
		}
}

void drwall(int x, int y) {
	x = x * 20;
	y = y * 20;
	setfillcolor(BROWN);
	bar(x + 1, y + 1, x + 19, y + 19);
}
void drawmap1()
{
	cleardevice();
	setcolor(WHITE);
	line(0, 0, 600, 0);
	line(0, 0, 0, 600);
	line(600, 0, 600, 600);
	line(0, 600, 600, 600);
	drgrass();
	drboom();
	drfood(food[0], food[1]);
	drwisegrass();
	Pscore();
	settextstyle(20, 0, _T("楷体"));
	outtextxy(650, 150, _T("关数："));
	switch (mode) {
	case 1:outtextxy(650, 180, _T("第一关")); break;
	case 2:outtextxy(650, 180, _T("第二关")); break;
	case 3:outtextxy(650, 180, _T("第三关")); break;
	}
	outtextxy(650, 220, _T("速度："));
	switch (Speed) {
	case 300:outtextxy(650, 240, _T("一般")); break;
	case 200:outtextxy(650, 240, _T("中等")); break;
	case 100:outtextxy(650, 240, _T("较快")); break;
	}
	setfillcolor(GREEN);
	bar3d(620, 300, 640, 320, 3, 1);
	outtextxy(650, 300, _T("食物"));
	setfillcolor(CYAN);
	bar3d(620, 340, 630, 360, 3, 1);
	setfillcolor(BROWN);
	bar3d(630, 340, 640, 360, 3, 1);
	outtextxy(650, 340, _T("地雷"));
	outtextxy(620, 380, _T("闪烁是毒草"));
}
void drawmap2() {
	cleardevice();
	drl(0, 0, 30, 0);
	drl(0, 1, 0, 30);/*左*/
	drl(0, 29, 30, 29);/*下*/
	drl(29, 1, 29, 29);/*右*/
	drgrass();
	drboom();
	drfood(food[0], food[1]);
	drwisegrass();
	Pscore();
	settextstyle(20, 0, _T("楷体"));
	outtextxy(650, 150, _T("关数："));
	switch (mode) {
	case 1:outtextxy(650, 180, _T("第一关")); break;
	case 2:outtextxy(650, 180, _T("第二关")); break;
	case 3:outtextxy(650, 180, _T("第三关")); break;
	}
	outtextxy(650, 220, _T("速度："));
	switch (Speed) {
	case 300:outtextxy(650, 240, _T("一般")); break;
	case 200:outtextxy(650, 240, _T("中等")); break;
	case 100:outtextxy(650, 240, _T("较快")); break;
	}
	setfillcolor(GREEN);
	bar3d(620, 300, 640, 320, 3, 1);
	outtextxy(650, 300, _T("食物"));
	setfillcolor(CYAN);
	bar3d(620, 340, 630, 360, 3, 1);
	setfillcolor(BROWN);
	bar3d(630, 340, 640, 360, 3, 1);
	outtextxy(650, 340, _T("地雷"));
}
void drawmap3() {
	cleardevice();
	setcolor(WHITE);
	line(0, 0, 600, 0);
	line(0, 0, 0, 600);
	line(600, 0, 600, 600);
	line(0, 600, 600, 600);
	int i, j = 1;
	for (i = 1; i <= 30; i++)
	{
		drwall(i, j);
		j++;
	}
	j = 1;
	for (i = 28; i >= 1; i--)
	{
		drwall(i, j);
		j++;
	}
	drgrass();
	drboom();
	drwisegrass();
	drfood(food[0], food[1]);
	Pscore();
	settextstyle(20, 0, _T("楷体"));
	outtextxy(650, 150, _T("关数："));
	switch (mode) {
	case 1:outtextxy(650, 180, _T("第一关")); break;
	case 2:outtextxy(650, 180, _T("第二关")); break;
	case 3:outtextxy(650, 180, _T("第三关")); break;
	}
	outtextxy(650, 220, _T("速度："));
	switch (Speed) {
	case 300:outtextxy(650, 240, _T("一般")); break;
	case 200:outtextxy(650, 240, _T("中等")); break;
	case 100:outtextxy(650, 240, _T("较快")); break;
	}
	setfillcolor(GREEN);
	bar3d(620, 300, 640, 320, 3, 1);
	outtextxy(650, 300, _T("食物"));
	setfillcolor(CYAN);
	bar3d(620, 340, 630, 360, 3, 1);
	setfillcolor(BROWN);
	bar3d(630, 340, 640, 360, 3, 1);
	outtextxy(650, 340, _T("地雷"));
} 

void Continue() {
	outtextxy(100, 240, _T("continue?"));
	settextstyle(30, 20, _T("宋体"));
	outtextxy(250, 290, _T("yes"));
	outtextxy(250, 330, _T("no"));
	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (m.x<400 & m.y>280 && m.x>250 && m.y<320)
				Begin();
			else
				if (m.x<400 && m.x>250 && m.y>320 && m.y<360)
					gameover(); break;
		default:break;
		}
	}
}

int main()
{
	Begin();
	gameover();
}
