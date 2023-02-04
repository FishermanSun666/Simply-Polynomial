#include<iostream>
#include<string>
#include<stdio.h>
#include"tools.h"
#include"expression.h"
#include"file.h"


using std::cin;
using std::cout;
using std::string;

void MakeExpression() {
	system("cls");
	auto input = 0;
	int coef[NUM_TERMS];
	cout << "Please input coefficients, The order is from zero to the fourth power term." << endl;
	for (int i = 0; i < NUM_TERMS; i++) {
		cin >> input;
		coef[i] = input;
	}
	system("cls");
	Expression exp(coef);
	exp.PrintExpression();
	while (true) {
		cout << "1. Make output." << endl << "2. Print output." << endl << "3. Write output to file." << endl << "0. Return to main menu." << endl;
		cin >> input;
		try {
			switch (input) {
			case 1:
				exp.MakeOutPut();
				break;
			case 2:
				exp.PrintOutPut();
				break;
			case 3:
				exp.WriteOutPut();
				break;
			case 0:
				return;
			}
		}
		catch (int ret) {
			cout << "Input error, please re-enter." << endl;
		}
		cin.get();
	}	
}

void ReadExpression() {
	vector<string> outputs;
	try {
		ReadFile(outputs);
	}
	catch (bool ret){
		cout << "read file error" << endl;
		return;
	}
	if (0 == outputs.size()) {
		return;
	}
	vector<Expression> exps;
	for (auto it = outputs.begin(); it != outputs.end(); it++) {
		exps.push_back(Expression(0, *it));
	}
	cout << "show expressions:" << endl;
	for (int i = 0; i < exps.size(); i++) {
		auto exp = exps[i];
		exp.FindOut();
		cout << i << ": ";
		exp.PrintExpression();
	}
}

void main() {
	InitModules();
	auto input = 0;
	while (true) {
		system("cls");
		cout << "1. Print model expression." << endl << "2. Make expression" << endl << "3. Read file" << endl << "0. Exit." << endl;
		cin >> input;
		try {
			switch (input) {
			case 0:
				return;
			case 1:
				SolveModules();
				break;
			case 2:
				MakeExpression();
				break;
			case 3:
				ReadExpression();
				break;
			default:
				cout << "Wrong input, please re-enter" << endl;
			}
		}
		catch (int ret){
			switch (ret) {
			case 0:
				cout << "len error" << endl;
				break;
			case 1:
				cout << "k value error" << endl;
				break;
			case 2:
				cout << "power error" << endl;
			case 3:
				cout << "expression print error" << endl;
			default:
				cout << "catch value error: " << ret << endl;
			}
		}
		system("pause");
	}
}