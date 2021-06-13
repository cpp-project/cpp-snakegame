
#include "sclass.h"
#include <locale.h>
#include <ctime>

time_t startgameT;
time_t overgameT;

using std::fstream; using std::ifstream; using std::ofstream;


//------------------------------------
bool writeEndAndGetInput();
void printScore(WINDOW*, int, int, int);
void printMission(WINDOW*, int, int);
void create(WINDOW*, Snake&, char*, int, int);
void proccesInput(WINDOW*, Snake&, int);
int MAP(int x, int y);
int main()
{
	setlocale(LC_ALL, "");
	count_growth = 0;
	count_poison = 0;
	count_gate = 0;
	com_1 = ' ';
	com_4 = ' ';
	com_3 = ' ';
	com_2 = ' ';
	int sz;
	int x,y;
	// getmaxyx(stdscr, y, x);
	x = 100;
	y = 30;
	initscr();
	noecho();
	// reseize_term(1000, 500);

	//render frame

	//resize_term(100, 50); //-> 원래는 전체 터미널 사이즈 설정하는건데 이건 나중에 수정해야할것같아서 놔뒀어
    start_color();

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
	int coin =0;
	mvprintw(y/2-3,x/2-15,"스네이크 게임을 시작합니다*^^*");
	mvprintw(y/2-2,x/2-15,"enter키를 눌러주세요"); refresh();   //default window 테두리 *
	getch();
	endwin();

	WINDOW *game = newwin(y+2, x-38, 0, 0); //height, width, startY, startX

	wbkgd(game, COLOR_PAIR(1));
    wattron(game, COLOR_PAIR(1));
	wborder(game, '|','|','-','-','+','+','+','+');

	nodelay(game, TRUE);
	keypad(game, TRUE);

	WINDOW *score = newwin(y/2 + 1, x-60, 0,x-37);
	wbkgd(score, COLOR_PAIR(1));
    wattron(score, COLOR_PAIR(1));
	wborder(score, '|','|','-','-','+','+','+','+');
	wrefresh(score);
	nodelay(score, TRUE);

	WINDOW *mission = newwin(y/2 + 1, x-60, y/2 + 1,x-37);
	wbkgd(mission, COLOR_PAIR(1));
    wattron(mission, COLOR_PAIR(1));
	wborder(mission, '|','|','-','-','+','+','+','+');
	wrefresh(mission);
	nodelay(mission, TRUE);


	//game loop
	do
	{
		startgameT = time(NULL);
		Snake snake(y, x-40);
		refresh();
		wrefresh(game);
		sz = snake.getBodySize();
		printScore(score, 0, 1,sz);
		printMission(mission,0,1);
		count_growth = 0;
		count_poison = 0;
		count_gate = 0;

		while(!snake.get_exit())
		{
			char *tbl = snake.check_map();
			//process data
			//draw
			create(game, snake, tbl, snake.get_height(), snake.get_width());

			printScore(score, snake.get_points(), snake.get_level(), snake.getBodySize());
			printMission(mission, snake.get_points(), snake.get_level());

			int input = wgetch(game);
			proccesInput(game, snake, input);
			if(snake.get_exit())break;
			snake.checkPoints();
			snake.body_move();
			snake.make_move();
			snake.checkForGrowth();
			snake.checkForPoison();
			snake.checkForwall();
			snake.check_gate();
			snake.change_gate();
			snake.mission();
			snake.game_end(); //몸 길이 3 미만일때 죽음



			//-----[ SLEEP ]-------
			#ifdef WIN32
			#else
			usleep(snake.get_speed());
			#endif
			//---------------------
		}

	}while(writeEndAndGetInput());

	delwin(score);
	delwin(game);
	delwin(mission);
	endwin();
}

bool writeEndAndGetInput()
{
	overgameT = time(NULL);
	int score = count_growth + max_count - count_poison + count_gate*2;
	WINDOW* endwin = newwin(32,110, 0, 0);
	box(endwin, 0, 0);
	nodelay(endwin, TRUE);
	keypad(endwin, TRUE);
	mvwprintw(endwin, 14, 40, "GAME OVER");
	mvwprintw(endwin, 15, 40, "Game Score : %d", score);
	mvwprintw(endwin, 16, 40, "Play Time : %d second", overgameT - startgameT);
	mvwprintw(endwin, 17, 40, "Total Score : %d", score + 1000 - (overgameT - startgameT));
	mvwprintw(endwin, 18, 40, "Press [q] to quit.");
	int c;
	do{
		c = wgetch(endwin);
	}while(c!=10 && c!=' ' && c!='q' && c!='Q');
	werase(endwin);
	wrefresh(endwin);
	delwin(endwin);
	return (c=='q' || c=='Q')?false:true;
}


