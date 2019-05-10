#ifndef STRINGMATHCING_H

#include <iostream>
#include <bitset>
#include <vector>
#include <cstring>

std::vector<int> findText(const std::string &pattern, const std::string &text) {
    int lps[20000];
    unsigned int i = 1, j, currLength = 0;
    lps[0] = 0;
    while(i < pattern.size()) {
        if(pattern[i] == pattern[currLength]) {
            lps[i ++] = ++ currLength;
        } else {
            if(currLength == 0) {
                lps[i ++] = 0;
            } else {
                currLength = lps[currLength - 1];
            }
        }
    }
    std::vector<int> ans;
    i = 0; j = 0;
    while(j < text.size()) {
        if(pattern[i] == text[j]) {
            i ++; j ++;
        }
        if(i == pattern.size()) {
            ans.push_back(j - i);
            i = lps[i - 1];
        } else if(j < text.size() && pattern[i] != text[j]) {
            if(i == 0) {
                j ++;
            } else {
                i = lps[i - 1];
            }
        }
    }
    return ans;
}

#endif // STRINGMATHCING_H
