#ifndef COMMON_H
#define COMMON_H

#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#include <conio.h>
#endif

#include <fstream>

void goodSleep(unsigned int ms) {
#ifdef __linux__
	usleep(ms * 1000);
#elif _WIN32
	Sleep(ms);
#endif
}

class Tab{
private:
	std::string name;
	std::fstream stream;
public:
	Buffer buff;
	std::string getText(const int &from, const int &to) const;
	std::string getText() const;
	std::string getName() const;
	void createNewFile(std::string n);
	Tab(const std::string &name);
	void saveCurr();
};

std::string Tab::getName() const{
	return name;
}
std::string Tab::getText(const int &from, const int &to) const{
	return buff.getText(from, to);
}
std::string Tab::getText() const{
	return this->buff.getText(0, this->buff.getTextSize());
}

void Tab::createNewFile(std::string n) {
	stream.open(name, std::ios::out);
}
Tab::Tab(const std::string &name) {
	this->name = name;
	stream.open(name, std::ios::in);
	if (stream.tellg() != -1) {
		std::string text(std::istreambuf_iterator<char>(stream), {} );
		this->buff.loadText(text);
	}else{
		createNewFile(name);
	}
}

void Tab::saveCurr(){
	std::string txt = buff.getText(0, buff.getTextSize());
	stream.close();
	stream.open(name, std::ios::out);
	stream << txt;
	stream.close();
}


std::vector<Tab> allTabs;

bool anythingNewToRedraw = true;
int lastRenderColumns = 0;
int lastRenderRows = 0;

#endif

