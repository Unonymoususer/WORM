#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define BACK 98
#define OPTION 111
#define HELP 104
#define PAUSE 112
#define STATUS 115
#define ESC 27
#define MAP_ADJ_X 3   //���� X�� ������
#define MAP_ADJ_Y 2   //���� Y�� ������

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

static int MAP_X = 50;      //���� X�� ũ��
static int MAP_Y = 30;		//���� Y�� ũ��
static int reverse_value = 0;
static clock_t reverse_item_effect_start = 0;
static clock_t reverse_item_effect_end = 0;
static clock_t food_item_time_start = 0;
static clock_t reverse_item_time_start = 0;
static clock_t length_item_time_start = 0;
static clock_t speed_item_time_start = 0;
static clock_t trap_item_time_start = 0;
static clock_t food_item_time_end = 0;
static clock_t reverse_item_time_end = 0;
static clock_t length_item_time_end = 0;
static clock_t speed_item_time_end = 0;
static clock_t trap_item_time_end = 0;

int speed = 100;
int accleration = 3;
int x[100], y[100];	//x,y ��ǥ���� ���� �� 100�� 

int food_x, food_y;	//food�� ��ǥ���� ���� 
int reverse_item_x, reverse_item_y;
int speed_up_item_x, speed_up_item_y;
int length_item_x, length_item_y;
int trap_x, trap_y;

int food_item_regeneration = 12;		//��������� 12��
int reverse_item_regeneration = 14;		//����������� 14��
int length_item_regeneration = 16;		//��������� 16��
int speed_item_regeneration = 18;		//���ǵ������ 18��
int trap_item_regeneration = 20;		//��������� 20��
int reverse_item_effect_duration = 5;

int food_item_regeneration_switch = 0;
int reverse_item_regeneration_switch = 0;
int length_item_regeneration_switch = 0;
int speed_item_regeneration_switch = 0;
int trap_item_regeneration_switch = 0;

int food_count = 0;
int length_item_count = 0;
int speed_up_item_count = 0;
int reverse_item_count = 0;
int trap_count = 0;

int length;         //�����̸� ���
int score;			//���� ����, reset�Լ��� ���� �ʱ�ȭ��
int best_score = 0; //�ְ� ���� ����, reset�Լ��� ���� �ʱ�ȭ ���� ���� 
int last_score = 0; //������ ���� ����, reset�Լ��� ���� �ʱ�ȭ ���� ����
int direction;      //�̵����� ���� 
int key;			//�Է¹��� Ű ���� 
int status_on = 0;	//�����ڿ� status ǥ��Ȱ��ȭ ����. �����߿� SŰ�� ������ Ȱ��ȭ
int game_over_count = 0;
static int difficulty = 0;	//���̵� ����. 0: EASY, 1: NORMAL, 2: HARD


void gotoxy(int x, int y, char *s);	//Ŀ���̵� �� �Է�
void movexy(int x, int y);			//Ŀ���̵�
void title(void);      //���� ����ȭ�� 
void reset(void);      //������ �ʱ�ȭ 
void draw_map(void);   //������ �׵θ��� �׸� 
void move(int direction);      //��Ӹ��� �̵� 
void pause(void);      //�Ͻ����� 
void game_over(void);   //���� ������ Ȯ�� 
void food(void);      //���� ���� 
void reverse_item(void);
void trap_item(void);
void length_item(void);
void speed_up_item(void);
void status(void);      //�����ڿ� status ǥ��
void status_off(void);   //�����ڿ� status ����
void option(void);      //option
void help(void);
void setCursorType(CURSOR_TYPE c);


