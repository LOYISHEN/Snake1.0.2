#pragma once
#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>

//��ʼ���������
void initOutput(void);

//������ɫ
void setColor(int color);

//���ù��λ��
void setPos(int x, int y);

//���һ������ɫֵ�����أ������ַ���
void printPix(int x, int y, int color);

#endif