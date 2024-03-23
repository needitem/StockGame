#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>

#include "article.h"
/***************************************메모장***********************************************/
//1. 저장/불러오기 만들어야됨(거의다함)
//2. 가격 그래프
//3. NEWSPAPER
//
//
//
//
//
//
//
/*********************************************************************************************/
#define STOCK_COUNT 8
#define START_MONEY 10000
#define TAX(money) (30000+(money)/4) 
/***************************************구조체************************************************/
struct STOCK////사용자가 보유한 주식----------나중에
{
	char* compname;
	int stockprice;//주식가격
	int prevprice;//1시간전주식가격
	int amount;//보유개수

};

typedef struct STOCK s_STOCK_type;
s_STOCK_type stock[STOCK_COUNT] = { {"samsoung"},{"LG"},{"Amazon"},{"Google"},{"bitcoin"},{"Aferica"},{"twitch"},{"Nexon"} };
s_STOCK_type* p_stock = &stock;


/*************************************전역변수************************************************/
long long int money = 0, loan = 0;
int mystock = 0;
int want_buy = 0;
int month = 0, day = 0, hour = 0;
int days[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
int graph[STOCK_COUNT][52] = { 0, };///////내가 선택한
//int stock_price[STOCK_COUNT] = { 0, }, prev_stock_price[STOCK_COUNT] = { 0, };
//= { "samsoung","LG","Amazon","Google","bitcoin","Aferica","twitch","Nexon" };
//int stock_bring[STOCK_COUNT] = { 0, }; // 각각의 주식 보유
bool good_news[STOCK_COUNT];//True면 뉴스에서 좋은소식 False면 나쁜소식-->주식의 미래 결정
bool stock_good_event[STOCK_COUNT];//뉴스랑 별개로 좋은확률
bool stock_bad_event[STOCK_COUNT];//뉴스량 별개로 나쁜확률
int hours = 0;


/***********************************기본적으로쓰일것들*****************************************/
void gotoxy(int x, int y)//커서이동
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void textcolor(int color_number)//글자색변경/////초록 2 빨강 4----검정 7----- 연초록 10 연빨강 14
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

void setcursortype(CURSOR_TYPE c)//커서타입변경. NOSURSUR만쓸거임
{
	CONSOLE_CURSOR_INFO CurInfo;
	switch (c)
	{
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

void clrscr(void)//화면지우기
{
	system("cls");
}

void clearbuffer()
{
	while (_kbhit())
	{
		_getch();
	}
	return 0;
}
void getkey(char* c)
{
	if (_kbhit())
	{
		*c = _getch();
	}
	return 0;
}
void title(char* title)
{
	clrscr();
	int i;
	printf("\n %s \n", title);
	for (i = 0; i < 40; i++)
	{
		printf("-");

	}
	printf("\n");

}

/********************************************************************************************************/


/******************************************함수정의****************************************************/

void f_save();//저장
void f_load();//로딩
void f_startlogo();//시작화면
void f_loan(int loan_money);//빚
void f_loanmenu();//빚 빌리는데 
void f_mainmenu();//메인메뉴
void f_stock_price_change();//주식가격변동
void f_game_reset();//초기화세팅
void f_interest();//이자
void f_buy_stock(int want_buy, int amount);//주식구매
void f_sell_stock(int want_buy, int amount);//주식판매
void f_startmain(); //주식회사 및 가격변동(증감 표시X)
void f_print_stock_price(int choice);//주식가격출력
void f_draw_newspaper(int choice);//주식 뉴스출력
void f_draw_graph(int choice);//주식가격그래프
void f_update_graph();// 그래프 업데이트
void f_payback();//돈갚기
void f_show_stock_price();//주식가격 메인화면 출력
void f_buy_menu(int choice);//구매메뉴
void f_sell_menu(int choice);//판매메뉴
void f_showstock();//보유주식들
void f_showstat();//스탯
void f_showcompany();//각회사의 보유주식
void f_stocksave();// 주식저장
/******************************************메인함수*****************************************************/

int main(void)
{
	srand(time(NULL));
	int i = 0, menu = 0, pausemenu;
	char select = '\0';
	char* pselect = &select;
	int cnt;
	cnt = month = day = hour = 0;



	f_startlogo();
	scanf_s("%d", &menu);

	f_game_reset();
	clrscr();
	f_loan(20000);


	switch (menu)//메뉴선택 후 게임진행
	{
	case 1:
	{
		f_stock_price_change();
		f_save();

		for (i = 1; i < STOCK_COUNT; i++)
		{
			if (rand() % 2 == 0)
			{
				*(good_news + i) = true;
			}
			else
			{
				*(good_news + i) = false;
			}
		}
		break;
	}
	case 2:
	{
		f_load();
		break;
	}
	case 3:
	{
		exit(0);
		break;
	}


	}
	clrscr();

	f_draw_newspaper(want_buy);

	while (1)//메인 게임시작
	{
		f_mainmenu();
		f_draw_graph(want_buy);

		if (money <= 0)
		{
			clrscr();
			printf("GG");
			Sleep(5000);
			exit(0);
		}

		gotoxy(52, 9 + want_buy);
		printf("  ");

		gotoxy(52, 10 + want_buy);
		printf("◀");

		gotoxy(52, 11 + want_buy);
		printf("  ");

		select = '\0';

		getkey(pselect);
		/*****************게임 작동키***********************************/
		switch (select)
		{
		case 'b':
		case 'B'://구매
		{
			f_buy_menu(want_buy);
			break;
		}
		case 'v':
		case 'V':
		{
			f_showstock();
			break;
		}

		case 27:
		{
			clrscr();

			title("일시정지");
			printf(" 1. 통  계\n\n 2. 대  출\n\n 3. 갚  기\n\n 4. 설  정\n\n 5. 끝내기\n\n Esc 돌아가기");

			pausemenu = _getch();

			switch (pausemenu)
			{
			case '1'://통계
			{
				f_showstat();
				break;
			}
			case '2'://대출
			{
				f_loanmenu();
				break;
			}
			case '3'://갚기
			{

				f_payback();
				clrscr();
				break;
			}
			case '4'://설정
			{
				clrscr();
				break;
			}
			case '5'://끝내기
			{
				exit(0);
				break;
			}
			case 27:
			{
				clrscr();
				break;
			}
			default:
			{
				break;
			}
			clrscr();

			}
			break;
		}
		case 's':
		case 'S'://판매
		{
			f_sell_menu(want_buy);
			break;
		}
		case 'e':
		case 'E'://저장
		{

			f_save();
			break;
		}
		case 'i':
		case 'I'://회사 정보
		{
			f_showcompany();
			break;
		}
		case '2':
		{
			if (want_buy < STOCK_COUNT - 1)
			{
				want_buy++;
				f_draw_newspaper(want_buy);
			}

			break;
		}
		case '8':
		{
			if (want_buy > 0)
			{
				want_buy--;
			}
			f_draw_newspaper(want_buy);
			break;
		}
		}
		clearbuffer();
		/**********************************************************************************************************/


		Sleep(20);
		cnt++;


		if (cnt % 50 == 0)///1초
		{
			for (i = 0; i < STOCK_COUNT; i++)
			{
				(p_stock + i)->prevprice = (p_stock + i)->stockprice;
			}
			hour++;
			hours++;

			f_stock_price_change();
			f_update_graph();

		}


		if (cnt % 100 == 0)
		{

		}



		gotoxy(0, 10);

		f_show_stock_price();




		if (hour == 0)
		{
			for (i = 0; i < STOCK_COUNT; i++)
			{
				if (rand() % 3 == 0)
				{
					if (*(stock_good_event + i) == false && *(stock_bad_event + i) == false)
					{
						*(good_news + i) = true;
					}
					else if (*(stock_good_event + i) == false && *(stock_bad_event + i) == false)
					{
						*(good_news + i) = false;
					}
				}
			}
		}
		if (hour > 23)//24시간=하루
		{
			f_save();
			hour = 0;
			f_interest();
			day++;
			srand(time(NULL));

		}
		if (day > *(days + month) - 1)//한달. 달마다 세금.
		{
			day = 0;
			month++;
			money -= TAX(money);
		}
		if (month > 11)//1년이 끝
		{
			month = 0;
		}

	}
}
/*****************************************쓰일 함수들******************************************************/

void f_save()
{
	int i = 0;
	FILE* save = fopen("stock_save.txt", "wb");

	if (save == NULL)
	{
		printf("입력 불가능입니다.");
		exit(1);
	};

	for (i = 0; i < STOCK_COUNT; i++)
	{
		fprintf(save, "%d\n", (p_stock + i)->amount);//Player가 가지고 있는 각각의 주식
	}
	fprintf(save, "\n\n");
	for (i = 0; i < STOCK_COUNT; i++)
	{
		fprintf(save, "%d,%d\n", (p_stock + i)->prevprice, (p_stock + i)->stockprice);//주식의 1시간전,현재 가격
	}
	for (i = 0; i < STOCK_COUNT; i++)
	{
		fprintf(save, "%d\n", *(good_news + i));// 주식의 미래결정
	}
	for (i = 0; i < STOCK_COUNT; i++)//뉴스랑 별개로 좋은확률
	{
		fprintf(save, "%d\n", *(stock_good_event + i));
	}
	for (i = 0; i < STOCK_COUNT; i++)//뉴스량 별개로 나쁜확률
	{
		fprintf(save, "%d\n", *(stock_bad_event + i));
	}

	fprintf(save, "돈:%lld\n 빚:%lld\n 보유주식개수:%d\n 달:%d 월:%d 시:%d", money, loan, mystock, month, day, hour);//Player 돈,빚,전체주식,달,일,시간,
	fclose(save);
}//미완
void f_load()
{

}//미완
void f_startlogo()
{
	printf("\n\n\t\t\t\t\t          새로운 게임  : 1");
	printf("\n\n\t\t\t\t\t          불 러 오 기  : 2");
	printf("\n\n\t\t\t\t\t          종       료  : 3");
	printf("\n\n\n\t\t\t\t\t\t    입력하세요:");
}
void f_loan(int loan_money)
{
	loan += loan_money;
	money += loan_money;
}
void f_payback()
{
	if (money >= loan)
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
void f_loanmenu()
{
	long long int loanmoney = 0;
	clrscr();
	title("대출");
	printf("\n 얼마를 대출받으시겠습니까? (한도 10억)");
	if (loan <= 1000000000)
	{
		scanf_s("%ld", &loanmoney);
	}

	f_loan(loanmoney);
}
void f_mainmenu()
{
	gotoxy(0, 1);
	printf("\n B 사기, S 팔기, E 저장, I 회사 정보, 8 / 2 회사 선택, Esc 메뉴");
	gotoxy(0, 3);
	printf("\n 현재 내 돈 : %lld원, 갚아야 할 돈 : %lld원, 보유 주식 주 : %d\n\n\n\n", money, loan, mystock);
	gotoxy(0, 5);
	printf("\n %d월 %d일 %d시   ", month + 1, day + 1, hour);
	printf("\n\n 이번 달 납부할 세금은 %d원입니다. %d일 남았습니다.\n", TAX(money), days[month] - day);

}
void f_startmain()
{

	int i = 0;
	for (i = 0; i < STOCK_COUNT; i++)
	{
		printf("회사 : %+10s     가격|:   %8d\n", (p_stock + i)->compname, (p_stock + i)->stockprice);
	}
}
void f_newsmenu()//미완
{

	int i = 0;
	for (i = 0; i < STOCK_COUNT; i++)
	{
		printf("회사 : %8s\n", (p_stock + i)->compname);
		printf("\n\n");
		printf("현재주가 : %10d\n", (p_stock + i)->stockprice);
		if (*(good_news + i) == true)
		{
			printf("전문가 의견 : 긍정적");
		}
		else
		{
			printf("전문가 의견 : 부정적");
		}
		printf("\n\n");


	}
}//미완
void f_stock_price_change()////////////////0원 이하로 안떨어지게
{
	int i = 0;
	for (i = 0; i < STOCK_COUNT; i++)
	{
		if (*(good_news + i) == true && *(stock_good_event + i) == false)
		{
			if (rand() % 2 == 0)
			{
				(p_stock + i)->stockprice += (rand() % 1000);
			}
			else
			{
				(p_stock + i)->stockprice -= (rand() % 100);
			}
		}
		else if (*(good_news + i) == true && *(stock_good_event + i) == true)
		{
			(p_stock + i)->stockprice += (rand() % 2000);
		}
		else if (*(good_news + i) == false && *(stock_good_event + i) == false)
		{
			if (rand() % 2 == 0)
			{
				(p_stock + i)->stockprice += (rand() % 100);
			}
			else
			{
				(p_stock + i)->stockprice -= (rand() % 1000);
			}

		}
		else
		{
			(p_stock + i)->stockprice -= (rand() % 2000);
		}

		if (((p_stock + i)->stockprice) < 2000)
		{
			(p_stock + i)->stockprice = 2000;
		}

	}

}
void f_game_reset()
{
	int i = 0;
	money = START_MONEY;
	for (i = 0; i < STOCK_COUNT; i++)
	{
		(p_stock + i)->stockprice = 10000;
		*(good_news + i) = true;
		*(stock_bad_event + i) = false;
		*(stock_good_event + i) = false;
	}
	mystock = 0;
	loan = 0;
	hour++;
	f_stock_price_change();
	setcursortype(NOCURSOR);


}
void f_interest()
{
	loan += (loan * 0.1);
}
void f_buy_stock(int want_buy, int amount)
{
	clrscr();

	int i = 0;
	for (i = 0; i < amount; i++)
	{
		if ((p_stock + want_buy)->stockprice <= money)
		{

			mystock++;
			money -= (p_stock + want_buy)->stockprice;
			(p_stock + want_buy)->amount++;
		}
	}


}
void f_sell_stock(int amounts)
{

	int i = 0;

	for (i = 0; i < amounts; i++)
	{
		if ((p_stock + want_buy)->amount >= amounts)
		{
			mystock--;
			money += (p_stock + want_buy)->stockprice;
			(p_stock + want_buy)->amount--;
		}

	}
}///미완
////보유한 주식이여만 팔수있음.
void f_showstock()
{
	clrscr();

	if (mystock <= 0)
	{
		printf("주식이 없습니다.");
	}
}

void f_print_stock_price(int i)
{
	int max, min;
	max = (p_stock + i)->stockprice - (p_stock + i)->prevprice;
	min = (p_stock + i)->prevprice - (p_stock + i)->stockprice;
	printf(" 회사 :");
	if ((p_stock + i)->prevprice < (p_stock + i)->stockprice)
	{
		textcolor(10);
		printf("%s", (p_stock + i)->compname);
		textcolor(7);
	}
	else
	{
		textcolor(12);
		printf("%s", (p_stock + i)->compname);
		textcolor(7);
	}
	printf("\t\t가격 : %8d원", (p_stock + i)->stockprice);
	if ((p_stock + i)->prevprice < (p_stock + i)->stockprice)
	{
		textcolor(2);
		printf("▲%7d원", max);
		textcolor(7);
	}
	else
	{
		textcolor(4);
		printf("▼%7d원", min);
		textcolor(7);
	}
	max = min = 0;
	printf("\n");
}
void f_draw_newspaper(int choice)
{

	gotoxy(88, 7);
	printf("목  대 신 문     %d월 %d일", month + 1, day + 1);
	gotoxy(76, 8);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

	gotoxy(70, 10);

	if (good_news[choice] == true)
	{
		printf(" %s%s ", (p_stock + choice)->compname, Goodnews_Headline[rand() % MAX_HEADLINE]);
	}
	else
	{
		printf(" %s%s ", (p_stock + choice)->compname, Badnews_Headline[rand() % MAX_HEADLINE]);
	}

	gotoxy(70, 11);

	if (good_news[choice] == true)
	{
		int i = 0, j = 0;
		int Article_rand = rand() % MAX_ARTICLE;
		printf(" %s", (p_stock + choice)->compname);

		while (GoodArticle[Article_rand][i] != '\0')
		{
			if (GoodArticle[Article_rand][i] == '\n')
			{

				gotoxy(65, 12 + j);
				j++;
			}
			else
			{
				putchar(GoodArticle[Article_rand][i]);
			}
			i++;
		}
	}
	else
	{
		int i = 0, j = 0;
		int Article_rand = rand() % MAX_ARTICLE;
		printf(" %s", (p_stock + choice)->compname);

		while (BadArticle[Article_rand][i] != '\0')
		{
			if (BadArticle[Article_rand][i] == '\n')
			{
				gotoxy(65, 12 + j);
				j++;
			}
			else
			{
				putchar(BadArticle[Article_rand][i]);
			}
			i++;
		}
	}
}


void f_draw_graph(int choice)
{
	int i, j = 0;
	gotoxy(0, 20);
#if 1
	printf(" 24000                                                                     \n 22000                                                                     \n 20000                                                                     "
		"\n 18000                                                                     \n 16000                                                                     \n 14000                                                                     "
		"\n 12000                                                                     \n 10000                                                                     \n  8000                                                                     "
		"\n  6000                                                                     \n  4000                                                                     \n  2000                                                                     \n                                                                              ");
#endif
	gotoxy(7, 25);

	for (i = 46; i >= 0; i--)
	{
		if (*(*(graph + choice) + i) > 0 && *(*(graph + choice) + i) < 26000)
		{
			gotoxy(7 + hours, 31 - ((int)((*(*(graph + choice) + i) % 26000) / 2000) - 1));
			printf("*");
		}
	}

}

void f_update_graph()
{
	int i = 0, j = 0;
	for (i = 0; i < STOCK_COUNT; i++)

	{
		for (j = 50; j > 0; j--)
		{
			graph[i][j + 1] = graph[i][j];
		}
		graph[i][0] = (p_stock + i)->stockprice;
	}
}
void f_show_stock_price()
{
	int i = 0;
	for (i = 0; i < STOCK_COUNT; i++)
	{
		f_print_stock_price(i);

	}
}
void f_buy_menu(int choice)
{
	int amount = 0;
	clrscr();
	title("주식사기");
	printf("몇개를 구매할겁니까:");


	scanf_s("%d", &amount);

	getchar();
	if (amount > 0)
	{
		f_buy_stock(choice, amount);
	}

}
void f_sell_menu(int choice)
{
	int amount = 0;
	clrscr();
	title("주식팔기");
	printf("몇 개의 주식을 팔겠습니까:");

	scanf_s("%d", &amount);

	printf("%d", amount);

	if (amount > 0)
	{
		f_sell_stock(amount);
	}


	clrscr();
}
void f_showstat()
{
	int i = 0;
	clrscr();
	title("통계");
	printf("\n현재 가진돈:%lld 주식개수:%d\n", money, mystock);
	for (i = 0; i < STOCK_COUNT; i++)
	{
		if ((p_stock + i)->amount > 0)
		{
			printf("%s : %d개\n", (p_stock + i)->compname, (p_stock + i)->amount);
		}
	}
	getchar();

	clrscr();
}
void f_showcompany()
{
	clrscr();

	if (p_stock->amount <= 0)
	{
		printf("현재 주식이 없습니다.");
	}
	else
	{
		title("보유 주식");
		printf("현재 Player가 %s의 가지고 있는 주식은 %d개 입니다.\n", (p_stock + want_buy)->compname, (p_stock + want_buy)->amount);
		printf("처음화면으로 돌아가시면 ESC를 누르세요..");
	}
	_getch();
	clrscr();
}

/*************************************************************************************************************/