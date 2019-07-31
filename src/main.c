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

#define WIDTH 26    //��ͼ�Ŀ��
#define HEIGHT 20    //��ͼ�ĸ߶�

//�����ĸ�������Ƽ���һ����ͣ��
#define KEY_UP '8'    //���ϼ�
#define KEY_DOWN '2'    //���¼�
#define KEY_LEFT '4'    //�����
#define KEY_RIGHT '6'    //���Ҽ�
#define KEY_SUSPEND '5'    //��ͣ��

//����ǽ���ߣ�ʳ��ͱ�������ɫ
#define FOOD_COLOR 0x0060
#define SNAKE_BODY_COLOR 0x0010
#define SNAKE_HEAD_COLOR 0x0040
#define WALL_COLOR	0x00f0
#define BG_COLOR 0x0000

//������Ϸ�ٶ�
const int GAME_SPEED = 24;

//����ת��ķ���
enum D { UP, RIGHT, DOWN, LEFT };
//���嵱ǰ���������ƶ�ת��ʱ�Ƚ�
enum D tempD = RIGHT;

typedef struct snake   //�洢������Ϣ
{
	int x;  //����ڵ� x ����
	int y;  //����ڵ� y ����
	struct snake *last;  //��һ����ڵ��ַ
}snake;

//��ͼ��¼��ֻ��Ҫ��¼һ�����ξͺ���
typedef struct game_map
{
	int startX;
	int startY;
	int endX;
	int endY;
}Map;
Map map = {0, 0, WIDTH-1, HEIGHT-1};

//�����ߺ�ʳ���ָ�룬ʳ��Ľṹ��ʵ��������Ľṹһ�� 
snake *tail, *body, *head, *food;

int snake_length = 3;  //�ߵĳ��ȣ���ʼ��Ϊ3


//������Ϸ
void start(void);

//��ʼ����
void initSnake(void);

//�����ͼ���ߵ�����
void cleanSnake(void);

//�����򣬷�����ȷ����
enum D judgeDirection(enum D direction);

//������������
void cleanInput(void);

//��ȡ����ķ���
enum D getDirection(void);

//�ж���û�гԵ�ʳ��Ե��˾����ӳ���
//�Ե�ʳ�ﷵ�� true�����򷵻� false
bool eatFood(enum D direction);

//�ƶ��ߣ�ע�ⲻҪ����������ˣ�
void moveSnake(enum D direction);

//�����û��ײ��ǽ
bool knockWall(void);

//�����û��ײ���Լ�������
bool biteSelf(void);

//�ͷ��ߵ��ڴ�
void freeSnake(void);

//��ǽ��һ����Ϸֻ��Ҫ��һ��ǽ
void printWall(void);

//����
void printSnake(void);

//��ʳ��
void printFood(void);


int main(void)
{
	initOutput();

	start();

	return 0;
}

