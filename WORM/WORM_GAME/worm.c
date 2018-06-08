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
#define MAP_ADJ_X 3   //맵의 X축 시작점
#define MAP_ADJ_Y 2   //맵의 Y축 시작점

static int MAP_X = 30;      //맵의 X축 크기
static int MAP_Y = 20;		//맵의 Y축 크기
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
int x[100], y[100];	//x,y 좌표값을 저장 총 100개 

int food_x, food_y;	//food의 좌표값을 저장 
int reverse_item_x, reverse_item_y;
int speed_up_item_x, speed_up_item_y;
int length_item_x, length_item_y;
int trap_x, trap_y;

int food_item_regeneration = 12;		//음식재생성 12초
int reverse_item_regeneration = 14;		//리버스재생생 14초
int length_item_regeneration = 16;		//길이재생생 16초
int speed_item_regeneration = 18;		//스피드재생성 18초
int trap_item_regeneration = 20;		//함정재생성 20초
int reverse_item_effect_duration = 5;

int food_count = 0;
int length_item_count = 0;
int speed_up_item_count = 0;
int reverse_item_count = 0;
int trap_count = 0;

int length;         //몸길이를 기억
int score;			//점수 저장, reset함수에 의해 초기화됨
int best_score = 0; //최고 점수 저장, reset함수에 의해 초기화 되지 않음 
int last_score = 0; //마지막 점수 저장, reset함수에 의해 초기화 되지 않음
int direction;      //이동방향 저장 
int key;			//입력받은 키 저장 
int status_on = 0;	//개발자용 status 표시활성화 변수. 게임중에 S키를 누르면 활성화


void gotoxy(int x, int y, char *s);	//커서이동 및 입력
void movexy(int x, int y);			//커서이동
void title(void);      //게임 시작화면 
void reset(void);      //게임을 초기화 
void draw_map(void);   //게임판 테두리를 그림 
void move(int direction);      //뱀머리를 이동 
void pause(void);      //일시정지 
void game_over(void);   //게임 오버를 확인 
void food(void);      //음식 생성 
void reverse_item(void);
void trap_item(void);
void length_item(void);
void speed_up_item(void);
void status(void);      //개발자용 status 표시
void status_off(void);   //개발자용 status 끄기
void option(void);      //option
void help(void);

						//메인함수
