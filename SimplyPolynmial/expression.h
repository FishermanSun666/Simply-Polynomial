#pragma once
#include<string>
#include<vector>
#include <mutex>

using namespace std;

const enum {
	SIMPLE_TERM_MIN_OUTPUT = 3,
	MAX_POWER = 4,
	NUM_TERMS = 5,
	MAX_COEF = 9,
	MIN_COEF = -9,
};

void InitModules();

class Expression {
	friend class OneTermExpression;
	friend class TwoTermExpression;
private:
	int terms;
	int coef[NUM_TERMS] = {};
	vector<int> output;
protected:
	void setCoef(int i, int a);
	bool test(int sample[NUM_TERMS]);
	bool findOutCoefs(int sample[NUM_TERMS], int coef_num, int index);
	bool findOutCoefs(int index, int need_coefs, int sample[NUM_TERMS]);
public:
	Expression() {}
	Expression(int arr[MAX_POWER]);
	Expression(int terms, string str);
	Expression(int terms, vector<int> output);
	~Expression() {}
	void PrintExpression();
	string PrintOutPut();
	void WriteOutPut();
	void MakeOutPut();
	void Calculate(int start, int end);
	void FindOut();
	bool FinishFindout();
	//void SolveModule(char key, int terms);
	virtual void Solve();
};

class OneTermExpression :public Expression {
public:
	OneTermExpression() {}
	OneTermExpression(int arr[MAX_POWER]) : Expression(arr) {}
	OneTermExpression(int terms, string str) : Expression(terms, str) {}
	OneTermExpression(int terms, vector<int> output) : Expression(terms, output) {}
	void Solve();
};

class TwoTermExpression :public Expression {
public:
	TwoTermExpression() {}
	TwoTermExpression(int arr[MAX_POWER]) : Expression(arr) {}
	TwoTermExpression(int terms, string str) : Expression(terms, str) {}
	TwoTermExpression(int terms, vector<int> output) : Expression(terms, output) {}
	void Solve();
};

int CalcExpression(int coef[NUM_TERMS], int x);
void SolveModules();