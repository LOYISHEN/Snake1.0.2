//filename : output.c
//author : PenG
//update time : 2019/07/24

#include "output.h"

HANDLE hOut;    //用于输出的句柄
COORD pos = { 0,0 };    //输出的位置
CONSOLE_CURSOR_INFO cci;    //定义结构体

//初始化输出设置
void initOutput(void)
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleCursorInfo(hOut, &cci);    //获取光标信息
	cci.dwSize = 1;    //设置光标大小
	cci.bVisible = 0; //设置光标不可见 FALSE
	SetConsoleCursorInfo(hOut, &cci);    //设置(应用)光标信息
	pos.X = 0;
	pos.Y = 0;
	SetConsoleCursorPosition(hOut, pos);    //设置光标坐标
}

//设置颜色
void setColor(int color)
{
	SetConsoleTextAttribute(hOut, color);
}

//设置光标位置
void setPos(int x, int y)
{
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hOut, pos);
}

//输出一个带颜色值的像素（两个字符）
void printPix(int x, int y, int color)
{
	setPos(2 * x, y);
	setColor(color);
	printf("  ");
}
