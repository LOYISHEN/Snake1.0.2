//filename : main.c
//author : PenG
//update time : 2019/07/24

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <Windows.h>
#include <time.h>

#include "output.h"

#define WIDTH 26    //地图的宽度
#define HEIGHT 20    //地图的高度

//定义四个方向控制键和一个暂停键
#define KEY_UP '8'    //向上键
#define KEY_DOWN '2'    //向下键
#define KEY_LEFT '4'    //向左键
#define KEY_RIGHT '6'    //向右键
#define KEY_SUSPEND '5'    //暂停键

//定义墙，蛇，食物和背景的颜色
#define FOOD_COLOR 0x0060
#define SNAKE_BODY_COLOR 0x0010
#define SNAKE_HEAD_COLOR 0x0040
#define WALL_COLOR	0x00f0
#define BG_COLOR 0x0000

//定义游戏速度
const int GAME_SPEED = 24;

//定义转向的方向
enum D { UP, RIGHT, DOWN, LEFT };
//定义当前方向，用于移动转向时比较
enum D tempD = RIGHT;

typedef struct snake   //存储身体信息
{
	int x;  //身体节点 x 坐标
	int y;  //身体节点 y 坐标
	struct snake *last;  //上一身体节点地址
}snake;

//地图记录，只需要记录一个矩形就好了
typedef struct game_map
{
	int startX;
	int startY;
	int endX;
	int endY;
}Map;
Map map = {0, 0, WIDTH-1, HEIGHT-1};

//定义蛇和食物的指针，食物的结构其实跟蛇身体的结构一样 
snake *tail, *body, *head, *food;

int snake_length = 3;  //蛇的长度，初始化为3


//启动游戏
void start(void);

//初始化蛇
void initSnake(void);

//清除地图上蛇的身体
void cleanSnake(void);

//处理方向，返回正确方向
enum D judgeDirection(enum D direction);

//清除多余的输入
void cleanInput(void);

//获取输入的方向
enum D getDirection(void);

//判断有没有吃到食物，吃到了就增加长度
//吃到食物返回 true，否则返回 false
bool eatFood(enum D direction);

//移动蛇（注意不要让蛇向后走了）
void moveSnake(enum D direction);

//检测有没有撞到墙
bool knockWall(void);

//检测有没有撞到自己的身体
bool biteSelf(void);

//释放蛇的内存
void freeSnake(void);

//画墙，一次游戏只需要画一次墙
void printWall(void);

//画蛇
void printSnake(void);

//画食物
void printFood(void);


int main(void)
{
	initOutput();

	start();

	return 0;
}

//启动游戏
void start(void)
{
	int size = (WIDTH - 2) * (HEIGHT - 2);

	initSnake();
	printFood();
	printWall();
	while (1)
	{
		tempD = judgeDirection(getDirection());
		if (eatFood(tempD))
		{
			snake_length++;

			//当已经吃满了整个地图之后就退出
			if (snake_length >= size)
			{
				printSnake();
				break;
			}

			printFood();
		}
		else
		{
			//在这里控制蛇的走向
			moveSnake(tempD);
		}

		//撞到墙或者咬到自己就重新开始
		if (knockWall() || biteSelf())
		{
			//提示游戏已经结束
			cleanInput();
			Sleep(1);
			setPos(0, map.endY + 1);
			setColor(0x000f);
			printf("\nYou lose!!\nAnd you got %d point(s)", snake_length - 3);
			printf("\nPress any key to play again...");
			_getch();

			//清除屏幕上的画面
			system("cls");
			cleanSnake();
			freeSnake();
			free(food);

			//重新初始化蛇，重新开始游戏
			initSnake();
			printSnake();
			printFood();
			printWall();
			tempD = RIGHT;
			snake_length = 3;
		}  //if  end
		Sleep(100);

	}  //while (main cycle) end
	printf("WoW!!\nYou win!!");
}

//初始化蛇,把蛇的初始长度的身体也画出来
void initSnake(void)
{
	//蛇初始的位置
	int x, y;
	x = 2;
	y = 1;

	//获取内存空间
	tail = (snake *)malloc(sizeof(snake));
	body = (snake *)malloc(sizeof(snake));
	head = (snake *)malloc(sizeof(snake));

	//把位置信息写入链表中
	tail->x = x - 1;
	tail->y = y;
	tail->last = body;
	body->x = x;
	body->y = y;
	body->last = head;
	head->x = x + 1;
	head->y = y;
	head->last = NULL;

	printPix(head->x, head->y, SNAKE_HEAD_COLOR);
	printPix(body->x, body->y, SNAKE_BODY_COLOR);
	printPix(tail->x, tail->y, SNAKE_BODY_COLOR);
}

//判断该点是否已经避开蛇的位置，避开返回真
bool avoidSnake(int x, int y)
{
	body = tail;
	while (body != NULL)
	{
		if (body->x == x && body->y == y)
		{
			return false;
		}
		body = body->last;
	}
	return true;
}

//打印蛇，整个蛇身打印出来
void printSnake(void)
{
	body = tail;	//不能动tail变量
	while (body != NULL)
	{
		if (body == head)
		{
			printPix(body->x, body->y, SNAKE_HEAD_COLOR);
		}
		else
		{
			printPix(body->x, body->y, SNAKE_BODY_COLOR);
		}
		
		body = body->last;
	}  //while  end
}

//判断该点是否已经避开墙的位置，避开返回真
bool avoidWall(int x, int y)
{
	if (x == 0 || y == 0 || x == map.endX || y == map.endY)
	{
		return false;
	}
	return true;
}

