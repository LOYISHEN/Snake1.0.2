//filename : output.c
//author : PenG
//update time : 2019/07/24

#include "output.h"

HANDLE hOut;    //��������ľ��
COORD pos = { 0,0 };    //�����λ��
CONSOLE_CURSOR_INFO cci;    //����ṹ��

//��ʼ���������
void initOutput(void)
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleCursorInfo(hOut, &cci);    //��ȡ�����Ϣ
	cci.dwSize = 1;    //���ù���С
	cci.bVisible = 0; //���ù�겻�ɼ� FALSE
	SetConsoleCursorInfo(hOut, &cci);    //����(Ӧ��)�����Ϣ
	pos.X = 0;
	pos.Y = 0;
	SetConsoleCursorPosition(hOut, pos);    //���ù������
}

//������ɫ
void setColor(int color)
{
	SetConsoleTextAttribute(hOut, color);
}

//���ù��λ��
void setPos(int x, int y)
{
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hOut, pos);
}

//���һ������ɫֵ�����أ������ַ���
void printPix(int x, int y, int color)
{
	setPos(2 * x, y);
	setColor(color);
	printf("  ");
}
