// 打球吃小球.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <graphics.h> //图形库头文件
#include <time.h>
#include <vector>
using namespace std;

//窗口宽
#define WIDTH 500
//窗口高
#define HEIGHT 800

COLORREF colors[7] = { BLACK, BLUE ,GREEN, CYAN ,MAGENTA, BROWN, YELLOW };


struct Fx{
	int x;
	int y;
};

struct Ball{
	int x, y;		//坐标
	COLORREF color;	//颜色
	int radius;		//半径
	Fx fx;			//方向
};

bool isLive = true;
int myX = 400, myY = 700;
int radius = 5;

//随机生成的球
vector<Ball> balls;

//创造敌人
void init(){
	srand(time(NULL));
	Ball ball;
	for (int i = 0; i < 20; i++){
		ball.x = 0;
		ball.y = HEIGHT / 2;
		ball.color = colors[rand() % 7];
		ball.radius = rand() % 80;

		ball.fx.x = rand()%10;
		ball.fx.y = (rand()%20) - 10;

		balls.push_back(ball);
	}
}
//绘制球
void drawBalls(){
	//2 画球
	setfillcolor(RED);
	fillcircle(myX, myY, radius);

	//画敌人球
	for (int i = 0; i < (int)balls.size(); i++){
		setfillcolor(balls[i].color);
		fillcircle(balls[i].x, balls[i].y, balls[i].radius);
	}
}

//改变每个敌人球的坐标
VOID moveBalls(HWND hWnd, UINT uInt, UINT_PTR ptr, DWORD dword){
	for (int i = 0; i < (int)balls.size(); i++){
		balls[i].x += balls[i].fx.x;
		balls[i].y += balls[i].fx.y;
	}
}

//监督函数
void check(){
	int x;
	int y;
	//1 有球移动出界,重新初始化
	for (int i = 0; i < (int)balls.size(); i++){
		if ((balls[i].x - balls[i].radius) >= WIDTH ||	//从右边出界
			(balls[i].y - balls[i].radius) >= HEIGHT ||	//从下边出界
			(balls[i].x + balls[i].radius) <= 0 ||		//从左边出界
			(balls[i].radius + balls[i].y) <= 0){		//从上边出界
			balls[i].x = 0;
			balls[i].y = HEIGHT / 2;
			balls[i].color = colors[rand() % 7];
			balls[i].radius = rand() % 80;

			balls[i].fx.x = rand() % 10;
			balls[i].fx.y = (rand() % 20) - 10;
		}
		else{
			x = (balls[i].x > myX) ? (balls[i].x - myX) : (myX - balls[i].x);//横向间隔
			y = (balls[i].y > myY) ? (balls[i].y - myY) : (myY - balls[i].y);//纵向间隔
			if ((x*x + y*y) < (balls[i].radius + radius)*(balls[i].radius + radius)){//和自己球有碰撞
				if (balls[i].radius > radius){
					//游戏结束
					isLive = false;
				}
				else{
					//吃掉一个球
					balls[i].x = balls[i].y = -100;


					//printf("before:redius:%d\n", radius);
					if (radius + balls[i].radius <= 200)
						radius += balls[i].radius;
					//printf("after:%d+%d = %d\n", balls[i].radius, radius,radius);
				}
			}
		}
	}
	//2 让球的数量变化

}


void setMyBall(){
	//3 让球跟随鼠标移动
	MOUSEMSG  msg;
	while (1){
		//3.1 随时获取鼠标坐标
		msg = GetMouseMsg();

		//3.2 设置球的圆心坐标为鼠标的坐标
		if (msg.mkLButton){//鼠标左键是否按下
			myX = msg.x;
			myY = msg.y;
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//1 创建窗口
	HWND hWnd = initgraph(WIDTH, HEIGHT,SHOWCONSOLE);

	//创造敌人
	init();

	//设置定时器,每隔固定时间改变球的位置
	SetTimer(hWnd, 10086, 10, (TIMERPROC)moveBalls);

	DWORD id;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)setMyBall, NULL, NULL, &id);

	
	while (1){
		if (!isLive) break;
		check();
		BeginBatchDraw();

		setbkcolor(WHITE);//设置窗口背景为 WHITE
		cleardevice();//刷新
		
		
		drawBalls();

		EndBatchDraw();
	}
		
	setbkcolor(WHITE);//设置窗口背景为 WHITE
	cleardevice();//刷新
	settextcolor(RED);
	outtextxy(300, 400, L"Game Over!");

	while (1);
	return 0;
}

