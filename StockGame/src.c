#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>
#include "article.h"

#define STOCK_SIZE 8
#define START_MONEY 10000
#define TAX(money) 30000+(money)*0.25

struct STOCK {
	char* name;
	int price;
	int prevPrice;
	int prevprevPrice;
	int amount;
	int good;
	int event
};

typedef struct STOCK STOCK;
STOCK stock[STOCK_SIZE] = {
	{"SAMSUNG", 1000000, 1000000, 1000000, 0, 0, 0},
	{"LG", 500000, 500000, 500000, 0, 0, 0},
	{"SK", 200000, 200000, 200000, 0, 0, 0},
	{"HYUNDAI", 300000, 300000, 300000, 0, 0, 0},
	{"KIA", 400000, 400000, 400000, 0, 0, 0},
	{"POSCO", 600000, 600000, 600000, 0, 0, 0},
	{"NAVER", 700000, 700000, 700000, 0, 0, 0},
	{"KAKAO", 800000, 800000, 800000, 0, 0, 0}
};
STOCK* pStock = stock;

int money = START_MONEY;
int loan = 0;
int enemyMoney = START_MONEY;
int myStock = 0;
int wantStock = 0;
int month = 1; int day = 1; int hour = 0;
int aistock[STOCK_SIZE] = { 0, };
int aistockPrice[STOCK_SIZE] = { 0, };
int* pAistock = aistock;
int days[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
int aiRandom = 0;
int SPEED = 50;

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void textcolor(int color_number) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}
void setcursor(CURSOR_TYPE c) {
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
void cls() {
	system("cls");
}
int getkey(char* c) {
	if(_kbhit()) {
		*c = _getch();
		return 0;
	}
	return 1;
}
void clearBuffer() {
	while(getchar() != '\n');
}
void title(char* title) {
	cls();
	gotoxy(0, 0);
	printf("Stock Game");
	gotoxy(0, 1);
	printf("%s", title);
}

void save();
void load();
void startlogo();
void floan(long long int loan);
void loanmenu();
void mainmenu();
void stockPriceChange();
void gameReset();
void interest();
void setting();
void setSpeed();
void buyStock(int stock, int amt);
void sellStock(int amt);
void startMain();
void printStock(int stock);
void drawNews();
void payback();
void showStockPrice();
void buyMenu(int choice);
void sellMenu(int choice);
void showStat();
void newsMenu();
void aiBuy(int stock,int price, int amt);
void aiSell(int stock, int price, int amt);
void showAiStat();

int main() {
	srand((unsigned int)time(NULL));
	int i=0, pausemenu=0;
	char menu = 0;
	char select = 0;
	int cnt = 0;
	cnt = month = day = hour = 0;
	
	do {
		cls();
		startlogo();
		scanf("%c", &menu, sizeof(menu)); //1: new game 2: load game 3: exit
		gameReset();
		cls();
		floan(20000);

		switch (menu)
		{
			case '1':
			stockPriceChange();
			save();

			for (i=0; i<STOCK_SIZE; i++) {
				if (rand()% 3 == 0) {
					(pStock + i)->event = true;
				}
				else {
					(pStock + i)->event = false;
				}
			}
			break;
			case '2':
				load();
				break;
			case '3':
				exit(0);
				break;
		default:
			continue;
			break;
		}

	} while (1);
	cls();
	drawNews();

	while (true)
	{
		mainmenu();
		newsMenu();

		if(money<0) {
			title("You are bankrupted");
			printf("You are bankrupted\n");
			printf("Press any key to continue");
			getch();
			break;
		};

		gotoxy(52, 9 + wantStock);
		printf("");
		gotoxy(52, 10 + wantStock);
		printf("<");
		gotoxy(52, 11 + wantStock);
		printf("");

		printf(" ");
		select = 0;

		getkey(&select);

		switch (select)
		{
		case 'j':
		{
			money += 100000;
			break;
		}
		case 'B':
		case 'b':
		{
			buyMenu(wantStock);
			break;
		}
		case 27:
		{
			cls();
			title("Pause");
			printf("1. 통계\n\n 2. 대출\n\n 3. 갚기\n\n 4. 설정\n\n 5. 끝내기\n\n ESC 돌아가기");
			pausemenu = _getch();

			switch (pausemenu)
			{
			case 1:
			{
				showStat();
				break;
			}
			case 2:
			{
				loanmenu();
				break;
			}
			case 3:
			{
				payback();
				cls();
				break;
			}
			case 4:
			{
				setting();
				cls();
				break;
			}
			case 5:
			{
				save();
				exit(0);
				break;
			}
			case 27:
			{
				cls();
				break;
			}
			default:
				break;
			}
		}
		case 'S':
		case 's':
		{
			sellMenu(wantStock);
			break;
		}
		case 'E':
		case 'e':
		{
			save();
			exit(0);
			break;
		}
		case '2':
		{
			if (wantStock < STOCK_SIZE - 1) {
				wantStock++;
			}
			drawNews(wantStock);
			break;
		}
		case '8':
		{
			if (wantStock > 0) {
				wantStock--;
			}
			drawNews(wantStock);
			break;
		}

		default:
			break;
		}

		Sleep(1000 / SPEED);
		cnt++;
		if (cnt % 50 == 1)
		{
			for (i = 0; i < STOCK_SIZE; i++)
			{
				(pStock + i)->prevprevPrice = (pStock + i)->prevPrice;
				(pStock + i)->prevPrice = (pStock + i)->price;
			}
			hour++;
			stockPriceChange();
		}
		if (cnt % 50 == 0)
		{
			for (i = 0; i < STOCK_SIZE; i++)
			{
				aiRandom = rand() % 3 + 1;
				if ((pStock + i)->price - (pStock + i)->prevPrice > 300)
				{
					aiBuy(i, (pStock + i)->prevPrice, aiRandom);
				}
				else if((pStock + i)->price < (pStock + i)->prevPrice)
				{
					aiSell(i, (pStock + i)->prevPrice, *(aistock + i + 1));
				}


			}
		}

		gotoxy(0, 10);
		showStockPrice();
		showaiStat();

		if(hour == 0) {
			for(i=0; i<STOCK_SIZE; i++) {
				if(rand()%3 == 0) {
					if((pStock + i)->event == true) {
						(pStock + i)->good = true;
					}
					else {
						(pStock + i)->good = false;
					}
				}
			}
		}
		if (hour > 23)
		{
			save();
			hour = 0;
			interest();
			day++;
			srand((unsigned int)time(NULL));
		}
		if (day > days[month - 1])
		{
			month++;
			day = 1;
			money -= TAX(money);
		}
		if (month > 11)
		{
			if(money - loan > 0)
			{
				title("You win");
				printf("You win\n");
				printf("Press any key to continue");
				getch();
				break;
			}
			else {
				title("You are bankrupted");
				printf("You are bankrupted\n");
				printf("Press any key to continue");
				getch();
				break;
			 }
			month = 1;
		}

	}

	return 0;

}

void save() {
	FILE* file = fopen("save.txt", "w");
	if (file == NULL) {
		printf("파일을 열 수 없습니다.\n");
		return;
	}
	for (int i = 0; i < STOCK_SIZE; i++) {
		fprintf(file, "%d %d %d %d %d %d %d\n", (pStock + i)->price, (pStock + i)->prevPrice, (pStock + i)->prevprevPrice, (pStock + i)->amount, (pStock + i)->good, (pStock + i)->event);
	}
	fprintf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", money, loan, enemyMoney, myStock, wantStock, month, day, hour, aistock[0], aistock[1], aistock[2], aistock[3], aistock[4], aistock[5], aistock[6], aistock[7]);
	fclose(file);
}

void load() {
	FILE* file = fopen("save.txt", "r");
	if (file == NULL) {
		printf("파일을 열 수 없습니다.\n");
		return;
	}
	for (int i = 0; i < STOCK_SIZE; i++) {
		fscanf(file, "%d %d %d %d %d %d %d", &(pStock + i)->price, &(pStock + i)->prevPrice, &(pStock + i)->prevprevPrice, &(pStock + i)->amount, &(pStock + i)->good, &(pStock + i)->event);
	}
	fscanf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &money, &loan, &enemyMoney, &myStock, &wantStock, &month, &day, &hour, &aistock[0], &aistock[1], &aistock[2], &aistock[3], &aistock[4], &aistock[5], &aistock[6], &aistock[7]);
	fclose(file);
}

