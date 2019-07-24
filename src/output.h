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

void initOutput(void);    //初始化输出设置
void setColor(int color);    //设置颜色
void setPos(int x, int y);    //设置光标位置
void printPix(int x, int y, int color);    //输出一个带颜色值的像素（两个字符）

#endif
