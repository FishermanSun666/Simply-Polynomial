#include<iostream>
#include<string>
#include<stdio.h>
#include<thread>
#include<map>

#include"expression.h"

using namespace std;

map<char, Expression> g_modules;
map<char, vector<int>> g_output;

void InitExamples() {
	g_output['A'] = { 0, 3, 24, 81, 192, 375, 648, 1029, 1536, 2187, 3000, 3993, 5184, 6591, 8232, 10125, 12288, 14739, 17496, 20577, 24000 };
	g_output['B'] = { -9, -4, 71, 396, 1271, 3116, 6471, 11996, 20471, 32796, 49991, 73196, 103671, 142796, 192071, 253116, 327671, 417596, 524871, 651596, 799991 };
	g_output['C'] = { 0, 14, 96, 300, 680, 1290, 2184, 3416, 5040, 7110, 9680, 12804, 16536, 20930, 26040, 31920, 38624, 46206, 54720, 64220, 74760 };
	g_output['D'] = { 7, 13, 17, 13, -5, -43, -107, -203, -337, -515, -743, -1027, -1373, -1787, -2275, -2843, -3497, -4243, -5087, -6035, -7093 };
	g_output['E'] = { 114, 110, 100, 78, 38, -26, -120, -250, -422, -642, -916, -1250, -1650, -2122, -2672, -3306, -4030, -4850, -5772, -6802, -7946 };
	g_output['F'] = { 715, 625, 523, 409, 283, 145, -5, -167, -341, -527, -725, -935, -1157 };
}

void PrintExamples(map<char, Expression> modules) {
	if (0 == modules.size()) {
		return;
	}
	for (auto it = modules.begin(); it != modules.end(); it++) {
		cout << it->first << ": ";
		it->second.PrintExpression();
		cout << endl;
	}
}

void SolveExample(char key, int terms) {
	if (0 == g_output.count(key)) {
		ExceptionHandling(SOLVE_EXAMPLE_ERROR);
		return;
	}
	switch (terms) {
	case 1:
	{
		OneTermExpression module_exp(1, g_output[key]);
		module_exp.Solve();
		g_modules[key] = module_exp;
		break;
	}
	case 2:
	{
		TwoTermExpression module_exp(2, g_output[key]);
		module_exp.Solve();
		g_modules[key] = module_exp;
		break;
	}
	default:
	{
		Expression module_exp(terms, g_output[key]);
		module_exp.Solve();
		g_modules[key] = module_exp;
	}
	}
}

void AddExampleFuncToThread(vector<thread> &pool) {
	pool.push_back(thread(&SolveExample, 'A', 1));
	pool.push_back(thread(&SolveExample, 'B', 2));
	pool.push_back(thread(&SolveExample, 'C', 3));
	pool.push_back(thread(&SolveExample, 'D', 4));
	pool.push_back(thread(&SolveExample, 'E', 0));
	pool.push_back(thread(&SolveExample, 'F', 0));
}

void WaitForThreadsJoin(vector<thread> &pool) {
	if (0 == pool.size()) {
		return;
	}
	for (int i = 0; i < pool.size(); i++) {
		pool[i].join();
	}
}

void SolveExamples() {
	if (0 != g_modules.size()) {
		PrintExamples(g_modules);
		return;
	}
	vector<thread> td_pool;
	AddExampleFuncToThread(td_pool);
	//thread join
	WaitForThreadsJoin(td_pool);
	PrintExamples(g_modules);
}

void Operation(int input) {
	try {
		switch (input) {
			return;
		case 1:
			SolveExamples();
			break;
		case 2:
			MakeExpression();
			break;
		case 3:
			ReadExpression();
			break;
		default:
			throw PROCESS_ERROR;
		}
	}
	catch (int err) {
		ExceptionHandling(err);
	}
}

void main() {
	InitExamples();
	auto input = 0;
	while (true) {
		system("cls");
		cout << "1. Print model expression." << endl << "2. Make expression" << endl << "3. Read file" << endl << "0. Exit." << endl;
		cin >> input;
		if (0 == input) {
			return;
		}
		Operation(input);
		system("pause");
	}
}