//�����Լ�
int main() {
	system("mode con cols=150 lines=40");
	SetConsoleTitleW(L"������ ����");
	setCursorType(NOCURSOR);
	title(); //���� ����ȭ�� ���

	trap_item();		//�δ���
	food();				//��ǥ	��
	speed_up_item();	//S		S
	length_item();		//Ŭ�ι�	��
	reverse_item();		//�籸��	��


	while (1) {
		gotoxy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y, " YOUR SCORE: ");   //����ǥ�� 
		printf("%5d     LAST SCORE: %5d     BEST SCORE: %5d",
			score, last_score, best_score);

		if (game_over_count == 1) {
			game_over_count = 0;
			trap_item();		//�δ���
			food();				//��ǥ	��
			speed_up_item();	//S		S
			length_item();		//Ŭ�ι�	��
			reverse_item();		//�籸��	��
		}

		if (_kbhit()) {
			do {
				key = _getch();
			} while (key == 224); //Ű �Է¹���
		}
		Sleep(speed);

		//�Է¹��� Ű�� �ľ��ϰ� ����
		switch (key) {
		case LEFT:
		case RIGHT:
		case UP:
		case DOWN:
			if ((direction == LEFT && key != RIGHT) ||
				(direction == RIGHT && key != LEFT) ||
				(direction == UP && key != DOWN) ||
				(direction == DOWN && key != UP)) {
				direction = key;	//180ȸ���̵��� �����ϱ� ���� �ʿ�.
				if (reverse_value == 1) {
					switch (direction) {
					case LEFT:
						direction = RIGHT;
						break;
					case RIGHT:
						direction = LEFT;
						break;
					case UP:
						direction = DOWN;
						break;
					case DOWN:
						direction = UP;
						break;
						if ((direction == LEFT && key == RIGHT) ||
							(direction == RIGHT && key == LEFT) ||
							(direction == UP && key == DOWN) ||
							(direction == DOWN && key == UP)) {
							direction = key;
						}
					}
				}
			}
			key = 0;   //Ű���� �����ϴ� �Լ��� reset 
			break;
		case PAUSE:      //PŰ�� ������ �Ͻ����� 
			pause();
			key = 0;
			break;
		case STATUS:   //SŰ�� ������ �����ڿ� status�� Ȱ��ȭ ��Ŵ  
			if (status_on == 0) status_on = 1;
			else status_on = 0;
			key = 0;
			break;
		case ESC:      //ESCŰ�� ������ ���α׷� ���� 
			gotoxy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y + 1, " ");
			exit(0);
		}
		move(direction);

		if (status_on == 1) {
			status();      //status ǥ��
		}
		else if (status_on == 0) {
			status_off();   //status ����
		}

		food_item_time_end = clock();
		reverse_item_time_end = clock();
		length_item_time_end = clock();
		speed_item_time_end = clock();
		trap_item_time_end = clock();
		reverse_item_effect_end = clock();


		if (food_item_regeneration_switch == 1 &&
			(food_item_time_end - food_item_time_start) / CLOCKS_PER_SEC >
			food_item_regeneration) {
			food();
			food_item_regeneration_switch = 0;
			food_item_time_start = clock();
		}
		if (reverse_item_regeneration_switch == 1 &&
			(reverse_item_time_end - reverse_item_time_start) / CLOCKS_PER_SEC >
			reverse_item_regeneration) {
			reverse_item();
			reverse_item_regeneration_switch = 0;
			reverse_item_time_start = clock();
		}
		if (length_item_regeneration_switch == 1 &&
			(length_item_time_end - length_item_time_start) / CLOCKS_PER_SEC >
			length_item_regeneration) {
			length_item();
			length_item_regeneration_switch = 0;
			length_item_time_start = clock();
		}
		if (speed_item_regeneration_switch == 1 &&
			(speed_item_time_end - speed_item_time_start) / CLOCKS_PER_SEC >
			speed_item_regeneration) {
			speed_up_item();
			speed_item_regeneration_switch = 0;
			speed_item_time_start = clock();
		}
		if ((reverse_item_effect_end - reverse_item_effect_start) / CLOCKS_PER_SEC >
			reverse_item_effect_duration) {
			reverse_value = 0;
			gotoxy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y + 2,
				"                                                                     ");
		}
	}
}


//x���� 2x�� ����, ��ǥ���� �ٷ� ���ڿ��� �Է��� �� �ֵ��� printf�Լ� ����
void gotoxy(int x, int y, char *s) {
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", s);
}


//Ŀ���̵��Լ�. gotoxy���� printf��� ����
void movexy(int x, int y) {
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


//���� ����ȭ�� ���
void title(void) {
	system("cls");   //ȭ���� ����
	int i, j;
	while (_kbhit()) _getch();   //���ۿ� �ִ� Ű���� ����
	draw_map();               //�� �׵θ��� �׸�

							  //�� �׵θ� ������ ��ĭ���� ä��
	for (i = MAP_ADJ_X + 1; i<MAP_ADJ_X + MAP_X; i++) {
		for (j = MAP_ADJ_Y + 1; j<MAP_ADJ_Y + MAP_Y - 1; j++) gotoxy(i, j, "  ");
	}

	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) - 5,
		"+--------------------------+");
	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) - 4,
		"|        S N A K E         |");
	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) - 3,
		"+--------------------------+");

	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) - 1,
		" < PRESS ANY KEY TO START > ");

	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) + 1,
		"   �� ��,��,��,�� : Move");
	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) + 2,
		"   �� P : Pause");
	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) + 3,
		"   �� O : Option");
	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) + 4,
		"   �� H : Help");
	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) + 5,
		"   �� ESC : Quit");

	while (1) {
		//Ű�Է¹���
		if (_kbhit()) {
			key = _getch();
			if (key == OPTION) {
				option();
			}
			if (key == HELP) {
				help();
			}
			if (key == ESC) exit(0);
			else break;   //�ƴϸ� �׳� ��� ����
		}
		gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) - 1,
			" < PRESS ANY KEY TO START > ");
		Sleep(400);
		gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) - 1,
			"                            ");
		Sleep(400);
		gotoxy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y + 1, " ");
	}
	reset();   //������ �ʱ�ȭ  
}


