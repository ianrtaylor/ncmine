#include <ncurses.h>
#include <string.h>
#include "display.h"
#include "mine.h"

WINDOW *board_win;
WINDOW *info_win;
static int prev_y, prev_x;

void reset_cur(void)
{
	move(prev_y, prev_x);
}

void set_cur(void)
{
	getyx(stdscr, prev_y, prev_x);
}

void move_cur_center(void)
{
	getbegyx(board_win, prev_y, prev_x);
	prev_y++;
	prev_x++;
	reset_cur();
}

void mvwincenter(WINDOW *win);
int startd(int size)
{
	char *title = "MINESWEEPER";
	char *sub   = "by Ian Taylor";
	char *start = "press any key to start";
	int row, col;

	// set some settings
	initscr();
	cbreak();
	keypad(stdscr, true);
	noecho();

	// start screen:
	getmaxyx(stdscr, row, col);
	attron(A_BOLD);
	mvprintw(row/2-1, (col-strlen(title))/2, "%s", title);
	attroff(A_BOLD);
	mvprintw(row/2,   (col-strlen(title))/2, "%s", sub);
	mvprintw(row*2/3, (col-strlen(start))/2, "%s", start);
	refresh();
	getch();

	// set up windows:
	clear();
	refresh();
	// set proper dimensions, with room for the boarder.
	board_win = newwin(2 + size, 1 + (size * 2), 0, 0);
	mvwincenter(board_win);
	box(board_win, 0 , 0);

	move_cur_center();

	return 0;
}

void mvwincenter(WINDOW *win)
{
	int max_x, max_y, win_x, win_y;
	getmaxyx(stdscr, max_x, max_y);
	getmaxyx(win, win_x, win_y);

	mvwin(board_win, max_x/2 - win_x/2, max_y/2 - win_y/2); 
}

int exitd(void)
{
	delwin(board_win);
	endwin();
	return 0;
}

char getminech(Mine* mine)
{
	if (mine->visible) {
		if (is_mine(mine)) {
			return '@';
		} else {
			return mine->adj_mines + (int) '0';
		}
	} else {
		return ' ';
	}
}

void printd(int size, Mine **board)
{
	// reset window position to center
	clear();
	mvwincenter(board_win);
	refresh();

	mvwprintw(board_win, 0, 0, "\n");
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			wprintw(board_win, " %c", getminech(&board[x][y]));
		}
		wprintw(board_win, "\n");
	}

	// draw boarders
	box(board_win, 0, 0);
	wrefresh(board_win);
}

static void offsetcur(int offy, int offx)
{
	int x, y;
	getyx(stdscr, y, x);
	move(y + offy, x + offx);
}

static void get_bounds(WINDOW *win, int *y1, int *x1, int *y2, int *x2)
{
	// top left corner of the window:
	int ybeg, xbeg;
	getbegyx(win, ybeg, xbeg);
	// bottom right corner:
	int yend, xend;
	getmaxyx(win, yend, xend);
	yend += ybeg; xend += xbeg; // getmaxyx returns size of win, not coordinates
	*y1 = ybeg; *x1 = xbeg;
	*y2 = yend; *x2 = xend;
}

/**
 * assumes that window has a border that may not be overwritten
 */
static bool is_in_win(WINDOW *win, int y, int x)
{
	// top left corner of the window:
	int ybeg, xbeg, yend, xend;
	get_bounds(win, &ybeg, &xbeg, &yend, &xend);

	return (y > ybeg && x > xbeg) && (y < yend - 1 && x < xend - 1);
}

void move_board_cur(int offy, int offx)
{
	int ycur, xcur;
	getyx(stdscr, ycur, xcur);
	ycur += offy;
	xcur += offx;
	if (is_in_win(board_win, ycur, xcur))
		offsetcur(offy, offx);
}

/**
 * @return: TODO
 */
void get_board_yx(int *y, int *x)
{
	int ycur, xcur;
	getyx(stdscr, ycur, xcur);
	int ywin, xwin;
	getbegyx(board_win, ywin, xwin);
	
	*y = ycur - ywin - 1;
	*x = (xcur - xwin - 1)/2;
}
	

void getin(int *x, int *y)
{
	reset_cur();
	int in;
	do {
		in = getch();
		offsetcur(0, 0);
		switch(in) {
		case KEY_LEFT:
			move_board_cur(0, -2);
			break;
		case KEY_RIGHT:
			move_board_cur(0, 2);
			break;
		case KEY_UP:
			move_board_cur(-1, 0);
			break;
		case KEY_DOWN:
			move_board_cur(1, 0);
			break;
		}

	} while (in != '\n' && in != '\r');
	get_board_yx(y, x);
	set_cur();
}