int main() {
	system("TITLE WORM GAME");
	title(); //게임 시작화면 출력

	trap_item();		//두더지
	food();				//음표	♪
	speed_up_item();	//S		S
	length_item();		//클로버	♣
	reverse_item();		//당구장	※
	food_item_time_start = clock();
	reverse_item_time_start = clock();
	length_item_time_start = clock();
	speed_item_time_start = clock();
	trap_item_time_start = clock();

	while (1) {
		gotoxy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y, " YOUR SCORE: ");   //점수표시 
		printf("%5d     LAST SCORE: %5d     BEST SCORE: %5d",
			score, last_score, best_score);

		if (_kbhit()) {
			do { 
				key = _getch();
			} while (key == 224); //키 입력받음
		}
		Sleep(speed);

		//입력받은 키를 파악하고 실행
		switch (key) {
		case LEFT:
		case RIGHT:
		case UP:
		case DOWN:
			if ((direction == LEFT && key != RIGHT) ||
				(direction == RIGHT && key != LEFT) ||
				(direction == UP && key != DOWN) ||
				(direction == DOWN && key != UP)) {
				direction = key;	//180회전이동을 방지하기 위해 필요.
				if (reverse_value == 1) {
					switch (key) {
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
					}
				}
			}
			key = 0;   //키값을 저장하는 함수를 reset 
			break;
		case PAUSE:      //P키를 누르면 일시정지 
			pause();
			key = 0;
			break;
		case STATUS:   //S키를 누르면 개발자용 status를 활성화 시킴  
			if (status_on == 0) status_on = 1;
			else status_on = 0;
			key = 0;
			break;
		case ESC:      //ESC키를 누르면 프로그램 종료 
			gotoxy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y + 1, " ");
			exit(0);
		}
		move(direction);

		if (status_on == 1) {
			status();      //status 표시
		}
		else if (status_on == 0) {
			status_off();   //status 끄기
		}

		food_item_time_end = clock();
		reverse_item_time_end = clock();
		length_item_time_end = clock();
		speed_item_time_end = clock();
		trap_item_time_end = clock();
		reverse_item_effect_end = clock();


		if ((food_item_time_end - food_item_time_start)/CLOCKS_PER_SEC > 
			food_item_regeneration) {
			food();
			food_item_time_start = clock();
		}
		if ((reverse_item_time_end - reverse_item_time_start) / CLOCKS_PER_SEC >
			reverse_item_regeneration) {
			reverse_item();
			reverse_item_time_start = clock();
		}
		if ((length_item_time_end - length_item_time_start) / CLOCKS_PER_SEC >
			length_item_regeneration) {
			length_item();
			length_item_time_start = clock();
		}
		if ((speed_item_time_end - speed_item_time_start) / CLOCKS_PER_SEC >
			speed_item_regeneration) {
			speed_up_item();
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


//x값을 2x로 변경, 좌표값에 바로 문자열을 입력할 수 있도록 printf함수 삽입
void gotoxy(int x, int y, char *s) {
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", s);
}


//커서이동함수. gotoxy에서 printf기능 삭제
void movexy(int x, int y) {
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


//게임 시작화면 출력
void title(void) {
	system("cls");   //화면을 지움
	int i, j;
	while (_kbhit()) _getch();   //버퍼에 있는 키값을 버림
	draw_map();               //맵 테두리를 그림

							  //맵 테두리 안쪽을 빈칸으로 채움
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
		"   ◇ ←,→,↑,↓ : Move");
	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) + 2,
		"   ◇ P : Pause");
	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) + 3,
		"   ◇ O : Option");
	gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) + 4,
		"   ◇ ESC : Quit");

	while (1) {
		//키입력받음
		if (_kbhit()) {
			key = _getch();
			if (key == OPTION) option();
			if (key == ESC) exit(0);
			else break;   //아니면 그냥 계속 진행
		}
		gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) - 1,
			" < PRESS ANY KEY TO START > ");
		Sleep(400);
		gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + (MAP_Y / 2) - 1,
			"                            ");
		Sleep(400);
		gotoxy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y + 1, " ");
	}
	reset();   //게임을 초기화  
}


// 리셋
void reset(void) {
	int i;
	system("cls");   //화면을 지움
	draw_map();      //맵 테두리를 그림
	while (_kbhit()) _getch();   //버퍼에 있는 키값을 버림

	direction = LEFT;      //방향 초기화, 초기방향 왼쪽
	length = 5;            //뱀 길이 초기화
	score = 0;            //점수 초기화

	//뱀 몸통값 입력 
	for (i = 0; i<length; i++) {
		x[i] = MAP_X / 2 + i;
		y[i] = MAP_Y / 2;
		gotoxy(MAP_ADJ_X + x[i], MAP_ADJ_Y + y[i], "ㅇ");
	}
	gotoxy(MAP_ADJ_X + x[0], MAP_ADJ_Y + y[0], "★");   //뱀 머리 그림

	trap_item();			//두더지
}


//맵 테두리 그리는 함수
void draw_map(void) {
	int i;
	for (i = 0; i<MAP_X; i++) {
		gotoxy(MAP_ADJ_X + i, MAP_ADJ_Y, "■");
		gotoxy(MAP_ADJ_X + i, MAP_ADJ_Y + MAP_Y - 1, "■");
	}
	for (i = 0; i<MAP_Y; i++) {
		gotoxy(MAP_ADJ_X, MAP_ADJ_Y + i, "■");
		gotoxy(MAP_ADJ_X + MAP_X, MAP_ADJ_Y + i, "■");
	}
}