// ����
void reset(void) {
	int i;
	system("cls");   //ȭ���� ����
	draw_map();      //�� �׵θ��� �׸�
	while (_kbhit()) _getch();   //���ۿ� �ִ� Ű���� ����

	direction = LEFT;      //���� �ʱ�ȭ, �ʱ���� ����
	length = 5;            //�� ���� �ʱ�ȭ
	score = 0;            //���� �ʱ�ȭ

						  //�� ���밪 �Է� 
	for (i = 0; i<length; i++) {
		x[i] = MAP_X / 2 + i;
		y[i] = MAP_Y / 2;
		gotoxy(MAP_ADJ_X + x[i], MAP_ADJ_Y + y[i], "��");
	}
	gotoxy(MAP_ADJ_X + x[0], MAP_ADJ_Y + y[0], "��");   //�� �Ӹ� �׸�

	trap_item();			//�δ���

	reverse_item_effect_start = 0;
	reverse_item_effect_end = 0;
	food_item_time_start = 0;
	reverse_item_time_start = 0;
	length_item_time_start = 0;
	speed_item_time_start = 0;
	trap_item_time_start = 0;

	food_item_time_end = 0;
	reverse_item_time_end = 0;
	length_item_time_end = 0;
	speed_item_time_end = 0;
	trap_item_time_end = 0;
}


//�� �׵θ� �׸��� �Լ�
void draw_map(void) {
	int i;
	for (i = 0; i<MAP_X; i++) {
		gotoxy(MAP_ADJ_X + i, MAP_ADJ_Y, "��");
		gotoxy(MAP_ADJ_X + i, MAP_ADJ_Y + MAP_Y - 1, "��");
	}
	for (i = 0; i<MAP_Y; i++) {
		gotoxy(MAP_ADJ_X, MAP_ADJ_Y + i, "��");
		gotoxy(MAP_ADJ_X + MAP_X, MAP_ADJ_Y + i, "��");
	}
}