void startlogo() {
	gotoxy(0, 0);
	printf("Stock Game");
	gotoxy(0, 1);
	printf("1. New Game\n");
	gotoxy(0, 2);
	printf("2. Load Game\n");
	gotoxy(0, 3);
	printf("3. Exit\n");
}

void floan(long long int loan) {
	money += loan;
	loan += loan;

}

void payback() {
	if (money > loan)
	{
		money -= loan;
		loan = 0;
	}
	else
	{
		loan -= money;
		money = 0;
	}
}

void loanmenu() {
	int loanmoney = 0;
	int check = 1;

	do
	{
		cls();
		title("대출");
		printf("\n얼마를 대출받으시겠습니까? (한도 130만원)");
		scanf("%d", &loanmoney);
		if(loanmoney > 1300000)
		{
			printf("한도를 초과하였습니다.\n");
			getch();
		}
		else
		{
			floan(loanmoney);
			check = 0;
		}
	} while (check);
}

void mainmenu() {
	gotoxy(0, 0);
	printf("\n B : 매수\n S: 매도\n E: 저장\n 2/8 선택, ESC 메뉴");
	gotoxy(0, 3);
	printf("\n 내 돈: %d원, 빚: %d원, 보유주식: %d", money, loan, myStock);
	gotoxy(0, 5);
	printf("\n %d월 %d일 %d시", month, day, hour);
	printf("\n\n 이번달 납부할 세금은 %d원입니다.", TAX(money));
}

