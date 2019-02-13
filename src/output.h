#pragma once
#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>

//初始化输出设置
void initOutput(void);

//设置颜色
void setColor(int color);

//设置光标位置
void setPos(int x, int y);

//输出一个带颜色值的像素（两个字符）
void printPix(int x, int y, int color);

#endif