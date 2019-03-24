// �����С��.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <graphics.h> //ͼ�ο�ͷ�ļ�
#include <time.h>
#include <vector>
using namespace std;

//���ڿ�
#define WIDTH 500
//���ڸ�
#define HEIGHT 800

COLORREF colors[7] = { BLACK, BLUE ,GREEN, CYAN ,MAGENTA, BROWN, YELLOW };


struct Fx{
	int x;
	int y;
};

struct Ball{
	int x, y;		//����
	COLORREF color;	//��ɫ
	int radius;		//�뾶
	Fx fx;			//����
};

bool isLive = true;
int myX = 400, myY = 700;
int radius = 5;

//������ɵ���
vector<Ball> balls;

//�������
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
//������
void drawBalls(){
	//2 ����
	setfillcolor(RED);
	fillcircle(myX, myY, radius);

	//��������
	for (int i = 0; i < (int)balls.size(); i++){
		setfillcolor(balls[i].color);
		fillcircle(balls[i].x, balls[i].y, balls[i].radius);
	}
}

//�ı�ÿ�������������
VOID moveBalls(HWND hWnd, UINT uInt, UINT_PTR ptr, DWORD dword){
	for (int i = 0; i < (int)balls.size(); i++){
		balls[i].x += balls[i].fx.x;
		balls[i].y += balls[i].fx.y;
	}
}

//�ල����
void check(){
	int x;
	int y;
	//1 �����ƶ�����,���³�ʼ��
	for (int i = 0; i < (int)balls.size(); i++){
		if ((balls[i].x - balls[i].radius) >= WIDTH ||	//���ұ߳���
			(balls[i].y - balls[i].radius) >= HEIGHT ||	//���±߳���
			(balls[i].x + balls[i].radius) <= 0 ||		//����߳���
			(balls[i].radius + balls[i].y) <= 0){		//���ϱ߳���
			balls[i].x = 0;
			balls[i].y = HEIGHT / 2;
			balls[i].color = colors[rand() % 7];
			balls[i].radius = rand() % 80;

			balls[i].fx.x = rand() % 10;
			balls[i].fx.y = (rand() % 20) - 10;
		}
		else{
			x = (balls[i].x > myX) ? (balls[i].x - myX) : (myX - balls[i].x);//������
			y = (balls[i].y > myY) ? (balls[i].y - myY) : (myY - balls[i].y);//������
			if ((x*x + y*y) < (balls[i].radius + radius)*(balls[i].radius + radius)){//���Լ�������ײ
				if (balls[i].radius > radius){
					//��Ϸ����
					isLive = false;
				}
				else{
					//�Ե�һ����
					balls[i].x = balls[i].y = -100;


					//printf("before:redius:%d\n", radius);
					if (radius + balls[i].radius <= 200)
						radius += balls[i].radius;
					//printf("after:%d+%d = %d\n", balls[i].radius, radius,radius);
				}
			}
		}
	}
	//2 ����������仯

}


void setMyBall(){
	//3 �����������ƶ�
	MOUSEMSG  msg;
	while (1){
		//3.1 ��ʱ��ȡ�������
		msg = GetMouseMsg();

		//3.2 �������Բ������Ϊ��������
		if (msg.mkLButton){//�������Ƿ���
			myX = msg.x;
			myY = msg.y;
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//1 ��������
	HWND hWnd = initgraph(WIDTH, HEIGHT,SHOWCONSOLE);

	//�������
	init();

	//���ö�ʱ��,ÿ���̶�ʱ��ı����λ��
	SetTimer(hWnd, 10086, 10, (TIMERPROC)moveBalls);

	DWORD id;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)setMyBall, NULL, NULL, &id);

	
	while (1){
		if (!isLive) break;
		check();
		BeginBatchDraw();

		setbkcolor(WHITE);//���ô��ڱ���Ϊ WHITE
		cleardevice();//ˢ��
		
		
		drawBalls();

		EndBatchDraw();
	}
		
	setbkcolor(WHITE);//���ô��ڱ���Ϊ WHITE
	cleardevice();//ˢ��
	settextcolor(RED);
	outtextxy(300, 400, L"Game Over!");

	while (1);
	return 0;
}

