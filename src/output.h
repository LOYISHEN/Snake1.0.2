//filename : output.h
//author : PenG
//update time : 2019/07/24

#pragma once
#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>

void initOutput(void);    //��ʼ���������
void setColor(int color);    //������ɫ
void setPos(int x, int y);    //���ù��λ��
void printPix(int x, int y, int color);    //���һ������ɫֵ�����أ������ַ���

#endif
