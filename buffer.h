#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <vector>

std::vector<std::pair<int, int> > selections;
//start, len

class Buffer {
	std::string text;
public:
	Buffer(){cursor = 0;}

	int cursor;//index of the cursor
	Buffer(const std::string &_text);
	void loadText(const std::string &s);
	size_t getTextSize() const;
	size_t getCursor() const;
	void writeOnCursor(const std::string &str);
	void deleteLine();
	void findAndSelect(const std::string &str) const;
	std::string getText(const int &from, const int &to) const;


	void testOutput() {
		std::cout << text << std::endl;
	}
};

size_t Buffer::getCursor() const {
	return this->cursor;
}

size_t Buffer::getTextSize() const {
	return this->text.size();
}

void Buffer::writeOnCursor(const std::string &str) {
	//std::cout << this->text.size() << std::endl;
	this->text.insert(this->cursor, str);
	this->cursor += str.size();
	//std::cout << this->text.size() << std::endl;
}

void Buffer::deleteLine() {
	int start = this->cursor, end = this->cursor;
	while(start >= 0 && this->text[start] != '\n')
		start --;
	while(end < this->text.size() && this->text[end] != '\n')
		end ++;
	this->text.erase(start, end - start);
}

void Buffer::findAndSelect(const std::string &str) const {
	std::vector<int> curr = rabinKarp(str, text);
	for(int i = 0; i < curr.size(); i ++)
		selections.push_back({curr[i], str.size()});
}

Buffer::Buffer(const std::string &_text) {
	text = _text;
	cursor = 0;
}

void Buffer::loadText(const std::string &s) {
	this->text = s;
}

std::string Buffer::getText(const int &from, const int &to) const {
	if(to >= this->text.size())
		return this->text.substr(from, this->text.size() - from);
	return this->text.substr(from, to - from);
}

#endif
