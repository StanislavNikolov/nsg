#ifndef KEYBOARD_H
#define KEYBOARD_H

class Keyboard {
	bool keyArray[256];

	public:
	Keyboard();
	void readKeyboard();
	bool isKeyPressed(int i) const;
#ifdef __linux__
	void enableRawMode();
	void disableRawMode();
#endif
};

#ifdef __linux__
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void Keyboard::enableRawMode(){
	struct termios raw = orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); // TODO check if -1
}
void Keyboard::disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
	// TODO check if -1
}
#endif

Keyboard::Keyboard() {
	std::fill(keyArray, keyArray + 256, false);
#ifdef __linux__
	enableRawMode();
#endif
}

#include <bitset>

void Keyboard::readKeyboard() {
#ifdef __linux__

#define CTRL_KEY(k)  (((k) & 0b01100000) == 0)

	unsigned char c = '\0';
	std::fill(keyArray, keyArray + 256, false);

	while(read(STDIN_FILENO, &c, 1) == 1) {
		if(c == '\0') break;

		//debug::log(std::bitset<8>(c).to_string() + "  ");
		//debug::log(std::to_string(c) + "\n");
		if(CTRL_KEY(c)) {
			keyArray[17] = true; // WIN32 GetKeyState compatible
			c |= 0b01100000;
			keyArray[c] = true;
			continue;
		}

		// todo get alt
		/*

		if(c >= 'A' and c <= 'Z') {
			keyArray[c] = true;
			keyArray[16] = true;
			continue;
		}
		if(c >= 'a' and c <= 'z') {
			keyArray[c - 'a' + 'A'] = true;
			keyArray[16] = false;
			continue;
		}
		*/

		keyArray[c] = true;
	}

#elif _WIN32
	bool shift = (GetKeyState(16) >> 8) != 0;
	keyArray[16] = true;
	for(int i = 32;i <= 126;i ++) {
		if((GetKeyState(i) >> 8) != 0) {
			if('0' <= i and i <= '9') {
				if(shift) keyArray[i - '0' + '!'] = true;
				else	  keyArray[i] = true;
			}
			if('A' <= i and i <= 'Z') {
				if(shift) keyArray[i] = true;
				else	  keyArray[i - 'A' + 'a'] = true;
			}
			if('[' <= i and i <= ']') {
				if(shift) keyArray[i - '[' + '{'] = true;
				else	  keyArray[i] = true;
			}
			if(i == ';') {
				if(shift) keyArray[':'] = true;
				else	  keyArray[';'] = false;
			}
			if(i == '-') {
				if(shift) keyArray['_'] = true;
				else	  keyArray['-'] = false;
			}
			if(i == '=') {
				if(shift) keyArray['+'] = true;
				else	  keyArray['='] = false;
			}
			if(i == '\\') {
				if(shift) keyArray['|'] = true;
				else	  keyArray['\\'] = false;
			}
			if(i == '\'') {
				if(shift) keyArray['\''] = true;
				else	  keyArray['\"'] = false;
			}
			if(i == '`') {
				if(shift) keyArray['~'] = true;
				else	  keyArray['`'] = true;
			}
			if(i == '`') {
				if(shift) keyArray['~'] = true;
				else	  keyArray['`'] = true;
			}
			if(i == '/') {
				if(shift) keyArray['?'] = true;
				else	  keyArray['/'] = true;
			}
			if(i == ',') {
				if(shift) keyArray['<'] = true;
				else	  keyArray[','] = true;
			}
			if(i == '.') {
				if(shift) keyArray['<'] = true;
				else	  keyArray['.'] = true;
			}
			}
		}
	}
#endif
}


bool Keyboard::isKeyPressed(int i) const {
	return keyArray[i];
}

int translateKeyToCommand(const Keyboard &kb){
	bool ctrl = kb.isKeyPressed(17);//, alt = kb.isKeyPressed(18);
	if(ctrl && kb.isKeyPressed('x')) return 1;  ///CUT
	if(ctrl && kb.isKeyPressed('c')) return 2;  ///COPY
	if(ctrl && kb.isKeyPressed('v')) return 3;  ///PASTE
	if(ctrl && kb.isKeyPressed('s')) return 4;  ///SAVE
	if(ctrl && kb.isKeyPressed('S')) return 5;  ///SAVE_EVERYTHING
	if(ctrl && kb.isKeyPressed('z')) return 6;  ///BACK
	if(ctrl && kb.isKeyPressed('Z')) return 7;  ///FORWARD
	if(ctrl && kb.isKeyPressed('a')) return 8;  ///MARK_EVERYTHING
	if(ctrl && kb.isKeyPressed('f')) return 9;  ///FIND_WORD
	if(ctrl && kb.isKeyPressed('w')) return 10; ///DELETE
	//if(ctrl && kb.isKeyPressed('W')) return 11; ///DELETE_EVERYTHING
	if(ctrl && kb.isKeyPressed('q')) return 12; ///QUIT
	//if(ctrl && kb.isKeyPressed('C')) return 13; ///COMMENT
	//if(ctrl && kb.isKeyPressed('X')) return 14; ///UNCOMMENT

	for(int i = 32;i <= 127;i ++) {
		if(kb.isKeyPressed(i)) return 1000 + i;
	}

	return -1;
}

#endif

