
#ifndef RENDER_H
#define RENDER_H

#ifdef __linux__
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#include <cassert>

enum COLOR {BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE};
void changeColor(COLOR fg, COLOR bg) {
#ifdef __linux__
	int _fg = 0, _bg = 0;

	if(fg == BLACK) {_fg = 30;}
	if(fg == RED) {_fg = 31;}
	if(fg == GREEN) {_fg = 32;}
	if(fg == YELLOW) {_fg = 33;}
	if(fg == BLUE) {_fg = 34;}
	if(fg == MAGENTA) {_fg = 35;}
	if(fg == CYAN) {_fg = 36;}
	if(fg == WHITE) {_fg = 37;}

	if(bg == BLACK) {_bg = 40;}
	if(bg == RED) {_bg = 41;}
	if(bg == GREEN) {_bg = 42;}
	if(bg == YELLOW) {_bg = 43;}
	if(bg == BLUE) {_bg = 44;}
	if(bg == MAGENTA) {_bg = 45;}
	if(bg == CYAN) {_bg = 46;}
	if(bg == WHITE) {_bg = 47; }

	std::cout<<char(27) << "[" << _bg <<";" << _fg << "m";

#elif _WIN32
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	int _fg = 0, _bg = 0;
	if(fg == BLACK) {_fg = 0;}
	if(fg == RED) {_fg = FOREGROUND_RED;}
	if(fg == GREEN) {_fg = FOREGROUND_GREEN;}
	if(fg == YELLOW) {_fg = 14;}
	if(fg == BLUE) {_fg = FOREGROUND_BLUE;}
	if(fg == MAGENTA) {_fg = 13;}
	if(fg == CYAN) {_fg = 11;}
	if(fg == WHITE) {_fg = 15;}

	if(bg == BLACK) {_bg = 0;}
	if(bg == RED) {_bg = FOREGROUND_RED;}
	if(bg == GREEN) {_bg = FOREGROUND_GREEN;}
	if(bg == YELLOW) {_bg = 14;}
	if(bg == BLUE) {_bg = FOREGROUND_BLUE;}
	if(bg == MAGENTA) {_bg = 13;}
	if(bg == CYAN) {_bg = 11;}
	if(bg == WHITE) {_bg = 15;}
	SetConsoleTextAttribute(hStdout, _fg | _bg);

#endif
}

class UICommand {
};


void gotoxy(int x, int y) {
#ifdef __linux__
	std::cout << "\033[" << y << ";" << x << "H" << std::flush;
#elif _WIN32
	COORD pos = {x, y};
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
#endif
}

void flushToScreen(const std::string &screen, const std::string &colorBuff, int rows, int columns) {
	//std::cout << int(screen[0]) << std::flush; return;
	char currColor = '\0';
	int curRow = 0;
	int curCol = 0;

	if(lastScreen.size() != screen.size()) lastScreen = std::string(rows * columns, '\0');

	for(int i = 0;i < screen.size();i ++) {
			if(lastScreen[i] != screen[i]/* or colorBuff[i] != currColor*/) {
				gotoxy(curCol, curRow);
				/*if(colorBuff[i] != currColor) {
					currColor = colorBuff[i];
					if(colorBuff[i] == 's') changeColor(GREEN, BLACK);//STATUS
					if(colorBuff[i] == 'd') changeColor(WHITE, BLACK);//DEF
					if(colorBuff[i] == 'c') changeColor(BLACK, WHITE);//CURSOR
					if(colorBuff[i] == 'n') changeColor(BLACK, BLACK);//NOTHING
				}*/
				std::cout << screen[i];
#ifdef _WIN32
				std::cout << std::flush;
#endif
			}
			curCol ++;
			if(curCol >= columns) {
				curCol = 0;
				curRow ++;
			}
	}

	gotoxy(0, 0);
	std::cout << std::flush;
}

void drawScreen(const Tab &currTab) {
	int columns, rows;

#ifdef __linux__
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	columns = w.ws_col;
	rows = w.ws_row;
#elif _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#endif
	anythingNewToRedraw = anythingNewToRedraw || (columns != lastRenderColumns or rows != lastRenderRows);
	if(!anythingNewToRedraw) return;
	anythingNewToRedraw = false;
	lastRenderColumns = columns;
	lastRenderRows = rows;

	//std::cout << currTab.getText() << std::endl;
	//anythingNewToRedraw = false;
	//return;

	gotoxy(0, 0);
	std::string screen = "";
	std::string colorBuff = "";

	for(auto &tab : allTabs) {
		screen += tab.getName();
		screen += "|";
	}
	while(screen.size() > columns) screen.pop_back();
	while(screen.size() < columns) screen.push_back(' ');
	while(colorBuff.size() < screen.size()) colorBuff.push_back('s');

	std::string bufText = currTab.getText(); // todo move to class TAB{}
	bufText += ".";

	int currRow = 1;
	int currCol = 0;

	char color = 'd';
	int cursor = currTab.buff.getCursor();

	for(int bt = 0;bt < bufText.size();bt ++) {
		if(currRow >= rows) break;
		color = (bt == cursor ? 'c' : 'd');

		if(bufText[bt] == '\n') {
			for(;currCol < columns;currCol ++) {
				screen += ' ';
				colorBuff += color;
			}
			currCol = 0;
			currRow ++;
			continue;
		}
		if(bufText[bt] == '\t') {
			// TODO configure tab size
			int end = currCol + 4;
			for(;currCol < columns && currCol < end;currCol ++) {
				screen += ' ';
				colorBuff += color;
			}
			continue;
		}

		if(currCol < columns) {
			screen += bufText[bt];
			colorBuff += color;
			currCol ++;
		}
	}
	screen += std::string(rows * columns - screen.size(), ' ');
	colorBuff += std::string(screen.size() - colorBuff.size(), 'n');

	//assert(screen.size() == colorBuff.size())

	flushToScreen(screen, colorBuff, rows, columns);
	lastScreen = screen;
}

#endif