//��Ӹ��� �̵�
void move(int direction) {
	int i;

	//food�� �浹���� ���
	if (x[0] == food_x && y[0] == food_y) {
		score += 10;   //���� ���� 
		length++;      //��������
		food_count++;
		speed -= accleration;

		x[length - 1] = x[length - 2];   //���θ��� ���뿡 �� �Է�
		y[length - 1] = y[length - 2];
		food_item_time_start = clock();
		food_item_regeneration_switch = 1;

	}

	//speed_up_item�� �浹���� ���
	else if (x[0] == speed_up_item_x && y[0] == speed_up_item_y) {
		speed_up_item_count++;
		speed += accleration * 3;

		speed_item_time_start = clock();
		speed_item_regeneration_switch = 1;

	}

	//reverse_item�� �浹���� ���
	else if (x[0] == reverse_item_x && y[0] == reverse_item_y) {
		reverse_item_count++;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		gotoxy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y + 2,
			"���� �����ۿ� �ɷȽ��ϴ�. ���� �ð����� Ű������ ������ �����˴ϴ�.");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		reverse_value = 1;
		reverse_item_effect_start = clock();
		reverse_item_regeneration_switch = 1;
	}

	//length_item�� �浹���� ���
	else if (x[0] == length_item_x && y[0] == length_item_y) {
		length_item_count++;
		if (length > 3) {
			length--;
			gotoxy(MAP_ADJ_X + x[length], MAP_ADJ_Y + y[length], "  ");
			gotoxy(MAP_ADJ_X + x[length - 1], MAP_ADJ_Y + y[length - 1], "  ");
		}
		else {
			gotoxy(MAP_ADJ_X + x[length], MAP_ADJ_Y + y[length], "  ");
		}

		length_item_time_start = clock();
		length_item_regeneration_switch = 1;

	}

	//trap�� �浹���� ���
	else if (x[0] >= trap_x && x[0] <= trap_x + 3 &&
		y[0] >= trap_y && y[0] <= trap_y + 2) {
		game_over();
		return;
	}

	//���� �浹���� ���
	if (x[0] == 0 || x[0] == MAP_X - 1 || y[0] == 0 || y[0] == MAP_Y - 1) {
		game_over();
		return; //game_over���� ������ �ٽ� �����ϰ� �Ǹ� ���⼭���� �ݺ��ǹǷ� 
				//return�� ����Ͽ� move�� ������ �κ��� ������� �ʵ��� �մϴ�. 
	}

	//�ڱ���� �浹�ߴ��� �˻�
	for (i = 1; i<length; i++) {
		if (x[0] == x[i] && y[0] == y[i]) {
			game_over();
			return;
		}
	}

	//���� �������� ����
	gotoxy(MAP_ADJ_X + x[length - 1], MAP_ADJ_Y + y[length - 1], "  ");

	//������ǥ�� ��ĭ�� �ű�
	for (i = length - 1; i>0; i--) {
		x[i] = x[i - 1];
		y[i] = y[i - 1];
	}

	if (reverse_value == 0) {
		gotoxy(MAP_ADJ_X + x[0], MAP_ADJ_Y + y[0], "��"); //�Ӹ��� �ִ����� �������� ��ħ 
		if (direction == LEFT) --x[0]; //���⿡ ���� ���ο� �Ӹ���ǥ(x[0],y[0])���� ���� 
		if (direction == RIGHT) ++x[0];
		if (direction == UP) --y[0];
		if (direction == DOWN) ++y[0];
		gotoxy(MAP_ADJ_X + x[i], MAP_ADJ_Y + y[i], "��"); //���ο� �Ӹ���ǥ���� �Ӹ��� �׸� 
	}
	else if (reverse_value == 1) {
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		gotoxy(MAP_ADJ_X + x[0], MAP_ADJ_Y + y[0], "��"); //�Ӹ��� �ִ����� �������� ��ħ 
		if (direction == LEFT) --x[0]; //���⿡ ���� ���ο� �Ӹ���ǥ(x[0],y[0])���� ���� 
		if (direction == RIGHT) ++x[0];
		if (direction == UP) --y[0];
		if (direction == DOWN) ++y[0];
		gotoxy(MAP_ADJ_X + x[i], MAP_ADJ_Y + y[i], "��"); //���ο� �Ӹ���ǥ���� �Ӹ��� �׸� 
														 //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
}


//pŰ�� ������ ��� ������ �Ͻ� ����
void pause(void) {
	int pause_value;
	while (1) {
		if (key == PAUSE) {
			gotoxy(MAP_ADJ_X + (MAP_X / 2) - 9, MAP_ADJ_Y,
				" < PAUSE : PRESS ANY KEY TO RESUME >");
			pause_value = 1;

			if (_kbhit()) {
				do {
					key = _getch();
					pause_value = 0;
					break;
				} while (key == 224);
			}
		}

		if (pause_value == 0) {
			key = _getch();
			draw_map();
			return;
		}
	}
}


//�������� �Լ�
void game_over(void) {
	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 6, MAP_ADJ_Y + (MAP_Y / 2) - 5,
		"+-----------------------+");
	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 6, MAP_ADJ_Y + (MAP_Y / 2) - 4,
		"|      GAME OVER...     |");
	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 6, MAP_ADJ_Y + (MAP_Y / 2) - 3,
		"+-----------------------+");
	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 6, MAP_ADJ_Y + (MAP_Y / 2) - 2,
		" YOUR SCORE : ");
	printf("%d", last_score = score);

	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) + 2,
		" Press any key to restart...");

	if (score>best_score) {
		best_score = score;
		gotoxy(MAP_ADJ_X + (MAP_X / 2) - 4, MAP_ADJ_Y + (MAP_Y / 2), "�� BEST SCORE ��");
	}
	Sleep(500);
	speed = speed + (food_count * accleration);
	speed = speed + (speed_up_item_count * accleration * (-3));
	food_count = 0;
	speed_up_item_count = 0;

	while (_kbhit()) _getch();
	key = _getch();
	title();
	game_over_count = 1;
}


