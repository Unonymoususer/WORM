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
#define PAUSE 112
#define STATUS 115
#define ESC 27
#define MAP_ADJ_X 3   //���� X�� ������
#define MAP_ADJ_Y 2   //���� Y�� ������

static int MAP_X = 30;      //���� X�� ũ��
static int MAP_Y = 20;		//���� Y�� ũ��
int speed = 100;
int accleration = 3;
int accleration;
int x[100], y[100];	//x,y ��ǥ���� ���� �� 100�� 
int food_x, food_y;	//food�� ��ǥ���� ���� 
int reverse_item_x, reverse_item_y;
int speed_up_item_x, speed_up_item_y;
int length;         //�����̸� ���
int score;			//���� ����, reset�Լ��� ���� �ʱ�ȭ��
int best_score = 0; //�ְ� ���� ����, reset�Լ��� ���� �ʱ�ȭ ���� ���� 
int last_score = 0; //������ ���� ����, reset�Լ��� ���� �ʱ�ȭ ���� ����
int direction;      //�̵����� ���� 
int key;			//�Է¹��� Ű ���� 
int trap_x, trap_y;
int length_item_x, length_item_y;
int status_on = 0;	//�����ڿ� status ǥ��Ȱ��ȭ ����. �����߿� SŰ�� ������ Ȱ��ȭ
int food_count = -1;
int length_item_count = -1;
int speed_up_item_count = -1;
int reverse_item_count = -1;
int reverse_value = 0;
int interval = 5000;

char *reverse_item_waring[] = { "YOU JUST ACTIVATED TRAP ITEM. \
YOUR KEYBOARD INPUT IS REVERSED FOR 5 SECONDS." };


void gotoxy(int x, int y, char *s);	//Ŀ���̵� �� �Է�
void movexy(int x, int y);			//Ŀ���̵�
void title(void);      //���� ����ȭ�� 
void reset(void);      //������ �ʱ�ȭ 
void draw_map(void);   //������ �׵θ��� �׸� 
void move(int dir);      //��Ӹ��� �̵� 
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


						//�����Լ�
int main() {
	system("TITLE WORM GAME");
	title(); //���� ����ȭ�� ���

	while (1) {
		if (_kbhit()) do { key = _getch(); } while (key == 224); //Ű �Է¹���
		Sleep(speed);

		//�Է¹��� Ű�� �ľ��ϰ� ����
		switch (key) {
		case LEFT:
		case RIGHT:
		case UP:
		case DOWN:
			if ((direction == LEFT && key != RIGHT) || (direction == RIGHT && key != LEFT) ||
				(direction == UP && key != DOWN) || (direction == DOWN && key != UP))
				//180ȸ���̵��� �����ϱ� ���� �ʿ�. 
				direction = key;
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

		if (status_on == 1) status();      //status ǥ��
		if (status_on == 0) status_off();   //status ����
	}
}


//x���� 2x�� ����, ��ǥ���� �ٷ� ���ڿ��� �Է��� �� �ֵ��� printf�Լ� ����
void gotoxy(int x, int y, char *s) {
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", s);
}


//Ŀ���̵��Լ�. gotoxy���� prinff��� ����
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
		"   �� ESC : Quit");

	while (1) {
		//Ű�Է¹���
		if (_kbhit()) {
			key = _getch();
			if (key == OPTION) option();
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
	speed += (-2) * accleration;   //�ӵ� �ʱ�ȭ
	length = 5;            //�� ���� �ʱ�ȭ
	score = 0;            //���� �ʱ�ȭ

						  //�� ���밪 �Է� 
	for (i = 0; i<length; i++) {
		x[i] = MAP_X / 2 + i;
		y[i] = MAP_Y / 2;
		gotoxy(MAP_ADJ_X + x[i], MAP_ADJ_Y + y[i], "��");
	}
	gotoxy(MAP_ADJ_X + x[0], MAP_ADJ_Y + y[0], "��");   //�� �Ӹ� �׸�

	trap_item();		//�δ���
	food();				//��ǥ	��
	speed_up_item();	//S		S
	length_item();		//Ŭ�ι�	��
	reverse_item();		//�籸��	��
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

		x[length - 1] = x[length - 2];   //���θ��� ���뿡 �� �Է�
		y[length - 1] = y[length - 2];

		food();
	}

	//speed_up_item�� �浹���� ���
	else if (x[0] == speed_up_item_x && y[0] == speed_up_item_y) {
		speed_up_item();
	}

	//reverse_item�� �浹���� ���
	else if (x[0] == reverse_item_x && y[0] == reverse_item_y) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		gotoxy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y + 2,
			"���� �����ۿ� �ɷȽ��ϴ�. 5�ʵ��� Ű������ ������ �����˴ϴ�.");

		//movexy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y + 2);
		//printf("%s",reverse_item_waring);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		reverse_value = 1;
		reverse_item();
	}

	//length_item�� �浹���� ���
	else if (x[0] == length_item_x && y[0] == length_item_y) {
		if (length > 3) {
			length--;
			gotoxy(MAP_ADJ_X + x[length - 1], MAP_ADJ_Y + y[length - 1], "  ");
			gotoxy(MAP_ADJ_X + x[length - 2], MAP_ADJ_Y + y[length - 2], "  ");
		}
		else {
			gotoxy(MAP_ADJ_X + x[length - 1], MAP_ADJ_Y + y[length - 1], "  ");
		}
		length_item();
	}

	//trap�� �浹���� ���
	else if (x[0] >= trap_x && x[0] <= trap_x + 3 && y[0] >= trap_y
		&& y[0] <= trap_y + 2) {
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
	food_count = -1;
	speed = speed + (speed_up_item_count * accleration * (-3));
	speed_up_item_count = -1;

	while (_kbhit()) _getch();
	key = _getch();
	title();
}


