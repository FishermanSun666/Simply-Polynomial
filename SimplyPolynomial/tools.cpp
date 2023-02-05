#include<iostream>
#include<string>
#include<sstream>
#include<iterator>
#include<vector>
#include<thread>
#include<cmath>
#include"tools.h"

using namespace std;

void StringClearChar(string& str, const char tar){
    string::iterator it;
    for (it = str.begin(); it != str.end(); it++)
    {
        if (*it == tar)
        {
            str.erase(it);
            if (it != str.begin()) {
                it--;
            }
        }
    }
}

void StringToVector(string str, vector<int> &numbers) {
    StringClearChar(str, ' ');
    string tmp = "";
    for (auto pos = str.find(','); ; pos = str.find(',')) {
        tmp = str.substr(0, pos);
        numbers.push_back(stoi(tmp));
        str = str.substr(pos+1);
        if (tmp.size() == str.size()) {
            break;
        }
    }
    cout << endl;
}

int CalcPower(int num, int x) {
    if (num < x) {
        return 0;
    }
    if (num % x != 0) {
        return 0;
    }
    int i = 0;
    while (true) {
        num /= x;
        i++;
        if (1 == num) {
            return i;
        }
    }
}
