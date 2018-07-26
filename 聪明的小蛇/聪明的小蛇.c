#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#define LEFT  0x4B00
#define RIGHT 0x4D00
#define UP    0x4800
#define DOWN  0x5000
#define ESC   0x011B
#define ENTER 0x1C0D
#define LEN sizeof(struct snake)
struct snake {//定义蛇的结构体
	int x;
	int y;
	struct snake*next;
}*head, *food, *poison[4], *bomb[4], *q;//蛇头，食物，毒草，遍历指针
struct number {
	int n;
};
void Goto();
void printmap();
void initsnake();
int bite();
void productfood();
void productpoison();
void productpoison_2();
void productpoison_3();
void productbomb();
void productbomb_2();
void productbomb_3();
void cantacross();
void cantdouble();
void snakemove();
void pause();
void gamecontrol();
void welcometogame();
void endgame_s();
void gamestart();
void save(); 
void read();
void sort();
void color();// 声明全部函数

int score =2000, add = 50, reduce = 40, superreduce = 200;//总得分与每次分数变化
int DIR, sleeptime = 200;//蛇状态及每次运行的时间间隔
int endgames = 0, choice, nodes = 0, Time = 0, judge[4] = { 0,0,0,0 }; //游戏结束的情况，选择,节数及毒草状态
void Goto(int x, int y) {//设置光标位置以便打印边框食物毒草及地雷
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}
void color(color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void printmap() {//创建地图
	int i;
	color(11);
	for (i = 0;i < 60;i = i + 2) {//打印上下边框
		Goto(i, 0);
		printf("◆");
		Goto(i, 24);
		printf("◆");
	}
	for (i = 1;i < 24;i++) {//打印左右边框
		Goto(0, i);
		printf("◆");
		Goto(58, i);
		printf("◆");
	}
	switch (choice) {
	case('2') : {
		for (i = 1;i < 10; i++) {
			Goto(20, i);
			printf("◆");
		}
		for (i = 14;i < 24;i++) {
			Goto(42, i);
			printf("◆");
		}
	}break;
	case('3') : {
		for (i = 1;i < 10; i++) {
			Goto(20, i);
			printf("◆");
		}
		for (i = 14;i < 24;i++) {
			Goto(42, i);
			printf("◆");
		}
		for (i = 0;i < 28; i = i + 2) {
			Goto(i, 18);
			printf("◆");
		}
		for (i = 30;i < 58;i = i + 2) {
			Goto(i, 8);
			printf("◆");
		}
	}break;
	
	}
}
void initsnake() {//初始化蛇身
	struct snake *tail;
	int i;
	tail = (struct snake*)malloc(LEN);//从蛇尾开始，加头法，以x,y设定开始的位置//
	tail->x = 24;
	tail->y = 5;
	tail->next = NULL;
	nodes = 1;
	for (i = 1;i <= 4;i++) {
		head = (struct snake*)malloc(LEN);
		head->next = tail;
		head->x = 24 + 2 * i;
		head->y = 5;
		tail = head;
		nodes++;
	}
	while (tail != NULL) {//从头到尾，输出蛇身
		Goto(tail->x, tail->y);
		color(9);
		printf("■");
		tail = tail->next;
	}
}
int bite() {//判断是否咬到了自己
	struct snake *self;
	self = head->next;
	while (self != NULL) {
		if (self->x == head->x && self->y == head->y) {
			return 1;
		}
		self = self->next;
	}
	return 0;
}
void productfood() {//随机出现食物
	srand((unsigned)time(NULL));//随机函数
	food = (struct snake*)malloc(LEN);
	color(9);
	switch (choice) {
	case('1') : {
		
		while ((food->x % 2) != 0) {//保证坐标其为偶数，使得食物能与蛇头对齐
			food->x = rand() % 54 + 2;//防止食物出现在地图之外
		}
		food->y = rand() % 22 + 1;
		q = head;
		while (q->next != NULL) {
			if (q->x == food->x && q->y == food->y) { //防止蛇身与食物重合
				free(food);//清除食物
				productfood();
			}
			q = q->next;
		}
		Goto(food->x, food->y);
		printf("■");//输出食物
	}break;
	case('2') : {
	
		while ((food->x % 2) != 0) {
			food->x = rand() % 54 + 2;
		}
		food->y = rand() % 22 + 1;
		while ((food->x == 20 && food->y < 10 && food->y > 1) || (food->x == 42 && food->y >= 14 && food->y < 24)) {
			free(food);
			while ((food->x % 2) != 0) {
				food->x = rand() % 54 + 2;
			}
			food->y = rand() % 22 + 1;
		}
		q = head;
		while (q->next != NULL) {
			if (q->x == food->x && q->y == food->y) {
				free(food);
				while ((food->x % 2) != 0) {
					food->x = rand() % 54 + 2;
				}
				food->y = rand() % 22 + 1;
			}
			q = q->next;
		}
		Goto(food->x, food->y);
		printf("■");
	}break;
	case('3') : {
		
		while ((food->x % 2) != 0) {
			food->x = rand() % 54 + 2;
		}
		food->y = rand() % 22 + 1;
		while ((food->x == 20 && food->y < 10 && food->y > 1) || (food->x == 42 && food->y >= 14 && food->y < 24) || (food->y == 18 && food->x >= 0 && food->x <= 26) || (food->y == 8 && food->x >= 30 && food->x <= 56)) {
			free(food);
			while ((food->x % 2) != 0) {
				food->x = rand() % 54 + 2;
			}
			food->y = rand() % 22 + 1;
		}
		q = head;
		while (q->next != NULL) {
			if (q->x == food->x && q->y == food->y) {
				free(food);
				while ((food->x % 2) != 0) {
					food->x = rand() % 54 + 2;
				}
				food->y = rand() % 22 + 1;
			}
			q = q->next;
		}
		Goto(food->x, food->y);
		printf("■");
	}break;
	default:break;
	}
}
void productpoison() {//随机出现毒草
	srand((unsigned)time(NULL));//随机函数
	color(14);
	switch (choice) {
	case('1') : {
		poison[0] = (struct snake*)malloc(LEN);
		while ((poison[0]->x % 2) != 0) {//保证坐标其为偶数，使得毒草能与蛇头对齐
			poison[0]->x = rand() % 54 + 2;//防止毒草出现在地图之外
		}
		poison[0]->y = rand() % 22 + 1;
		q = head;
		while (q->next != NULL) {
			if (q->x == poison[0]->x && q->y == poison[0]->y) { //防止蛇身与毒草重合
				free(poison[0]);//清除
				while ((poison[0]->x % 2) != 0) {
					poison[0]->x = rand() % 54 + 2;
				}
				poison[0]->y = rand() % 22 + 1;
			}
			q = q->next;
		}
		Goto(poison[0]->x, poison[0]->y);
		printf("▲");//输出毒草
	}
	case('2') : {
		for (int i = 0; i <= 1;i++) {
			productpoison_2(i);
		}
	}break;
	case('3') : {
		for (int i = 0;i <= 2;i++) {
			productpoison_3(i);
		}
	}break;
	default:break;
	}
}
void productpoison_2(int i) {
	color(14);
	poison[i] = (struct snake*)malloc(LEN);
	while ((poison[i]->x % 2) != 0) {
		poison[i]->x = rand() % 54 + 2;
	}
	poison[i]->y = rand() % 22 + 1;
	while ((poison[i]->x == 20 && poison[i]->y < 10 && poison[i]->y > 1) || (poison[i]->x == 42 && poison[i]->y >= 14 && poison[i]->y < 24)) {
		free(poison[i]);
		while ((poison[i]->x % 2) != 0) {
			poison[i]->x = rand() % 54 + 2;
		}
		poison[i]->y = rand() % 22 + 1;
	}
	q = head;
	while (q->next != NULL) {
		if (q->x == poison[i]->x && q->y == poison[i]->y) {
			free(poison[i]);
			productpoison_2(i);
		}
		q = q->next;
	}
	Goto(poison[i]->x, poison[i]->y);
	printf("▲");
}
void productpoison_3(int i) {
	color(14);
	poison[i] = (struct snake*)malloc(LEN);
	while ((poison[i]->x % 2) != 0) {
		poison[i]->x = rand() % 54 + 2;
	}
	poison[i]->y = rand() % 22 + 1;
	while ((poison[i]->x == 20 && poison[i]->y < 10 && poison[i]->y > 1) || (poison[i]->x == 42 && poison[i]->y >= 14 && poison[i]->y < 24) || (poison[i]->y == 18 && poison[i]->x >= 0 && poison[i]->x <= 26) || (poison[i]->y == 8 && poison[i]->x >= 30 && poison[i]->x <= 56)) {
		free(poison[i]);
		while ((poison[i]->x % 2) != 0) {
			poison[i]->x = rand() % 54 + 2;
		}
		poison[i]->y = rand() % 22 + 1;
	}
	q = head;
	while (q->next != NULL) {
		if (q->x == poison[i]->x && q->y == poison[i]->y) {
			free(poison[i]);
			productpoison_3(i);
		}
		q = q->next;
	}
	Goto(poison[i]->x, poison[i]->y);
	printf("▲");
}
void productbomb() {//随机出现地雷
	int i = 0;
	srand((unsigned)time(NULL));//随机函数
	color(12);
	switch (choice) {
	case('1') : {
		bomb[0] = (struct snake*)malloc(LEN);
		while ((bomb[0]->x % 2) != 0) {//保证坐标其为偶数，使得地雷能与蛇头对齐
			bomb[0]->x = rand() % 54 + 2;//防止地雷出现在地图之外
		}
		bomb[0]->y = rand() % 22 + 1;
		q = head;
		while (q->next != NULL) {
			if (q->x == bomb[0]->x && q->y == bomb[0]->y) { //防止蛇身与地雷重合
				free(bomb[0]);
				productbomb();
			}
			q = q->next;
		}
		Goto(bomb[0]->x, bomb[0]->y);
		printf("●");//输出地雷
	}break;
	case('2') : {
		for (i = 0;i <= 1;i++)
			productbomb_2(i);
	}break;
	case('3') : {
		for (i = 0;i <= 1;i++)
			productbomb_3(i);
	}break;
	default:break;
	}
}
void productbomb_2(int i) {//第二关生产第i个炸弹
	color(12);
	bomb[i] = (struct snake*)malloc(LEN);
	while ((bomb[i]->x % 2) != 0) {
		bomb[i]->x = rand() % 54 + 2;
	}
	bomb[i]->y = rand() % 22 + 1;
	while ((bomb[i]->x == 20 && bomb[i]->y < 10 && bomb[i]->y > 1) || (bomb[i]->x == 42 && bomb[i]->y >= 14 && bomb[i]->y < 24)) {
		free(bomb[i]);
		while ((bomb[i]->x % 2) != 0) {
			bomb[i]->x = rand() % 54 + 2;
		}
		bomb[i]->y = rand() % 22 + 1;
	}
	q = head;
	while (q->next != NULL) {
		if (q->x == bomb[i]->x && q->y == bomb[i]->y) {
			free(bomb[i]);
			productbomb_2(i);
		}
		q = q->next;
	}
	Goto(bomb[i]->x, bomb[i]->y);
	printf("●");//输出地雷
}
void productbomb_3(int i) {//第三关生产第i个炸弹
	color(12);
	bomb[i] = (struct snake*)malloc(LEN);
	while ((bomb[i]->x % 2) != 0) {
		bomb[i]->x = rand() % 54 + 2;
	}
	bomb[i]->y = rand() % 22 + 1;
	while ((bomb[i]->x == 20 && bomb[i]->y < 10 && bomb[i]->y > 1) || (bomb[i]->x == 42 && bomb[i]->y >= 14 && bomb[i]->y < 24) || (bomb[i]->y == 18 && bomb[i]->x >= 0 && bomb[i]->x <= 26) || (bomb[i]->y == 8 && bomb[i]->x >= 30 && bomb[i]->x <= 56)) {
		free(bomb[i]);
		while ((bomb[i]->x % 2) != 0) {
			bomb[i]->x = rand() % 54 + 2;
		}
		bomb[i]->y = rand() % 22 + 1;
	}
	q = head;
	while (q->next != NULL) {
		if (q->x == bomb[i]->x && q->y == bomb[i]->y) {
			free(bomb[i]);
			productbomb_3(i);
		}
		q = q->next;
	}
	Goto(bomb[i]->x, bomb[i]->y);
	printf("●");
}
void cantacross() {//不能穿墙
	switch (choice) {
	case('1') : if (head->x == 0 || head->x == 58 || head->y == 0 || head->y == 24) {//蛇头与墙壁重合
		endgames = 1;
		endgame_s();
	}break;
	case('2') : if (head->x == 0 || head->x == 58 || head->y == 0 || head->y == 24 || (head->x == 20 && head->y < 10 && head->y > 1) || (head->x == 42 && head->y >= 14 && head->y < 24)) {
		endgames = 1;
		endgame_s();
	}break;
	case('3') : if (head->x == 0 || head->x == 58 || head->y == 0 || head->y == 24 || (head->x == 20 && head->y < 10 && head->y > 1) || (head->x == 42 && head->y >= 14 && head->y < 24) || (head->y == 18 && head->x >= 0 && head->x <= 26) || (head->y == 8 && head->x >= 30 && head->x <= 56)) {
		endgames = 1;
		endgame_s();
	}break;
	default:break;
	}
}
void cantdouble() {//防止重合
	switch (choice) {
	case('1') : {
		while (food->x == poison[0]->x&&food->y == poison[0]->y) {//食物与毒草
			Goto(food->x, food->y);
			color(9);
			printf("■");
			free(poison[0]);
			productpoison();
		}
		while (bomb[0]->x == poison[0]->x&&bomb[0]->y == poison[0]->y) {//地雷与毒草
			Goto(bomb[0]->x, bomb[0]->y);
			color(12);
			printf("●");
			free(poison[0]);
			productpoison();
		}

		while (food->x == bomb[0]->x&&food->y == bomb[0]->y) {//食物与地雷
			Goto(food->x, food->y);
			color(9);
			printf("■");
			free(bomb[0]);
			productbomb();
		}
	}break;
	case('2') : {
		for (int i = 0;i <= 1;i++) {
			while (food->x == poison[i]->x&&food->y == poison[i]->y) {//食物与毒草
				Goto(poison[i]->x, poison[i]->y);
				color(14);
				printf("▲");
				free(food);
				productfood();
			}
		}
		for (int i = 0;i <= 1;i++) {//地雷与毒草
			for (int n = 0;n <= 1;n++) {
				while (bomb[i]->x == poison[n]->x&&bomb[i]->y == poison[n]->y) {
					Goto(poison[n]->x, poison[n]->y);
					color(14);
					printf("▲");
					free(bomb[i]);
					productbomb_2(i);
				}
			}
		}
		while (poison[0]->x == poison[1]->x&&poison[0]->y == poison[1]->y) {//毒草与毒草
			Goto(poison[1]->x, poison[1]->y);
			color(14);
			printf("▲");
			free(poison[0]);
			productpoison_2(0);
		}
		for (int i = 0;i <= 1;i++) {
			while (food->x == bomb[i]->x&&food->y == bomb[i]->y) {//食物与地雷
				Goto(bomb[i]->x, bomb[i]->y);
				color(12);
				printf("●");
				free(food);
				productfood();
			}
		}

		while (bomb[0]->x == bomb[1]->x&&bomb[0]->y == bomb[1]->y) {//地雷与地雷
			Goto(bomb[1]->x, bomb[1]->y);
			color(12);
			printf("●");
			free(bomb[0]);
			productbomb_2(0);
		}
	}break;
	case('3') : {
		for (int i = 0;i <= 2;i++) {
			while (food->x == poison[i]->x&&food->y == poison[i]->y) {//食物与毒草
				Goto(food->x, food->y);
				color(9);
				printf("■");
				free(poison[i]);
				productpoison_3(i);
			}
		}
		for (int i = 0;i <= 1;i++) {//地雷与毒草
			for (int n = 0;n <= 2;n++) {
				while (bomb[i]->x == poison[n]->x&&bomb[i]->y == poison[n]->y) {
					Goto(poison[n]->x, poison[n]->y);
					color(14);
					printf("▲");
					free(bomb[i]);
					productbomb_3(i);
				}
			}
		}
		for (int i = 0;i <= 1;i++) {
			while (food->x == bomb[i]->x&&food->y == bomb[i]->y) {//食物与地雷
				Goto(food->x, food->y);
				color(9);
				printf("■");
				free(bomb[i]);
				productbomb_3(i);
			}
		}
		for (int i = 1;i <= 2;i++) {
			while (poison[0]->x == poison[i]->x&&poison[0]->y == poison[i]->y) {//毒草与毒草
				Goto(poison[i]->x, poison[i]->y);
				color(14);
				printf("▲");
				free(poison[0]);
				productpoison_3(0);
			}
		}
		while (poison[1]->x == poison[2]->x&&poison[1]->y == poison[2]->y) {
			Goto(poison[2]->x, poison[2]->y);
			color(14);
			printf("▲");
			free(poison[1]);
			productpoison_3(1);
		}
		while (bomb[0]->x == bomb[1]->x&&bomb[0]->y == bomb[1]->y) {//地雷与地雷
			Goto(bomb[1]->x, bomb[1]->y);
			color(12);
			printf("●");
			free(bomb[0]);
			productbomb_3(0);
		}
	}break;
	default:break;
	}
}
void snakemove() {//蛇前进,上UP,下DOWN,左LEFT,右RIGHT
	int i = 0, n = 0;
	struct snake * nexthead;//头部前方
	cantacross();
	nexthead = (struct snake*)malloc(LEN);
	if (DIR == UP) {
		nexthead->x = head->x;
		nexthead->y = head->y - 1;
	}
	else if (DIR == DOWN) {
		nexthead->x = head->x;
		nexthead->y = head->y + 1;
	}
	else if (DIR == LEFT) {
		nexthead->x = head->x - 2;
		nexthead->y = head->y;
	}
	else if (DIR == RIGHT) {
		nexthead->x = head->x + 2;
		nexthead->y = head->y;
	}

	switch (choice) {
	case('1') : {
		if (Time < 50 && nexthead->x == poison[0]->x && nexthead->y == poison[0]->y) {//有毒草
			judge[0] = 1;
			Goto(poison[0]->x, poison[0]->y);
			printf(" ");
			free(poison[0]);
			if (nodes == 1) {
				endgames = 5;
				score = score - reduce;
				endgame_s();
			}
			else {
				q = head;
				while (q->next->next != NULL) {
					q = q->next;
				}
				Goto(q->next->x, q->next->y);
				printf("  ");
				free(q->next);
				q->next = NULL;
				nodes--;

			}
			score = score - reduce;
		}
		if (nexthead->x == food->x && nexthead->y == food->y) {//有食物
			nexthead->next = head;
			head = nexthead;
			q = head;
			Goto(q->x, q->y);
			color(9);
			printf("■");
			nodes++;
			score = score + add;
			productfood();
		}
		else if (nexthead->x == bomb[0]->x && nexthead->y == bomb[0]->y) {//有地雷
			if (nodes == 1) {
				endgames = 4;
				score = score - superreduce;
				endgame_s();
			}
			else {
				for (i = 1;i <= nodes / 2;i++) {
					q = head;
					while (q->next->next != NULL) {
						q = q->next;
					}
					Goto(q->next->x, q->next->y);
					printf("  ");
					free(q->next);
					q->next = NULL;
				}
				nodes = nodes - nodes / 2;
			}

			score = score - superreduce;
			productbomb();
		}
		else {                                     //没有食物，加头去尾
			nexthead->next = head;
			head = nexthead;
			q = head;
			Goto(q->x, q->y);
			color(9);
			printf("■");
			q = head;
			while (q->next->next != NULL) {
				q = q->next;
			}
			Goto(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}break;
	case('2') : {
		for (n = 0;n <= 1;n++) {
			if (Time < 50 && nexthead->x == poison[n]->x && nexthead->y == poison[n]->y) {//有毒草
				judge[n] = 1;
				Goto(poison[n]->x, poison[n]->y);
				printf(" ");
				if (nodes == 1) {
					endgames = 5;
					score = score - reduce;
					endgame_s();
				}
				else {
					q = head;
					while (q->next->next != NULL) {
						q = q->next;
					}
					Goto(q->next->x, q->next->y);
					printf("  ");
					free(q->next);
					q->next = NULL;
					nodes--;
				}
				score = score - reduce;
			}
			else if (nexthead->x == bomb[n]->x && nexthead->y == bomb[n]->y) {//有地雷
				if (nodes == 1) {
					endgames = 4;
					score = score - superreduce;
					endgame_s();
				}
				else {
					for (i = 1;i <= nodes / 2;i++) {
						q = head;
						while (q->next->next != NULL) {
							q = q->next;
						}
						Goto(q->next->x, q->next->y);
						printf("  ");
						free(q->next);
						q->next = NULL;
					}
					nodes = nodes - nodes / 2;
				}

				score = score - superreduce;
				productbomb_2(n);
			}
		}
		if (nexthead->x == food->x && nexthead->y == food->y) {//有食物
			nexthead->next = head;
			head = nexthead;
			q = head;
			color(9);
			Goto(q->x, q->y);
			printf("■");
			nodes++;
			score = score + add;
			productfood();
		}
		else {                                     //没有食物，加头去尾
			nexthead->next = head;
			head = nexthead;
			q = head;
			color(9);
			Goto(q->x, q->y);
			printf("■");
			q = head;
			while (q->next->next != NULL) {
				q = q->next;
			}
			Goto(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}break;
	case('3') : {
		for (n = 0;n <= 2;n++) {
			if (Time < 50 && nexthead->x == poison[n]->x && nexthead->y == poison[n]->y) {//有毒草
				judge[n] = 1;
				Goto(poison[n]->x, poison[n]->y);
				printf(" ");
				if (nodes == 1) {
					endgames = 5;
					score = score - reduce;
					endgame_s();
				}
				else {
					q = head;
					while (q->next->next != NULL) {
						q = q->next;
					}
					Goto(q->next->x, q->next->y);
					printf("  ");
					free(q->next);
					q->next = NULL;
					nodes--;
				}
				score = score - reduce;
			}
		}
		for (int n = 0;n <= 1;n++) {
			if (nexthead->x == bomb[n]->x && nexthead->y == bomb[n]->y) {//有地雷
				if (nodes == 1) {
					endgames = 4;
					score = score - superreduce;
					endgame_s();
				}
				else {
					for (i = 1;i <= nodes / 2;i++) {
						q = head;
						while (q->next->next != NULL) {
							q = q->next;
						}
						Goto(q->next->x, q->next->y);
						printf("  ");
						free(q->next);
						q->next = NULL;
					}
					nodes = nodes - nodes / 2;
				}
				score = score - superreduce;
				productbomb_3(n);
			}
		}
		if (nexthead->x == food->x && nexthead->y == food->y) {//有食物
			nexthead->next = head;
			head = nexthead;
			q = head;
			Goto(q->x, q->y);
			color(9);
			printf("■");
			nodes++;
			score = score + add;
			productfood();
		}
		else {                                     //没有食物，加头去尾
			nexthead->next = head;
			head = nexthead;
			q = head;
			Goto(q->x, q->y);
			color(9);
			printf("■");
			q = head;
			while (q->next->next != NULL) {
				q = q->next;
			}
			Goto(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}break;
	default:break;
	}

	if (bite() == 1) {//判断是否会咬到自己
		endgames = 2;
		endgame_s();
	}
}
void pause() {//暂停
	while (1) {
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE)) {
			break;
		}
	}
}
void gamecontrol() {//控制游戏        
	color(14);
	Goto(64, 15);
	printf("不能穿墙，不能咬到自己.");
	Goto(64, 16);
	printf("■为食物,▲为毒草,●为地雷.");
	Goto(64, 17);
	printf("用↑.↓.←.→分别控制蛇的移动.");
	Goto(64, 18);
	printf("ESC ：退出游戏.SPACE：暂停游戏.");
	DIR = RIGHT;
	while (1)
	{
		cantdouble();
		switch (choice) {
		case('1') : {
			Sleep(250);
			add = 50;
		}break;
		case('2') : {
			Sleep(200);
			add = 100;
		}break;
		case('3') : {
			Sleep(150);
			add = 150;
		}break;
		default:break;
		}
		color(13);
		Goto(64, 10);
		printf("得分：%d  ", score);
		Goto(64, 11);
		printf("每个食物得分：%d分", add);
		Goto(64, 12);
		printf("每颗毒草减分：%d分", reduce);
		Goto(64, 13);
		printf("每个地雷减分：%d分", superreduce);
		if (GetAsyncKeyState(VK_UP) && DIR != DOWN) {//判断按键输入并确保不会立刻反向运动
			DIR = UP;
		}
		else if (GetAsyncKeyState(VK_DOWN) && DIR != UP) {
			DIR = DOWN;
		}
		else if (GetAsyncKeyState(VK_LEFT) && DIR != RIGHT) {
			DIR = LEFT;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && DIR != LEFT) {
			DIR = RIGHT;
		}
		else if (GetAsyncKeyState(VK_SPACE)) {
			pause();
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			endgames = 3;
			break;
		}
		color(7);
		switch (choice) {
		case('1') : {
			Sleep(250);
			if (Time % 2 == 0 && Time < 50 && judge[0] == 0) {
				Goto(poison[0]->x, poison[0]->y);
				printf(" ");
			}
			else if (Time % 2 != 0 && Time < 50 && judge[0] == 0) {
				Goto(poison[0]->x, poison[0]->y);
				color(14);
				printf("▲");
			}
			if (Time == 50 && judge[0] == 0) {
				Goto(poison[0]->x, poison[0]->y);
				printf(" ");
			}
		}break;
		case('2') : {
			for (int i = 0;i <= 1;i++) {
				if (Time % 2 == 0 && Time < 50 && judge[i] == 0) {
					Goto(poison[i]->x, poison[i]->y);
					printf(" ");
				}
				else if (Time % 2 != 0 && Time < 50 && judge[i] == 0) {
					Goto(poison[i]->x, poison[i]->y);
					color(14);
					printf("▲");
				}

				if (Time == 50 && judge[i] == 0) {
					Goto(poison[i]->x, poison[i]->y);
					printf(" ");
				}
			}
		}break;
		case('3') : {
			for (int i = 0;i <= 2;i++) {
				if (Time % 2 == 0 && Time <50 && judge[i] == 0) {
					Goto(poison[i]->x, poison[i]->y);
					printf(" ");
				}
				else if (Time % 2 != 0 && Time<50 && judge[i] == 0) {
					Goto(poison[i]->x, poison[i]->y);
					color(14);
					printf("▲");
				}

				if (Time == 50 && judge[i] == 0) {
					Goto(poison[i]->x, poison[i]->y);
					printf(" ");
				}
			}
		}break;
		default:break;
		}
		snakemove();
		Time++;
	}
}
void welcometogame() {//开始界面
	color(13);
	Goto(38, 2);
	printf("欢迎来玩聪明的小蛇游戏");
	Goto(42, 6);
	printf("1、第一关（简单）");
	Goto(42, 10);
	printf("2、第二关（一般）");
	Goto(42, 14);
	printf("3、第三关（较难）");
	Goto(42, 18);
	printf("4、排行榜");
	Goto(60, 24);
	printf("软件工程1504，刘博谦");
	choice = _getch();
	system("cls");
}
void endgame_s() {//结束游戏
	system("cls");
	color(11);
	Goto(24, 12);
	if (endgames == 1) {
		printf("对不起,您被撞死了.");
	}
	else if (endgames == 2) {
		printf("对不起， 您被咬死了.");
	}
	else if (endgames == 3) {
		printf("对不起，您干掉了自己。");
	}
	else if (endgames == 4) {
		printf("对不起，您被炸死了。");
	}
	else if (endgames == 5) {
		printf("对不起，您被毒死了。");
	}
	save(score);
	Goto(24, 14);
	printf("GAME OVER！\n", score);
	Goto(24, 15);
	printf("您的得分是%d\n", score);
	//system("pause");
	exit(0);
	
}
void gamestart() {//游戏初始化
	system("mode con cols=110 lines=32");//设定控制台宽度和高度
	welcometogame();
	printmap();
	initsnake();
	productfood();
	productbomb();
	productpoison();
	cantdouble();
}
int main() {
	gamestart();
	if (choice == '1' || choice == '2' || choice == '3') {
		gamecontrol();
		endgame_s();
		return 0;
	}
	else if (choice == '4') {
		system("cls");
		read();
		if (GetAsyncKeyState(VK_ESCAPE))
			exit(0);
	}
}
void save(int score){
	struct number a;
	a.n = score;
	FILE *fp;
	if ((fp = fopen("scorefile.dat", "a+")) == NULL)
		printf("ERROR OPEN\n");

	if (fwrite(&a, sizeof(struct number), 1, fp) != 1)
		printf("EOORO WRITE");
	fclose(fp);
}
void read()//该段代码的使用需要save（）已经存入5个数据
{
	void sort(struct number a[], int);
	color(9);
	FILE *gp;
	struct number b[100], *p = b;
	if ((gp = fopen("scorefile.dat", "rb")) == NULL)
		printf("ERROR READ");
	int m = 0;
	while (!feof(gp))
	{
		if (fread(&b[m], sizeof(struct number), 1, gp) != 1)
		{
			Goto(45, 2);
			printf("排  行  榜\n");
		}
		m++;
	}
	sort(p, m - 1);
	for (int w = 0;w < 5;w++)
	{
		char s[5];
		sprintf(s, "%d", b[w].n);//输出分数
		Goto(44, 6 + w * 4);
		printf("No. %d.   %s", w + 1, s);
	}
	Goto(35, 25);
	getch();
	fclose(gp);
}
void sort(struct number c[], int m){
	int i, j, k, t;
	for (i = 0;i<m - 1;i++)
	{
		k = i;
		for (j = i + 1;j<m;j++)
			if (c[j].n>c[k].n)
				k = j;
		t = c[k].n;c[k].n = c[i].n;c[i].n = t;
	}
}