void printScore(WINDOW* w, int score, int level, int sz)
{
	werase(w);
	wbkgd(w, COLOR_PAIR(1));
    wattron(w, COLOR_PAIR(1));
	wborder(w, '|','|','-','-','+','+','+','+');
	 mvwprintw(w, 1, 2, "스코어 보드");
    mvwprintw(w, 4, 2, "B: 현재 길이 / 최대 길이 %d/%d",sz,mission_4);
    mvwprintw(w, 6, 2, "+: Growth 아이템 개수 %d",count_growth);
    mvwprintw(w, 8, 2, "-: Poison 아이템 개수 %d",count_poison);
    mvwprintw(w, 10, 2, "G: 게이트 사용 횟수 %d",count_gate);
	wrefresh(w);
}

void printMission(WINDOW* w, int score, int level)
{
	werase(w);
	wbkgd(w, COLOR_PAIR(1));
    wattron(w, COLOR_PAIR(1));
	wborder(w, '|','|','-','-','+','+','+','+');
    mvwprintw(w, 1, 2, "미션 보드");
    mvwprintw(w, 4, 2, "B: %d   (%c)",mission_4,com_4);
    mvwprintw(w, 5, 2, "+: %d   (%c)",mission_1,com_1);
    mvwprintw(w, 6, 2, "-: %d   (%c)",mission_2,com_2);
    mvwprintw(w, 7, 2, "G: %d   (%c)",mission_3,com_3);
	wrefresh(w);
}
void create(WINDOW* game, Snake& snake, char* table, int height, int width)
{
	werase(game);
	wbkgd(game, COLOR_PAIR(1));
    wattron(game, COLOR_PAIR(1));
	wborder(game, '|','|','-','-','+','+','+','+');



	for(int i=0; i<(height*width); ++i)
	{
		if(table[i]!=' ')
		{

			int y = i/width;
			int x = i-(y*width);

			int d;
			switch(table[i])
			{
				case 'a':
					mvwprintw(game, 1+y,1+x, "*");
					break;
				case 'x':
					mvwprintw(game, 1+y,1+x, "X");
					break;
				case '1':
						mvwprintw(game, 1+y,1+x, "=");
						break;
				case '2':
						mvwprintw(game, 1+y,1+x, "+");
						break;
				case '3':
						mvwprintw(game, 1+y,1+x, " " );
						break;
				case 'G':
					mvwprintw(game, 1+y,1+x, " ");
					break;
				case 'h':
					mvwprintw(game, 1+y,1+x, "O");
					break;
				case 'b':
					mvwprintw(game, 1+y,1+x, "o");
					break;
				}

		}

	}
	wrefresh(game);
}
void proccesInput(WINDOW* game, Snake& snake, int input)
{
	int d = snake.get_direction();
	switch(input)
	{
		case KEY_UP:
			if(d == 2) {
				snake.set_exit(true);
				break;}
			if(d!=0 && !(d==2 && snake.get_points() > 0))
			snake.set_direction(0);
			break;
		case KEY_DOWN:
			if(d == 0) {
				snake.set_exit(true);
				break;}
			if(d!=2 && !(d==0 && snake.get_points() > 0))
			snake.set_direction(2);
			break;
		case KEY_LEFT:
			if(d == 1) {
				snake.set_exit(true);
				break;}
			if(d!=3 && !(d==1 && snake.get_points() > 0))
			snake.set_direction(3);
			break;
		case KEY_RIGHT:
			if(d == 3) {
				snake.set_exit(true);
				break;}
			if(d!=1 && !(d==3 && snake.get_points() > 0))
			snake.set_direction(1);
			break;
		case 'Q':
		case 'q':
			snake.set_exit(true);
			break;
		case 'P':
		case 'p':
			char c;
			wattron(game, A_BOLD);
			mvwprintw(game, snake.get_height()/2, snake.get_width()/2, "PAUSE");
			wattroff(game, A_BOLD);
			nodelay(game, FALSE);
			do{
				c = wgetch(game);
			}while(c!='p' && c!='P');
			nodelay(game, TRUE);
			break;
	}
}
