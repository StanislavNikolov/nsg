#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <vector>

std::vector<std::pair<int, int> > selections;
//start, len

long long fpow[140];

const long long modAns = 1e9 + 7;
const long long mod = 1000000000000037;
std::vector<int> rabinKarp(const std::string &pat, const std::string &text)
{
	std::vector<int> indexes;
	if(pat.size() > text.size())
		return indexes;
	long long hash = 0;
	long long hashText = 0;
	long long smallMod = 37;
	long long power = fpow[pat.size() - 1];
	for(int i = 0; i < pat.size(); i ++)
	{
		hash = (hash * smallMod + pat[i]) % mod;
		hashText = (hashText * smallMod + text[i]) % mod;
	}
	for(int i = 0; i < text.size() - pat.size(); i ++)
	{
		if(hash == hashText)
			indexes.push_back(i);
		hashText = (smallMod * (hashText - text[i] * power) + text[pat.size() + i]) % mod;
		if(hashText < 0)
			hashText += mod;
	}
	if(hash == hashText)
		indexes.push_back(text.size() - pat.size());
	return indexes;
}

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
