#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>

#include "article.h"
/***************************************�޸���***********************************************/
//1. ����/�ҷ����� �����ߵ�(���Ǵ���)
//2. ���� �׷���
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
/***************************************����ü************************************************/
struct STOCK////����ڰ� ������ �ֽ�----------���߿�
{
	char* compname;
	int stockprice;//�ֽİ���
	int prevprice;//1�ð����ֽİ���
	int amount;//��������

};

typedef struct STOCK s_STOCK_type;
s_STOCK_type stock[STOCK_COUNT] = { {"samsoung"},{"LG"},{"Amazon"},{"Google"},{"bitcoin"},{"Aferica"},{"twitch"},{"Nexon"} };
s_STOCK_type* p_stock = &stock;


/*************************************��������************************************************/
long long int money = 0, loan = 0;
int mystock = 0;
int want_buy = 0;
int month = 0, day = 0, hour = 0;
int days[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
int graph[STOCK_COUNT][52] = { 0, };///////���� ������
//int stock_price[STOCK_COUNT] = { 0, }, prev_stock_price[STOCK_COUNT] = { 0, };
//= { "samsoung","LG","Amazon","Google","bitcoin","Aferica","twitch","Nexon" };
//int stock_bring[STOCK_COUNT] = { 0, }; // ������ �ֽ� ����
bool good_news[STOCK_COUNT];//True�� �������� �����ҽ� False�� ���ۼҽ�-->�ֽ��� �̷� ����
bool stock_good_event[STOCK_COUNT];//������ ������ ����Ȯ��
bool stock_bad_event[STOCK_COUNT];//������ ������ ����Ȯ��
int hours = 0;


/***********************************�⺻�����ξ��ϰ͵�*****************************************/
void gotoxy(int x, int y)//Ŀ���̵�
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void textcolor(int color_number)//���ڻ�����/////�ʷ� 2 ���� 4----���� 7----- ���ʷ� 10 ������ 14
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

void setcursortype(CURSOR_TYPE c)//Ŀ��Ÿ�Ժ���. NOSURSUR��������
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

void clrscr(void)//ȭ�������
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


/******************************************�Լ�����****************************************************/

void f_save();//����
void f_load();//�ε�
void f_startlogo();//����ȭ��
void f_loan(int loan_money);//��
void f_loanmenu();//�� �����µ� 
void f_mainmenu();//���θ޴�
void f_stock_price_change();//�ֽİ��ݺ���
void f_game_reset();//�ʱ�ȭ����
void f_interest();//����
void f_buy_stock(int want_buy, int amount);//�ֽı���
void f_sell_stock(int want_buy, int amount);//�ֽ��Ǹ�
void f_startmain(); //�ֽ�ȸ�� �� ���ݺ���(���� ǥ��X)
void f_print_stock_price(int choice);//�ֽİ������
void f_draw_newspaper(int choice);//�ֽ� �������
void f_draw_graph(int choice);//�ֽİ��ݱ׷���
void f_update_graph();// �׷��� ������Ʈ
void f_payback();//������
void f_show_stock_price();//�ֽİ��� ����ȭ�� ���
void f_buy_menu(int choice);//���Ÿ޴�
void f_sell_menu(int choice);//�ǸŸ޴�
void f_showstock();//�����ֽĵ�
void f_showstat();//����
void f_showcompany();//��ȸ���� �����ֽ�
void f_stocksave();// �ֽ�����
/******************************************�����Լ�*****************************************************/

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


	switch (menu)//�޴����� �� ��������
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

	while (1)//���� ���ӽ���
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
		printf("��");

		gotoxy(52, 11 + want_buy);
		printf("  ");

		select = '\0';

		getkey(pselect);
		/*****************���� �۵�Ű***********************************/
		switch (select)
		{
		case 'b':
		case 'B'://����
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

			title("�Ͻ�����");
			printf(" 1. ��  ��\n\n 2. ��  ��\n\n 3. ��  ��\n\n 4. ��  ��\n\n 5. ������\n\n Esc ���ư���");

			pausemenu = _getch();

			switch (pausemenu)
			{
			case '1'://���
			{
				f_showstat();
				break;
			}
			case '2'://����
			{
				f_loanmenu();
				break;
			}
			case '3'://����
			{

				f_payback();
				clrscr();
				break;
			}
			case '4'://����
			{
				clrscr();
				break;
			}
			case '5'://������
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
		case 'S'://�Ǹ�
		{
			f_sell_menu(want_buy);
			break;
		}
		case 'e':
		case 'E'://����
		{

			f_save();
			break;
		}
		case 'i':
		case 'I'://ȸ�� ����
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


		if (cnt % 50 == 0)///1��
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
		if (hour > 23)//24�ð�=�Ϸ�
		{
			f_save();
			hour = 0;
			f_interest();
			day++;
			srand(time(NULL));

		}
		if (day > *(days + month) - 1)//�Ѵ�. �޸��� ����.
		{
			day = 0;
			month++;
			money -= TAX(money);
		}
		if (month > 11)//1���� ��
		{
			month = 0;
		}

	}
}
/*****************************************���� �Լ���******************************************************/

void f_save()
{
	int i = 0;
	FILE* save = fopen("stock_save.txt", "wb");

	if (save == NULL)
	{
		printf("�Է� �Ұ����Դϴ�.");
		exit(1);
	};

	for (i = 0; i < STOCK_COUNT; i++)
	{
		fprintf(save, "%d\n", (p_stock + i)->amount);//Player�� ������ �ִ� ������ �ֽ�
	}
	fprintf(save, "\n\n");
	for (i = 0; i < STOCK_COUNT; i++)
	{
		fprintf(save, "%d,%d\n", (p_stock + i)->prevprice, (p_stock + i)->stockprice);//�ֽ��� 1�ð���,���� ����
	}
	for (i = 0; i < STOCK_COUNT; i++)
	{
		fprintf(save, "%d\n", *(good_news + i));// �ֽ��� �̷�����
	}
	for (i = 0; i < STOCK_COUNT; i++)//������ ������ ����Ȯ��
	{
		fprintf(save, "%d\n", *(stock_good_event + i));
	}
	for (i = 0; i < STOCK_COUNT; i++)//������ ������ ����Ȯ��
	{
		fprintf(save, "%d\n", *(stock_bad_event + i));
	}

	fprintf(save, "��:%lld\n ��:%lld\n �����ֽİ���:%d\n ��:%d ��:%d ��:%d", money, loan, mystock, month, day, hour);//Player ��,��,��ü�ֽ�,��,��,�ð�,
	fclose(save);
}//�̿�
void f_load()
{

}//�̿�
void f_startlogo()
{
	printf("\n\n\t\t\t\t\t          ���ο� ����  : 1");
	printf("\n\n\t\t\t\t\t          �� �� �� ��  : 2");
	printf("\n\n\t\t\t\t\t          ��       ��  : 3");
	printf("\n\n\n\t\t\t\t\t\t    �Է��ϼ���:");
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
	title("����");
	printf("\n �󸶸� ��������ðڽ��ϱ�? (�ѵ� 10��)");
	if (loan <= 1000000000)
	{
		scanf_s("%ld", &loanmoney);
	}

	f_loan(loanmoney);
}
void f_mainmenu()
{
	gotoxy(0, 1);
	printf("\n B ���, S �ȱ�, E ����, I ȸ�� ����, 8 / 2 ȸ�� ����, Esc �޴�");
	gotoxy(0, 3);
	printf("\n ���� �� �� : %lld��, ���ƾ� �� �� : %lld��, ���� �ֽ� �� : %d\n\n\n\n", money, loan, mystock);
	gotoxy(0, 5);
	printf("\n %d�� %d�� %d��   ", month + 1, day + 1, hour);
	printf("\n\n �̹� �� ������ ������ %d���Դϴ�. %d�� ���ҽ��ϴ�.\n", TAX(money), days[month] - day);

}
void f_startmain()
{

	int i = 0;
	for (i = 0; i < STOCK_COUNT; i++)
	{
		printf("ȸ�� : %+10s     ����|:   %8d\n", (p_stock + i)->compname, (p_stock + i)->stockprice);
	}
}
void f_newsmenu()//�̿�
{

	int i = 0;
	for (i = 0; i < STOCK_COUNT; i++)
	{
		printf("ȸ�� : %8s\n", (p_stock + i)->compname);
		printf("\n\n");
		printf("�����ְ� : %10d\n", (p_stock + i)->stockprice);
		if (*(good_news + i) == true)
		{
			printf("������ �ǰ� : ������");
		}
		else
		{
			printf("������ �ǰ� : ������");
		}
		printf("\n\n");


	}
}//�̿�
void f_stock_price_change()////////////////0�� ���Ϸ� �ȶ�������
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
}///�̿�
////������ �ֽ��̿��� �ȼ�����.
void f_showstock()
{
	clrscr();

	if (mystock <= 0)
	{
		printf("�ֽ��� �����ϴ�.");
	}
}