//���� ����		��
void food(void) {
	int i;
	int food_crush_on = 0;   //food�� �� ������ǥ�� ���� ��� on 
	int r = 0;            //���� ������ �絿�Ǵ� ���� 
	gotoxy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y, " YOUR SCORE: ");   //����ǥ�� 
	printf("%5d     LAST SCORE: %5d     BEST SCORE: %5d",
		score, last_score, best_score);

	while (1) {
		food_crush_on = 0;
		srand((unsigned)time(NULL) + r);      //����ǥ���� 
		food_x = (rand() % (MAP_X - 2)) + 1;    //������ ��ǥ���� ���� 
		food_y = (rand() % (MAP_Y - 2)) + 1;

		//food�� �� ����� ��ġ���� Ȯ��
		for (i = 0; i<length; i++) {
			if (food_x == x[i] && food_y == y[i]) {
				food_crush_on = 1;   //��ġ�� food_crush_on �� on 
				r++;
				break;
			}

			//food_item�� ������ ��ġ���� Ȯ��
			if (x[i] >= trap_x && x[i] <= trap_x + 4 && y[i] >= trap_y
				&& y[i] <= trap_y + 3) {
				food_crush_on = 1;   //��ġ�� speed_up_crush_on �� on 
				r++;
				break;
			}
		}

		if (food_crush_on == 1) {
			continue;   //������ ��� while���� �ٽ� ����
		}

		//�Ȱ����� ��� ��ǥ���� food�� ���
		food_count++;
		gotoxy(MAP_ADJ_X + food_x, MAP_ADJ_Y + food_y, "��");
		speed -= accleration;   //�ӵ� ���� 
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
		reverse_item_x = ((rand() / 2) % (MAP_X - 2)) + 1;    //������ ��ǥ���� ���� 
		reverse_item_y = ((rand() / 2) % (MAP_Y - 2)) + 1;

		for (i = 0; i<length; i++) {
			//reverse_item�� �� ����� ��ġ���� Ȯ��
			if (reverse_item_x == x[i] && reverse_item_y == y[i]) {
				reverse_item_crush_on = 1;   //��ġ�� reverse_crush_on �� on 
				r++;
				break;
			}

			//reverse_item�� ������ ��ġ���� Ȯ��
			if (x[i] >= trap_x && x[i] <= trap_x + 4 && y[i] >= trap_y
				&& y[i] <= trap_y + 3) {
				reverse_item_crush_on = 1;   //��ġ�� speed_up_crush_on �� on 
				r++;
				break;
			}
		}

		if (reverse_item_crush_on == 1) {
			continue;   //������ ��� while���� �ٽ� ����
		}

		reverse_item_count++;
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

			//length_item�� ������ ��ġ���� Ȯ��
			if (x[i] >= trap_x && x[i] <= trap_x + 4 && y[i] >= trap_y
				&& y[i] <= trap_y + 3) {
				length_item_crush_on = 1;   //��ġ�� speed_up_crush_on �� on 
				r++;
				break;
			}
		}

		if (length_item_crush_on == 1) {
			continue;   //������ ��� while���� �ٽ� ����
		}

		length_item_count++;
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

			//speed_up_item�� ������ ��ġ���� Ȯ��
			if (x[i] >= trap_x && x[i] <= trap_x + 3 && y[i] >= trap_y
				&& y[i] <= trap_y + 2) {
				speed_up_item_crush_on = 1;   //��ġ�� speed_up_crush_on �� on 
				r++;
				break;
			}
		}
		if (speed_up_item_crush_on == 1) {
			continue;   //������ ��� while���� �ٽ� ����
		}
		speed_up_item_count++;
		speed += accleration * 3;

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
			if (x[i] >= trap_x && x[i] <= trap_x + 4 && y[i] >= trap_y && y[i] <= trap_y + 3) {
				trap_crush_on = 1;   //��ġ�� trap_crush_on �� on 
				r++;
				break;
			}
		}

		if (trap_crush_on == 1) {
			continue;   //������ ��� while���� �ٽ� ����
		}

		gotoxy(MAP_ADJ_X + trap_x, MAP_ADJ_Y + trap_y, " �ȤѪ� ");
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
	gotoxy(MAP_ADJ_X + 5, MAP_ADJ_Y,
		" < PRESS B KEY TO GO BACK TO THE TITLE > ");
	gotoxy(MAP_ADJ_X + 8, MAP_ADJ_Y + 3,
		"+--------------------------+");
	gotoxy(MAP_ADJ_X + 8, MAP_ADJ_Y + 4,
		"|        O P T I O N       |");
	gotoxy(MAP_ADJ_X + 8, MAP_ADJ_Y + 5,
		"+--------------------------+");

	movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 8);
	printf("SIZE OF MAP_X:   < %3d >", MAP_X);
	movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 11);
	printf("SIZE OF MAP_Y:   < %3d >", MAP_Y);
	movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 14);
	printf("INITIAL SPEED:   < %3d >", speed);
	movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 17);
	printf("ACCLERATION:     < %3d >", accleration);

	while (1) {
		if (_kbhit()) {
			do { key = _getch(); } while (key == 224);
		}

		if (direction_stack_vertical == 0) {
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 8, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 11, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 14, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 17, " ");
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 8, "��");
			Sleep(50);

			switch (key) {
			case LEFT:
				if (MAP_X <= 30) {
					key = 0;
					MAP_X = 30;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 8);
					printf("SIZE OF MAP_X:   < %3d >", MAP_X);
				}
				else if (30 < MAP_X) {
					key = 0;
					MAP_X -= 10;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 8);
					printf("SIZE OF MAP_X:   < %3d >", MAP_X);
				}
				break;

			case RIGHT:
				if (MAP_X < 200) {
					key = 0;
					MAP_X += 10;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 8);
					printf("SIZE OF MAP_X:   < %3d >", MAP_X);
				}
				else if (MAP_X >= 200) {
					key = 0;
					MAP_X = 200;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 8);
					printf("SIZE OF MAP_X:   < %3d >", MAP_X);
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

			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 11, "��");
			Sleep(50);

			switch (key) {
			case LEFT:
				if (MAP_Y <= 20) {
					key = 0;
					MAP_Y = 20;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 11);
					printf("SIZE OF MAP_Y:   < %3d >", MAP_Y);
				}
				else if (20 < MAP_Y) {
					key = 0;
					MAP_Y -= 10;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 11);
					printf("SIZE OF MAP_Y:   < %3d >", MAP_Y);
				}
				break;


			case RIGHT:
				if (MAP_Y < 200) {
					key = 0;
					MAP_Y += 10;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 11);
					printf("SIZE OF MAP_Y:   < %3d >", MAP_Y);
				}
				else if (MAP_Y >= 200) {
					key = 0;
					MAP_Y = 200;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 11);
					printf("SIZE OF MAP_Y:   < %3d >", MAP_Y);
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

			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 14, "��");
			Sleep(50);

			switch (key) {
			case LEFT:
				if (speed <= 30) {
					key = 0;
					speed = 30;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 14);
					printf("INITIAL SPEED:   < %3d >", speed);
				}
				else if (30 < speed) {
					key = 0;
					speed -= 10;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 14);
					printf("INITIAL SPEED:   < %3d >", speed);
				}
				break;

			case RIGHT:
				if (speed < 150) {
					key = 0;
					speed += 10;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 14);
					printf("INITIAL SPEED:   < %3d >", speed);
				}
				else if (speed >= 150) {
					key = 0;
					speed = 150;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 14);
					printf("INITIAL SPEED:   < %3d >", speed);
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
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 17, "��");
			Sleep(50);

			switch (key) {
			case LEFT:
				if (accleration <= 1) {
					key = 0;
					accleration = 1;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 17);
					printf("ACCLERATION:      < %3d >", accleration);
				}
				else if (1 < accleration) {
					key = 0;
					accleration -= 1;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 17);
					printf("ACCLERATION:      < %3d >", accleration);
				}
				break;

			case RIGHT:
				if (accleration < 5) {
					key = 0;
					accleration += 1;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 17);
					printf("ACCLERATION:      < %3d >", accleration);
				}
				else if (accleration >= 5) {
					key = 0;
					accleration = 5;
					movexy(MAP_ADJ_X + 6, MAP_ADJ_Y + 17);
					printf("ACCLERATION:      < %3d >", accleration);
				}
				break;

			case UP:
				direction_stack_vertical = 2;
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


//���� ������ ���� �ִ� �Լ�
void status(void) {
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y, "head= ");
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
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 6, "map_x= ");
	printf("%3d", MAP_X);
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 7, "map_y= ");
	printf("%3d", MAP_Y);
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 8, "food_count= ");
	printf("%3d", food_count);
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 9, "reverse_item_x= ");
	printf("%3d", reverse_item_x);
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 10, "reverse_item_y= ");
	printf("%3d", reverse_item_y);
}


//�������ͽ� ����
void status_off(void) {
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 1, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 2, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 3, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 4, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 5, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 6, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 7, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 8, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 9, "                   ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 10, "                   ");
	movexy(MAP_ADJ_X + 1, MAP_ADJ_Y + MAP_Y + 2);
}