void stockPriceChange() {
	int i = 0;
	for (i = 0; i < STOCK_SIZE; i++) {
		(pStock + i)->price = (pStock + i)->prevPrice + (rand() % 10000 - 5000);
	}
}

void gameReset() {
	money = START_MONEY;
	loan = 0;
	enemyMoney = START_MONEY;
	myStock = 0;
	wantStock = 0;
	month = 1; day = 1; hour = 0;
}

void interest() {
	loan = loan * 1.1;
}

void setting() {
	int menu = 0;
	cls();
	title("설정");
	printf("1. 속도\n\n 2. 돌아가기");
	menu = _getch();
	switch (menu)
	{
	case 1:
	{
		setSpeed();
		break;
	}
	case 2:
	{
		cls();
		break;
	}
	default:
		break;
	}
}

void setSpeed() {
	int speed = 0;
	cls();
	title("속도");
	printf("1. 빠름\n\n 2. 보통\n\n 3. 느림");
	speed = _getch();
	switch (speed)
	{
	case 1:
	{
		SPEED = 25;
		break;
	}
	case 2:
	{
		SPEED = 50;
		break;
	}
	case 3:
	{
		SPEED = 100;
		break;
	}
	default:
		break;
	}
}

void buyStock(int stock, int amt) {
	if (money >= (pStock + stock)->price * amt) {
		money -= (pStock + stock)->price * amt;
		(pStock + stock)->amount += amt;
		myStock += amt;
	}
	else {
		printf("돈이 부족합니다.\n");
	}
}

void sellStock(int amt) {
	if (myStock >= amt) {
		money += (pStock + wantStock)->price * amt;
		(pStock + wantStock)->amount -= amt;
		myStock -= amt;
	}
	else {
		printf("주식이 부족합니다.\n");
	}
}

void startMain() {
	cls();
	drawNews();
}

void printStock(int stock) {
	gotoxy(0, 0);
	printf("주식명: %s\n", (pStock + stock)->name);
	printf("가격: %d\n", (pStock + stock)->price);
	printf("전일가: %d\n", (pStock + stock)->prevPrice);
	printf("전전일가: %d\n", (pStock + stock)->prevprevPrice);
	printf("보유주식: %d\n", (pStock + stock)->amount);
}

void drawNews() {
	int i = 0;
	cls();
	title("뉴스");
	for (i = 0; i < STOCK_SIZE; i++) {
		if ((pStock + i)->event == true) {
			printf("%s의 이벤트가 발생했습니다.\n", (pStock + i)->name);
		}
	}
}

void showStockPrice() {
	int i = 0;
	for (i = 0; i < STOCK_SIZE; i++) {
		gotoxy(0, 10 + i);
		printf("%s: %d", (pStock + i)->name, (pStock + i)->price);
	}
}

void buyMenu(int choice) {
	int amt = 0;
	cls();
	title("매수");
	printStock(choice);
	printf("얼마나 매수하시겠습니까?");
	scanf("%d", &amt);
	buyStock(choice, amt);
}

void sellMenu(int choice) {
	int amt = 0;
	cls();
	title("매도");
	printStock(choice);
	printf("얼마나 매도하시겠습니까?");
	scanf("%d", &amt);
	sellStock(amt);
}

void showStat() {
	cls();
	title("통계");
	printf("내 돈: %d원\n", money);
	printf("빚: %d원\n", loan);
	printf("보유주식: %d\n", myStock);
	printf("적 돈: %d원\n", enemyMoney);
	printf("적 보유주식: %d\n", wantStock);
	printf("현재 시간: %d월 %d일 %d시\n", month, day, hour);
	printf("세금: %d원\n", TAX(money));
	printf("속도: %d\n", SPEED);
	printf("Press any key to continue");
	getch();
}

void newsMenu() {
	int i = 0;
	for (i = 0; i < STOCK_SIZE; i++) {
		if ((pStock + i)->event == true) {
			drawNews();
		}
	}
}

void aiBuy(int stock, int price, int amt) {
	if (enemyMoney >= price * amt) {
		enemyMoney -= price * amt;
		(pStock + stock)->amount += amt;
		wantStock += amt;
	}
}

void aiSell(int stock, int price, int amt) {
	if (wantStock >= amt) {
		enemyMoney += price * amt;
		(pStock + stock)->amount -= amt;
		wantStock -= amt;
	}
}

void showaiStat() {
	gotoxy(0, 20);
	printf("적 돈: %d원\n", enemyMoney);
	printf("적 보유주식: %d\n", wantStock);
}

// Path: StockGame/src.h