//������Ϸ
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

			//���Ѿ�������������ͼ֮����˳�
			if (snake_length >= size)
			{
				printSnake();
				break;
			}

			printFood();
		}
		else
		{
			//����������ߵ�����
			moveSnake(tempD);
		}

		//ײ��ǽ����ҧ���Լ������¿�ʼ
		if (knockWall() || biteSelf())
		{
			//��ʾ��Ϸ�Ѿ�����
			cleanInput();
			Sleep(1);
			setPos(0, map.endY + 1);
			setColor(0x000f);
			printf("\nYou lose!!\nAnd you got %d point(s)", snake_length - 3);
			printf("\nPress any key to play again...");
			_getch();

			//�����Ļ�ϵĻ���
			system("cls");
			cleanSnake();
			freeSnake();
			free(food);

			//���³�ʼ���ߣ����¿�ʼ��Ϸ
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

//��ʼ����,���ߵĳ�ʼ���ȵ�����Ҳ������
void initSnake(void)
{
	//�߳�ʼ��λ��
	int x, y;
	x = 2;
	y = 1;

	//��ȡ�ڴ�ռ�
	tail = (snake *)malloc(sizeof(snake));
	body = (snake *)malloc(sizeof(snake));
	head = (snake *)malloc(sizeof(snake));

	//��λ����Ϣд��������
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

//�жϸõ��Ƿ��Ѿ��ܿ��ߵ�λ�ã��ܿ�������
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

//��ӡ�ߣ����������ӡ����
void printSnake(void)
{
	body = tail;	//���ܶ�tail����
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

//�жϸõ��Ƿ��Ѿ��ܿ�ǽ��λ�ã��ܿ�������
bool avoidWall(int x, int y)
{
	if (x == 0 || y == 0 || x == map.endX || y == map.endY)
	{
		return false;
	}
	return true;
}

//��¼����ӡһ��ʳ��
void printFood(void)
{
	int tx, ty;
	food = (snake *)malloc(sizeof(snake));

	//��ȡһ�������λ�ã�����Ҫ�ܿ�ǽ���ߵ�λ��
	srand((unsigned)time(NULL));
	do
	{
		tx = (rand() % (WIDTH - 2)) + 1;
		ty = (rand() % (HEIGHT - 2)) + 1;
	} while (!avoidSnake(tx, ty) && !avoidWall(tx, ty));	//�ܿ��ߺ�ǽ

	food->x = tx;
	food->y = ty;
	food->last = NULL;
	printPix(tx, ty, FOOD_COLOR);
}

//��ǽ
void printWall(void)
{
	//�����±�Ե
	for (int i = 0; i < WIDTH; i++)
	{
		printPix(i, 0, WALL_COLOR);
		printPix(i, map.endY, WALL_COLOR);
	}
	//�����ұ�Ե���ظ������ĸ���Ҳû��ϵ
	for (int i = 0; i < HEIGHT; i++)
	{
		printPix(0, i, WALL_COLOR);
		printPix(map.endX, i, WALL_COLOR);
	}
}

//�����ͼ����ȫ��������
void cleanSnake(void)
{
	body = tail;
	while (body != NULL)
	{
		printPix(body->x, body->y, BG_COLOR);
		body = body->last;
	}  //while  end
}

//�����򣬷�����ȷ����
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

//������������
void cleanInput(void)
{
	while (_kbhit())
	{
		_getch();
	}
}

//��ȡ����ķ���
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

//�ж���û�гԵ�ʳ��Ե��˾����ӳ���,Ȼ�󻭳��ߵ�����
//�Ե�ʳ�ﷵ�� true�����򷵻� flase
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

	//�жϴ�ʱ�Ƿ�Ե�ʳ��
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

//�ƶ��ߣ�ע�ⲻҪ����������ˣ�,�ƶ�ǰ����βͿ�ڣ���ͷͿ��������ɫ��Ȼ����Ϳ��ͷ
void moveSnake(enum D direction)
{
	int tx, ty;

	tempD = judgeDirection(direction);

	//β�Ʒ��ƶ���
	body = tail;	//����Ҫ�õ�tail

	//����βͿ�� ��Ҫ�ж�ʳ���ǲ�����β������β���Ͳ�Ϳ�ڡ������������ߵ��ƶ����ͺ�����
	if (!(food->x == tail->x && food->y == tail->y))
	{
		printPix(tail->x, tail->y, BG_COLOR);
	}

	//�ƶ�����ͷ������ͷͿ��������ɫ
	printPix(head->x, head->y, SNAKE_BODY_COLOR);
	while (body->last != NULL)
	{
		body = body->last;
	}  //while  end

	//�ظ����ÿռ䣬���˷��ˣ�
	//����������˳���ܱ�
	head->last = tail;
	tail = tail->last;
	head = head->last;
	head->last = NULL;

	//����ֻ��Ҫ���ݷ���������ͷ��λ�þ�����
	//��ʱbodyָ��head���һ��body
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
	//���Ϳ����ͷ����ɫ
	printPix(head->x, head->y, SNAKE_HEAD_COLOR);
}

//�����û��ײ��ǽ
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

//�����û��ײ���Լ�������
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

//�ͷ��ߵ��ڴ�
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