//뱀머리를 이동
void move(int direction) {
	int i;

	//food와 충돌했을 경우
	if (x[0] == food_x && y[0] == food_y) {
		score += 10;   //점수 증가 
		length++;      //길이증가
		food_count++;
		speed -= accleration;

		x[length - 1] = x[length - 2];   //새로만든 몸통에 값 입력
		y[length - 1] = y[length - 2];
	}

	//speed_up_item과 충돌했을 경우
	else if (x[0] == speed_up_item_x && y[0] == speed_up_item_y) {
		speed_up_item_count++;
		speed += accleration * 3;
	}

	//reverse_item과 충돌했을 경우
	else if (x[0] == reverse_item_x && y[0] == reverse_item_y) {
		reverse_item_count++;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		gotoxy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y + 2,
			"함정 아이템에 걸렸습니다. 일정 시간동안 키보드의 방향이 반전됩니다.");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		reverse_value = 1;
		reverse_item_effect_start = clock();
	}

	//length_item과 충돌했을 경우
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
	}

	//trap과 충돌했을 경우
	else if (x[0] >= trap_x && x[0] <= trap_x + 3 && 
		y[0] >= trap_y && y[0] <= trap_y + 2) {
		game_over();
		return;
	}

	//벽과 충돌했을 경우
	if (x[0] == 0 || x[0] == MAP_X - 1 || y[0] == 0 || y[0] == MAP_Y - 1) {
		game_over();
		return; //game_over에서 게임을 다시 시작하게 되면 여기서부터 반복되므로 
				//return을 사용하여 move의 나머지 부분이 실행되지 않도록 합니다. 
	}

	//자기몸과 충돌했는지 검사
	for (i = 1; i<length; i++) {
		if (x[0] == x[i] && y[0] == y[i]) {
			game_over();
			return;
		}
	}

	//몸통 마지막을 지움
	gotoxy(MAP_ADJ_X + x[length - 1], MAP_ADJ_Y + y[length - 1], "  ");

	//몸통좌표를 한칸씩 옮김
	for (i = length - 1; i>0; i--) {
		x[i] = x[i - 1];
		y[i] = y[i - 1];
	}

	if (reverse_value == 0) {
		gotoxy(MAP_ADJ_X + x[0], MAP_ADJ_Y + y[0], "ㅇ"); //머리가 있던곳을 몸통으로 고침 
		if (direction == LEFT) --x[0]; //방향에 따라 새로운 머리좌표(x[0],y[0])값을 변경 
		if (direction == RIGHT) ++x[0];
		if (direction == UP) --y[0];
		if (direction == DOWN) ++y[0];
		gotoxy(MAP_ADJ_X + x[i], MAP_ADJ_Y + y[i], "★"); //새로운 머리좌표값에 머리를 그림 
	}
	else if (reverse_value == 1) {
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		gotoxy(MAP_ADJ_X + x[0], MAP_ADJ_Y + y[0], "ㅇ"); //머리가 있던곳을 몸통으로 고침 
		if (direction == LEFT) --x[0]; //방향에 따라 새로운 머리좌표(x[0],y[0])값을 변경 
		if (direction == RIGHT) ++x[0];
		if (direction == UP) --y[0];
		if (direction == DOWN) ++y[0];
		gotoxy(MAP_ADJ_X + x[i], MAP_ADJ_Y + y[i], "★"); //새로운 머리좌표값에 머리를 그림 
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
}


//p키를 눌렀을 경우 게임을 일시 정지
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


//게임종료 함수
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
		gotoxy(MAP_ADJ_X + (MAP_X / 2) - 4, MAP_ADJ_Y + (MAP_Y / 2), "☆ BEST SCORE ☆");
	}
	Sleep(500);
	speed = speed + (food_count * accleration);
	speed = speed + (speed_up_item_count * accleration * (-3));
	food_count = 0;
	speed_up_item_count = 0;

	while (_kbhit()) _getch();
	key = _getch();
	title();
}


