#pragma once
#include<string>
#include<vector>
#include <mutex>

#include "tools.h"
#include "file.h"
#include "error_code.h"

using namespace std;

const enum {
	SIMPLE_TERM_MIN_OUTPUT = 3,
	MAX_POWER = 4, 
	MAX_TERMS = 5,
	MAX_COEF = 9,
	MIN_COEF = -9,
};

void InitExamples();

class Expression {
	friend class OneTermExpression;
	friend class TwoTermExpression;
private:
	int terms;
	int coef[MAX_TERMS] = {};
	vector<int> output;
protected:
	void SetCoef(int i, int a);
	bool tryVerify(int sample[MAX_TERMS]);
	bool findOutCoefs(int sample[MAX_TERMS], int coef_num, int index);
	bool findOutCoefs(int index, int need_coefs, int sample[MAX_TERMS]);
public:
	Expression() {}
	Expression(int arr[MAX_TERMS]);
	Expression(int terms, string str);
	Expression(int terms, vector<int> output);
	~Expression() {}
	void PrintExpression();
	string PrintOutPut();
	void WriteOutPut();
	void CreateOutPutSeq();
	void OutPut(int start, int end);
	void FindOutCoefs();
	void OperateExpression();
	bool CheckComplete();
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

int CalcExpression(int coef[MAX_TERMS], int x);

void SolveExamples();

void MakeExpression();

void ReadExpression();