//记录并打印一个食物
void printFood(void)
{
	int tx, ty;
	food = (snake *)malloc(sizeof(snake));

	//获取一个随机的位置，而且要避开墙和蛇的位置
	srand((unsigned)time(NULL));
	do
	{
		tx = (rand() % (WIDTH - 2)) + 1;
		ty = (rand() % (HEIGHT - 2)) + 1;
	} while (!avoidSnake(tx, ty) && !avoidWall(tx, ty));	//避开蛇和墙

	food->x = tx;
	food->y = ty;
	food->last = NULL;
	printPix(tx, ty, FOOD_COLOR);
}

//画墙
void printWall(void)
{
	//画上下边缘
	for (int i = 0; i < WIDTH; i++)
	{
		printPix(i, 0, WALL_COLOR);
		printPix(i, map.endY, WALL_COLOR);
	}
	//画左右边缘，重复画了四个角也没关系
	for (int i = 0; i < HEIGHT; i++)
	{
		printPix(0, i, WALL_COLOR);
		printPix(map.endX, i, WALL_COLOR);
	}
}

//清除地图上蛇全部的身体
void cleanSnake(void)
{
	body = tail;
	while (body != NULL)
	{
		printPix(body->x, body->y, BG_COLOR);
		body = body->last;
	}  //while  end
}

//处理方向，返回正确方向
enum D judgeDirection(enum D direction)
{
	switch (direction)
	{
		case LEFT:
		case RIGHT:
			if (tempD != LEFT && tempD != RIGHT)
			{
				return direction;
			}
			else
			{
				return tempD;
			}
			break;

		case UP:
		case DOWN:
			if (tempD != UP && tempD != DOWN)
			{
				return direction;
			}
			else
			{
				return tempD;
			}
			break;

		default:
			NULL;
			break;
	}  //switch  end
}

//清除多余的输入
void cleanInput(void)
{
	while (_kbhit())
	{
		_getch();
	}
}

//获取输入的方向
enum D getDirection(void)
{
	while (_kbhit())
	{
		switch (getch())
		{
			case KEY_UP:
				cleanInput();
				return UP;
				break;

			case KEY_DOWN:
				cleanInput();
				return DOWN;
				break;

			case KEY_LEFT:
				cleanInput();
				return LEFT;
				break;

			case KEY_RIGHT:
				cleanInput();
				return RIGHT;
				break;

			default:
				NULL;
				break;
		}  //switch  end
	}  //while  end

	return tempD;
}

//判断有没有吃到食物，吃到了就增加长度,然后画出蛇的身体
//吃到食物返回 true，否则返回 flase
bool eatFood(enum D direction)
{
	enum D t_dire = direction;

	int tx, ty;
	tx = head->x;
	ty = head->y;

	switch (t_dire)
	{
		case UP:
			ty--;
			break;

		case DOWN:
			ty++;
			break;

		case LEFT:
			tx--;
			break;

		case RIGHT:
			tx++;
			break;

		default:
			NULL;
			break;
	}  //switch end

	//判断此时是否吃到食物
	if (food->x == tx && food->y == ty)
	{
		printPix(head->x, head->y, SNAKE_BODY_COLOR);
		head->last = food;
		head = head->last;
		printPix(head->x, head->y, SNAKE_HEAD_COLOR);
		return true;
	}
	return false;
}

//移动蛇（注意不要让蛇向后走了）,移动前把蛇尾涂黑，蛇头涂成蛇身颜色，然后再涂蛇头
void moveSnake(enum D direction)
{
	int tx, ty;

	tempD = judgeDirection(direction);

	//尾推法移动蛇
	body = tail;	//下面要用到tail

	//把蛇尾涂黑 需要判断食物是不是在尾部，在尾部就不涂黑。这里解决的是蛇的移动的滞后问题
	if (!(food->x == tail->x && food->y == tail->y))
	{
		printPix(tail->x, tail->y, BG_COLOR);
	}

	//移动到蛇头，把蛇头涂成蛇身颜色
	printPix(head->x, head->y, SNAKE_BODY_COLOR);
	while (body->last != NULL)
	{
		body = body->last;
	}  //while  end

	//重复利用空间，别浪费了！
	//下面这四行顺序不能变
	head->last = tail;
	tail = tail->last;
	head = head->last;
	head->last = NULL;

	//下面只需要根据方向设置蛇头的位置就行了
	//此时body指向head后第一个body
	tx = body->x;
	ty = body->y;
	switch (tempD)
	{
		case UP:
			ty--;
			break;

		case DOWN:
			ty++;
			break;

		case LEFT:
			tx--;
			break;

		case RIGHT:
			tx++;
			break;

		default:
			NULL;
			break;
	}  //switch  end
	head->x = tx;
	head->y = ty;
	//最后涂上蛇头的颜色
	printPix(head->x, head->y, SNAKE_HEAD_COLOR);
}

//检测有没有撞到墙
bool knockWall(void)
{
	int tx = head->x, ty = head->y;
	if (tx == 0 || tx == (WIDTH - 1) || ty == 0 || ty == (HEIGHT - 1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//检测有没有撞到自己的身体
bool biteSelf(void)
{
	int tx = head->x, ty = head->y;
	body = tail;
	while (body->last != NULL)
	{
		if (body->x == tx && body->y == ty)
		{
			return true;
		}
		body = body->last;
	}  //while  end
	return false;
}

//释放蛇的内存
void freeSnake(void)
{
	body = tail;
	while (body != NULL)
	{
		tail = tail->last;
		free(body);
		body = tail;
	}  //while  end
	//free(tail);
	tail = NULL;
	body = NULL;
	head = NULL;
}