//음식 생성		♪
void food(void) {
	int i;
	int food_crush_on = 0;   //food가 뱀 몸통좌표에 생길 경우 on 
	int r = 0;            //난수 생성에 사동되는 변수 

	while (1) {
		food_crush_on = 0;
		srand((unsigned)time(NULL) + r);      //난수표생성 
		food_x = (rand() % (MAP_X - 2)) + 1;    //난수를 좌표값에 넣음 
		food_y = (rand() % (MAP_Y - 2)) + 1;

		for (i = 0; i<length; i++) {
			//food가 뱀 몸통과 겹치는지 확인
			if (food_x == x[i] && food_y == y[i]) {
				food_crush_on = 1;   //겹치면 food_crush_on 를 on 
				r++;
				break;
			}
		}

		//food_item이 함정과 겹치는지 확인
		if (food_x >= trap_x && food_x <= trap_x + 4 &&
			food_y >= trap_y && food_y <= trap_y + 3) {
			food_crush_on = 1;   //겹치면 speed_up_crush_on 를 on 
			r++;
		}

		if (food_crush_on == 1) {
			continue;   //겹쳤을 경우 while문을 다시 시작
		}

		//안겹쳤을 경우 좌표값에 food를 찍고
		gotoxy(MAP_ADJ_X + food_x, MAP_ADJ_Y + food_y, "♪");
		break;
	}
}