void f_print_stock_price(int i)
{
	int max, min;
	max = (p_stock + i)->stockprice - (p_stock + i)->prevprice;
	min = (p_stock + i)->prevprice - (p_stock + i)->stockprice;
	printf(" ȸ�� :");
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
	printf("\t\t���� : %8d��", (p_stock + i)->stockprice);
	if ((p_stock + i)->prevprice < (p_stock + i)->stockprice)
	{
		textcolor(2);
		printf("��%7d��", max);
		textcolor(7);
	}
	else
	{
		textcolor(4);
		printf("��%7d��", min);
		textcolor(7);
	}
	max = min = 0;
	printf("\n");
}
void f_draw_newspaper(int choice)
{

	gotoxy(88, 7);
	printf("��  �� �� ��     %d�� %d��", month + 1, day + 1);
	gotoxy(76, 8);
	printf("��������������������������������������������������������������������������");

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
	title("�ֽĻ��");
	printf("��� �����Ұ̴ϱ�:");


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
	title("�ֽ��ȱ�");
	printf("�� ���� �ֽ��� �Ȱڽ��ϱ�:");

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
	title("���");
	printf("\n���� ������:%lld �ֽİ���:%d\n", money, mystock);
	for (i = 0; i < STOCK_COUNT; i++)
	{
		if ((p_stock + i)->amount > 0)
		{
			printf("%s : %d��\n", (p_stock + i)->compname, (p_stock + i)->amount);
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
		printf("���� �ֽ��� �����ϴ�.");
	}
	else
	{
		title("���� �ֽ�");
		printf("���� Player�� %s�� ������ �ִ� �ֽ��� %d�� �Դϴ�.\n", (p_stock + want_buy)->compname, (p_stock + want_buy)->amount);
		printf("ó��ȭ������ ���ư��ø� ESC�� ��������..");
	}
	_getch();
	clrscr();
}

/*************************************************************************************************************/