//���� ����		��
void food(void) {
	int i;
	int food_crush_on = 0;   //food�� �� ������ǥ�� ���� ��� on 
	int r = 0;            //���� ������ �絿�Ǵ� ���� 

	while (1) {
		food_crush_on = 0;
		srand((unsigned)time(NULL) + r);      //����ǥ���� 
		food_x = (rand() % (MAP_X - 2)) + 1;    //������ ��ǥ���� ���� 
		food_y = (rand() % (MAP_Y - 2)) + 1;

		for (i = 0; i<length; i++) {
			//food�� �� ����� ��ġ���� Ȯ��
			if (food_x == x[i] && food_y == y[i]) {
				food_crush_on = 1;   //��ġ�� food_crush_on �� on 
				r++;
				break;
			}
		}

		//food_item�� ������ ��ġ���� Ȯ��
		if (food_x >= trap_x && food_x <= trap_x + 4 &&
			food_y >= trap_y && food_y <= trap_y + 3) {
			food_crush_on = 1;   //��ġ�� speed_up_crush_on �� on 
			r++;
		}

		if (food_crush_on == 1) {
			continue;   //������ ��� while���� �ٽ� ����
		}

		//�Ȱ����� ��� ��ǥ���� food�� ���
		gotoxy(MAP_ADJ_X + food_x, MAP_ADJ_Y + food_y, "��");
		break;
	}
}