//리버스 아이템	※
void reverse_item(void) {
	int i;
	int reverse_item_crush_on = 0;   //reverse_item이 뱀 몸통좌표에 생길 경우 on 
	int r = 0;                  //난수 생성에 사동되는 변수 

	while (1) {
		reverse_item_crush_on = 0;
		srand((unsigned)time(NULL) + r);      //난수표생성 
		reverse_item_x = ((rand() / 2) % (MAP_X - 2)) + 1;	//난수를 좌표값에 넣음 
		reverse_item_y = ((rand() / 2) % (MAP_Y - 2)) + 1;

		for (i = 0; i<length; i++) {
			//reverse_item이 뱀 몸통과 겹치는지 확인
			if (reverse_item_x == x[i] && reverse_item_y == y[i]) {
				reverse_item_crush_on = 1;   //겹치면 reverse_crush_on 를 on 
				r++;
				break;
			}
		}

		//reverse_item이 함정과 겹치는지 확인
		if (reverse_item_x >= trap_x && reverse_item_x <= trap_x + 4 &&
			reverse_item_y >= trap_y && reverse_item_y <= trap_y + 3) {
			reverse_item_crush_on = 1;   //겹치면 speed_up_crush_on 를 on 
			r++;
		}

		if (reverse_item_crush_on == 1) {
			continue;   //겹쳤을 경우 while문을 다시 시작
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		gotoxy(MAP_ADJ_X + reverse_item_x, MAP_ADJ_Y + reverse_item_y, "※");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		break;
	}
}


//길이단축 아이템	♣
void length_item(void) {
	int i;

	int length_item_crush_on = 0;   //speed_up_item이 뱀 몸통좌표에 생길 경우 on 
	int r = 0;            //난수 생성에 사동되는 변수 

	while (1) {
		length_item_crush_on = 0;
		srand((unsigned)time(NULL) + r);      //난수표생성 
		length_item_x = ((rand() / 3) % (MAP_X - 2)) + 1;	//난수를 좌표값에 넣음 
		length_item_y = ((rand() / 3) % (MAP_Y - 2)) + 1;

		for (i = 0; i < length; i++) {
			//length_item이 뱀 몸통과 겹치는지 확인
			if (length_item_x == x[i] && length_item_y == y[i]) {
				length_item_crush_on = 1;   //겹치면 speed_up_crush_on 를 on 
				r++;
				break;
			}
		}

		//length_item이 함정과 겹치는지 확인
		if (length_item_x >= trap_x && length_item_x <= trap_x + 4 &&
			length_item_y >= trap_y && length_item_y <= trap_y + 3) {
			length_item_crush_on = 1;   //겹치면 speed_up_crush_on 를 on 
			r++;
		}

		if (length_item_crush_on == 1) {
			continue;   //겹쳤을 경우 while문을 다시 시작
		}

		gotoxy(MAP_ADJ_X + length_item_x, MAP_ADJ_Y + length_item_y, "♣");
		break;
	}
}


//속도변경 아이템	S
void speed_up_item(void) {
	int i;

	int speed_up_item_crush_on = 0;   //speed_up_item이 뱀 몸통좌표에 생길 경우 on 
	int r = 0;            //난수 생성에 사동되는 변수 

	while (1) {
		speed_up_item_crush_on = 0;
		srand((unsigned)time(NULL) + r);      //난수표생성 
		speed_up_item_x = ((rand() / 4) % (MAP_X - 2)) + 1;    //난수를 좌표값에 넣음 
		speed_up_item_y = ((rand() / 4) % (MAP_Y - 2)) + 1;

		for (i = 0; i < length; i++) {
			//speed_up_item이 뱀 몸통과 겹치는지 확인
			if (speed_up_item_x == x[i] && speed_up_item_y == y[i]) {
				speed_up_item_crush_on = 1;   //겹치면 speed_up_crush_on 를 on 
				r++;
				break;
			}
		}

		//speed_up_item이 함정과 겹치는지 확인
		if (speed_up_item_x >= trap_x && speed_up_item_x <= trap_x + 3 &&
			speed_up_item_y >= trap_y && speed_up_item_y <= trap_y + 2) {
			speed_up_item_crush_on = 1;   //겹치면 speed_up_crush_on 를 on 
			r++;
		}

		if (speed_up_item_crush_on == 1) {
			continue;   //겹쳤을 경우 while문을 다시 시작
		}

		gotoxy(MAP_ADJ_X + speed_up_item_x, MAP_ADJ_Y + speed_up_item_y, "S");
		break;
	}
}


//함정 아이템
void trap_item(void) {
	int i;

	int trap_crush_on = 0;   //trap_item가 뱀 몸통좌표에 생길 경우 on 
	int r = 0;            //난수 생성에 사동되는 변수 

	while (1) {
		trap_crush_on = 0;
		srand((unsigned)time(NULL) + r);      //난수표생성 
		trap_x = ((rand() + rand()) % (MAP_X - 8)) + 1;    //난수를 좌표값에 넣음 
		trap_y = ((rand() + rand()) % (MAP_Y - 6)) + 1;

		//trap_item가 뱀 몸통과 겹치는지 확인
		for (i = 0; i < length; i++) {
			if (x[i] >= trap_x && x[i] <= trap_x + 3 && 
				y[i] >= trap_y && y[i] <= trap_y + 2) {
				trap_crush_on = 1;   //겹치면 trap_crush_on 를 on 
				r++;
				break;
			}
		}

		if (trap_crush_on == 1) {
			continue;   //겹쳤을 경우 while문을 다시 시작
		}

		gotoxy(MAP_ADJ_X + trap_x, MAP_ADJ_Y + trap_y, " とㅡつ ");
		gotoxy(MAP_ADJ_X + trap_x, MAP_ADJ_Y + trap_y + 1, "ㅣ⊙⊙ㅣ");
		gotoxy(MAP_ADJ_X + trap_x, MAP_ADJ_Y + trap_y + 2, "ㅡㅡㅡㅡ");
		break;
	}
}


//옵션
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
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 8, "→");
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

			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 11, "→");
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

			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 14, "→");
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
			gotoxy(MAP_ADJ_X + 4, MAP_ADJ_Y + 17, "→");
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


//스테이터스
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
}


//스테이터스 오프
void status_off(void) {
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 0, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 1, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 2, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 3, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 4, "                  ");
	gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 5, "                  ");
}


//도움말(임시)
void help(void) {

}