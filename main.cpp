#include <iostream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>

#elif __linux__

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#endif

#include "debug.h"
#include "buffer.h"
#include "common.h"
#include "keyboard.h"
#include "render.h"

Keyboard kb;

void clearScreen() {
#ifdef __linux__
	std::system("clear");
#elif _WIN32
	std::system("cls");
#endif	
}

int main() {
	fpow[0] = 1;
	for(int i = 1;i < 130;i ++) fpow[i] = (fpow[i-1] * 37) % mod;
	//changeColor();
	allTabs.push_back(Tab("kur.cpp"));
	allTabs.push_back(Tab("asd.txt"));
	allTabs.push_back(Tab("mnogodulgoimeasdasdasdasd.txt"));
	clearScreen();
	while(true) {
		drawScreen(allTabs[0]);
		kb.readKeyboard();
		/*
		for(int i = 0;i < 256;i ++) {
			if(kb.isKeyPressed(i) != 0) std::cout << i << '\t' << std::flush;
		}
		*/
		int currCommand = translateKeyToCommand(kb);
		if(currCommand == 12) {
			#ifdef __linux__
			kb.disableRawMode();
			#endif
			return 0;
		}
		if(currCommand > 1000) {
			allTabs[0].buff.writeOnCursor(std::string(1, (char)(currCommand - 1000)));
			anythingNewToRedraw = true;
		}
		goodSleep(1000/30);
	}
}