//������ ������	��
void reverse_item(void) {
	int i;
	int reverse_item_crush_on = 0;   //reverse_item�� �� ������ǥ�� ���� ��� on 
	int r = 0;                  //���� ������ �絿�Ǵ� ���� 

	while (1) {
		reverse_item_crush_on = 0;
		srand((unsigned)time(NULL) + r);      //����ǥ���� 
		reverse_item_x = ((rand() / 2) % (MAP_X - 2)) + 1;	//������ ��ǥ���� ���� 
		reverse_item_y = ((rand() / 2) % (MAP_Y - 2)) + 1;

		for (i = 0; i<length; i++) {
			//reverse_item�� �� ����� ��ġ���� Ȯ��
			if (reverse_item_x == x[i] && reverse_item_y == y[i]) {
				reverse_item_crush_on = 1;   //��ġ�� reverse_crush_on �� on 
				r++;
				break;
			}
		}

		//reverse_item�� ������ ��ġ���� Ȯ��
		if (reverse_item_x >= trap_x && reverse_item_x <= trap_x + 4 &&
			reverse_item_y >= trap_y && reverse_item_y <= trap_y + 3) {
			reverse_item_crush_on = 1;   //��ġ�� speed_up_crush_on �� on 
			r++;
		}

		if (reverse_item_crush_on == 1) {
			continue;   //������ ��� while���� �ٽ� ����
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		gotoxy(MAP_ADJ_X + reverse_item_x, MAP_ADJ_Y + reverse_item_y, "��");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		break;
	}
}


//���̴��� ������	��
void length_item(void) {
	int i;

	int length_item_crush_on = 0;   //speed_up_item�� �� ������ǥ�� ���� ��� on 
	int r = 0;            //���� ������ �絿�Ǵ� ���� 

	while (1) {
		length_item_crush_on = 0;
		srand((unsigned)time(NULL) + r);      //����ǥ���� 
		length_item_x = ((rand() / 3) % (MAP_X - 2)) + 1;	//������ ��ǥ���� ���� 
		length_item_y = ((rand() / 3) % (MAP_Y - 2)) + 1;

		for (i = 0; i < length; i++) {
			//length_item�� �� ����� ��ġ���� Ȯ��
			if (length_item_x == x[i] && length_item_y == y[i]) {
				length_item_crush_on = 1;   //��ġ�� speed_up_crush_on �� on 
				r++;
				break;
			}
		}

		//length_item�� ������ ��ġ���� Ȯ��
		if (length_item_x >= trap_x && length_item_x <= trap_x + 4 &&
			length_item_y >= trap_y && length_item_y <= trap_y + 3) {
			length_item_crush_on = 1;   //��ġ�� speed_up_crush_on �� on 
			r++;
		}

		if (length_item_crush_on == 1) {
			continue;   //������ ��� while���� �ٽ� ����
		}

		gotoxy(MAP_ADJ_X + length_item_x, MAP_ADJ_Y + length_item_y, "��");
		break;
	}
}


//�ӵ����� ������	S
void speed_up_item(void) {
	int i;

	int speed_up_item_crush_on = 0;   //speed_up_item�� �� ������ǥ�� ���� ��� on 
	int r = 0;            //���� ������ �絿�Ǵ� ���� 

	while (1) {
		speed_up_item_crush_on = 0;
		srand((unsigned)time(NULL) + r);      //����ǥ���� 
		speed_up_item_x = ((rand() / 4) % (MAP_X - 2)) + 1;    //������ ��ǥ���� ���� 
		speed_up_item_y = ((rand() / 4) % (MAP_Y - 2)) + 1;

		for (i = 0; i < length; i++) {
			//speed_up_item�� �� ����� ��ġ���� Ȯ��
			if (speed_up_item_x == x[i] && speed_up_item_y == y[i]) {
				speed_up_item_crush_on = 1;   //��ġ�� speed_up_crush_on �� on 
				r++;
				break;
			}
		}

		//speed_up_item�� ������ ��ġ���� Ȯ��
		if (speed_up_item_x >= trap_x && speed_up_item_x <= trap_x + 3 &&
			speed_up_item_y >= trap_y && speed_up_item_y <= trap_y + 2) {
			speed_up_item_crush_on = 1;   //��ġ�� speed_up_crush_on �� on 
			r++;
		}

		if (speed_up_item_crush_on == 1) {
			continue;   //������ ��� while���� �ٽ� ����
		}

		gotoxy(MAP_ADJ_X + speed_up_item_x, MAP_ADJ_Y + speed_up_item_y, "S");
		break;
	}
}


//���� ������
void trap_item(void) {
	int i;

	int trap_crush_on = 0;   //trap_item�� �� ������ǥ�� ���� ��� on 
	int r = 0;            //���� ������ �絿�Ǵ� ���� 

	while (1) {
		trap_crush_on = 0;
		srand((unsigned)time(NULL) + r);      //����ǥ���� 
		trap_x = ((rand() + rand()) % (MAP_X - 8)) + 1;    //������ ��ǥ���� ���� 
		trap_y = ((rand() + rand()) % (MAP_Y - 6)) + 1;

		//trap_item�� �� ����� ��ġ���� Ȯ��
		for (i = 0; i < length; i++) {
			if (x[i] >= trap_x && x[i] <= trap_x + 3 &&
				y[i] >= trap_y && y[i] <= trap_y + 2) {
				trap_crush_on = 1;   //��ġ�� trap_crush_on �� on 
				r++;
				break;
			}
		}

		if (trap_crush_on == 1) {
			continue;   //������ ��� while���� �ٽ� ����
		}

		gotoxy(MAP_ADJ_X + trap_x, MAP_ADJ_Y + trap_y + 0, " �ȤѪ� ");
		gotoxy(MAP_ADJ_X + trap_x, MAP_ADJ_Y + trap_y + 1, "�Ӣ�����");
		gotoxy(MAP_ADJ_X + trap_x, MAP_ADJ_Y + trap_y + 2, "�ѤѤѤ�");
		break;
	}
}


//�ɼ�
void option(void) {
	int i, j;
	int direction_stack_vertical = 0;
	int direction_stack_x = 0;
	int direction_stack_y = 0;
	int direction_stack_speed = 0;
	int direction_stack_accleration = 0;

	for (i = MAP_ADJ_X + 1; i<MAP_ADJ_X + MAP_X; i++) {
		for (j = MAP_ADJ_Y + 1; j<MAP_ADJ_Y + MAP_Y - 1; j++) gotoxy(i, j, "  ");
	}
	gotoxy(MAP_ADJ_X + MAP_X / 2 - 10, MAP_ADJ_Y,
		" < PRESS B KEY TO GO BACK TO THE TITLE > ");
	gotoxy(MAP_ADJ_X + MAP_X / 2 - 7, MAP_ADJ_Y + 3,
		"+--------------------------+");
	gotoxy(MAP_ADJ_X + MAP_X / 2 - 7, MAP_ADJ_Y + 4,
		"|        O P T I O N       |");
	gotoxy(MAP_ADJ_X + MAP_X / 2 - 7, MAP_ADJ_Y + 5,
		"+--------------------------+");

	movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 8);
	printf("SIZE OF MAP_X:   <   %3d  >", MAP_X);
	movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 11);
	printf("SIZE OF MAP_Y:   <   %3d  >", MAP_Y);
	movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 14);
	printf("INITIAL SPEED:   <   %3d  >", speed);
	movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 17);
	printf("ACCLERATION:     <   %3d  >", accleration);
	movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 20);
	printf("DIFFICULTY:      <  EASY  >");

	while (1) {
		if (_kbhit()) {
			do { key = _getch(); } while (key == 224);
		}

		if (direction_stack_vertical == 0) {
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 8, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 11, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 14, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 17, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 20, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 8, "��");
			Sleep(50);

			switch (key) {
			case LEFT:
				if (MAP_X <= 30) {
					key = 0;
					MAP_X = 30;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 8);
					printf("SIZE OF MAP_X:   <   %3d  >", MAP_X);
				}
				else if (30 < MAP_X) {
					key = 0;
					MAP_X -= 10;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 8);
					printf("SIZE OF MAP_X:   <   %3d  >", MAP_X);
				}
				break;

			case RIGHT:
				if (MAP_X < 200) {
					key = 0;
					MAP_X += 10;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 8);
					printf("SIZE OF MAP_X:   <   %3d  >", MAP_X);
				}
				else if (MAP_X >= 200) {
					key = 0;
					MAP_X = 200;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 8);
					printf("SIZE OF MAP_X:   <   %3d  >", MAP_X);
				}
				break;

			case DOWN:
				direction_stack_vertical = 1;
				key = 0;
				break;

			default:
				break;
			}
		}

		else if (direction_stack_vertical == 1) {
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 8, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 11, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 14, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 17, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 20, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 11, "��");
			Sleep(50);

			switch (key) {
			case LEFT:
				if (MAP_Y <= 30) {
					key = 0;
					MAP_Y = 30;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 11);
					printf("SIZE OF MAP_Y:   <   %3d  >", MAP_Y);
				}
				else if (30 < MAP_Y) {
					key = 0;
					MAP_Y -= 10;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 11);
					printf("SIZE OF MAP_Y:   <   %3d  >", MAP_Y);
				}
				break;


			case RIGHT:
				if (MAP_Y < 200) {
					key = 0;
					MAP_Y += 10;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 11);
					printf("SIZE OF MAP_Y:   <   %3d  >", MAP_Y);
				}
				else if (MAP_Y >= 200) {
					key = 0;
					MAP_Y = 200;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 11);
					printf("SIZE OF MAP_Y:   <   %3d  >", MAP_Y);
				}
				break;

			case UP:
				direction_stack_vertical = 0;
				key = 0;
				break;

			case DOWN:
				direction_stack_vertical = 2;
				key = 0;
				break;

			default:
				break;
			}
		}

		else if (direction_stack_vertical == 2) {
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 8, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 11, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 14, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 17, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 20, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 14, "��");
			Sleep(50);

			switch (key) {
			case LEFT:
				if (speed <= 30) {
					key = 0;
					speed = 30;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 14);
					printf("INITIAL SPEED:   <   %3d  >", speed);
				}
				else if (30 < speed) {
					key = 0;
					speed -= 10;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 14);
					printf("INITIAL SPEED:   <   %3d  >", speed);
				}
				break;

			case RIGHT:
				if (speed < 150) {
					key = 0;
					speed += 10;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 14);
					printf("INITIAL SPEED:   <   %3d  >", speed);
				}
				else if (speed >= 150) {
					key = 0;
					speed = 150;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 14);
					printf("INITIAL SPEED:   <   %3d  >", speed);
				}
				break;

			case UP:
				direction_stack_vertical = 1;
				key = 0;
				break;

			case DOWN:
				direction_stack_vertical = 3;
				key = 0;
				break;

			default:
				break;
			}
		}

		else if (direction_stack_vertical == 3) {
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 8, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 11, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 14, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 17, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 20, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 17, "��");
			Sleep(50);

			switch (key) {
			case LEFT:
				if (accleration <= 1) {
					key = 0;
					accleration = 1;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 17);
					printf("ACCLERATION:     <   %3d  >", accleration);
				}
				else if (1 < accleration) {
					key = 0;
					accleration -= 1;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 17);
					printf("ACCLERATION:     <   %3d  >", accleration);
				}
				break;

			case RIGHT:
				if (accleration < 5) {
					key = 0;
					accleration += 1;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 17);
					printf("ACCLERATION:     <   %3d  >", accleration);
				}
				else if (accleration >= 5) {
					key = 0;
					accleration = 5;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 17);
					printf("ACCLERATION:     <   %3d  >", accleration);
				}
				break;

			case UP:
				direction_stack_vertical = 2;
				key = 0;
				break;

			case DOWN:
				direction_stack_vertical = 4;
				key = 0;
				break;

			default:
				break;
			}
		}

		else if (direction_stack_vertical == 4) {
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 8, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 11, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 14, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 17, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 20, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 20, "��");
			Sleep(50);

			switch (key) {
			case LEFT:
				if (difficulty == 0) {
					key = 0;
					difficulty = 0;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 20);
					printf("                           ");
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 20);
					printf("DIFFICULTY:      <  EASY  >");
				}
				else if (difficulty == 1) {
					key = 0;
					difficulty = 0;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 20);
					printf("                           ");
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 20);
					printf("DIFFICULTY:      <  EASY  >");
				}
				else if (difficulty == 2) {
					key = 0;
					difficulty = 1;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 20);
					printf("                           ");
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 20);
					printf("DIFFICULTY:      < NORMAL >");
				}
				break;

			case RIGHT:
				if (difficulty == 0) {
					key = 0;
					difficulty = 1;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 20);
					printf("                           ");
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 20);
					printf("DIFFICULTY:      < NORMAL >");
				}
				else if (difficulty == 1) {
					key = 0;
					difficulty = 2;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 20);
					printf("                           ");
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 20);
					printf("DIFFICULTY:      <  HARD  >");
				}
				else if (difficulty == 2) {
					key = 0;
					difficulty = 2;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 20);
					printf("                           ");
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 20);
					printf("DIFFICULTY:      <  HARD  >");
				}
				break;

			case UP:
				direction_stack_vertical = 3;
				key = 0;
				break;

			case DOWN:
				direction_stack_vertical = 4;
				key = 0;
				break;

			default:
				break;
			}
		}

		if (_kbhit()) {
			key = _getch();
			if (key == BACK) {
				break;
			}
			if (key == ESC) {
				gotoxy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y + 1, " ");
				exit(0);
			}
			else continue;
		}
	}
	title();
}


