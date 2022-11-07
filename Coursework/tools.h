#pragma once
#include<string>
#include<vector>
#include<thread>

using namespace std;

int Len(int* arr[]);

void StringClearChar(string& str, const char tar);

void StringToVector(string str, vector<int> &numbers);

int CalcPower(int num, int x);

void JoinThreads(vector<thread> threads);