//�������ͽ�
void status(void) {
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 0, "head= ");
	printf("%2d,%2d", x[0], y[0]);
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 1, "food= ");
	printf("%2d,%2d", food_x, food_y);
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 2, "length= ");
	printf("%2d", length);
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 3, "key= ");
	printf("%3d", key);
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 4, "speed= ");
	printf("%3d", speed);
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 5, "acc= ");
	printf("%3d", accleration);
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 6, "switch= ");
	printf("%3d", food_item_regeneration_switch);
}


//�������ͽ� ����
void status_off(void) {
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 0, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 1, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 2, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 3, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 4, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 5, "                  ");
}


//Ŀ�� �Ⱥ��̰� ���ִ� �Լ�
void setCursorType(CURSOR_TYPE c)
{
	CONSOLE_CURSOR_INFO CurInfo;
	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}


//����
void help(void) {
	int i, j;

	for (i = MAP_ADJ_X + 1; i<MAP_ADJ_X + MAP_X; i++) {
		for (j = MAP_ADJ_Y + 1; j<MAP_ADJ_Y + MAP_Y - 1; j++) gotoxy(i, j, "  ");
	}
	gotoxy(MAP_ADJ_X + MAP_X / 2 - 10, MAP_ADJ_Y,
		" < PRESS B KEY TO GO BACK TO THE TITLE > ");
	gotoxy(MAP_ADJ_X + MAP_X / 2 - 7, MAP_ADJ_Y + 3,
		"+--------------------------+");
	gotoxy(MAP_ADJ_X + MAP_X / 2 - 7, MAP_ADJ_Y + 4,
		"|        H  E  L  P        |");
	gotoxy(MAP_ADJ_X + MAP_X / 2 - 7, MAP_ADJ_Y + 5,
		"+--------------------------+");
	gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 8,
		"�� : ������ �÷��ִ� ������");
	gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 9,
		"	   ȹ���� ������ �������� ���̰� �þ��");
	gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 10,
		"�� : �������� �ӵ��� ������ ����� ������");
	gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 11,
		"�� : �������� ���̸� �ٿ��ִ� ������");
	gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 12,
		"�� : Ű�����Է��� ���� �ð����� ������Ų��");
	gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 14,
		" �ȤѪ�    ");
	gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 15,
		"�Ӣ�����   �÷��� ���� ��Ÿ���� ����");
	gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 16,
		"�ѤѤѤ�   �����̿� ���� ��� ������ ������");
	gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 17,
		" �δ��� ");

	while (1) {
		if (_kbhit()) {
			key = _getch();
			if (key == BACK) {
				break;
			}
			if (key == ESC) {
				gotoxy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y + 1, " ");
				exit(0);
			}
			else continue;
		}
	}
	title();
}