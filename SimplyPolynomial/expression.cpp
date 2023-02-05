#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include <mutex>
#include<algorithm>

#include "expression.h"


using namespace std;

Expression::Expression(int arr[MAX_TERMS]) {
	for (int i = 0; i < MAX_TERMS; i++) {
		coef[i] = arr[i];
		if (0 != arr[i]) {
			this->terms++;
		}
	}
}

Expression::Expression(int terms, string str) {
	this->terms = terms;
	vector<int> tmp;
	StringToVector(str, tmp);
	this->output = tmp;
}

Expression::Expression(int terms, vector<int> output) {
	this->terms = terms;
	this->output = output;
}

void Expression::PrintExpression() {
	if (!this->CheckComplete()) {
		ExceptionHandling(EXPERSSION_NOT_COMPLETE);
		return;
	}
	string buff = "";
	for (int i = MAX_TERMS-1; i >= 0 ; i--) {
		auto val = coef[i];
		if (0 == val) {
			continue;
		}
		if (buff != "" && val > 0 ) {
			buff += "+";
		}
		//if coefficient == 1, do not print it.
		if (abs(val) != 1 || 0 == i) {
			buff += to_string(val);
		}
		if (0 == i) {
			break;
		}
		buff += "x";
		if (i != 1) {
			buff += "^" + to_string(i);
		}
	}
	cout << buff << endl;
}

string Expression::PrintOutPut() {
	string str = "";
	if (0 == output.size()) {
		throw OUTPUT_IS_EMPTY;
		return str;
	}
	for (auto it = output.begin(); it != output.end(); it++) {
		if (it != output.begin()) {
			cout << ", ";
			str += ", ";
		}
		cout << *it;
		str += *it;
	}
	return str;
}

void Expression::WriteOutPut() {
	if (!WriteDownOutput(this->output)) {
		throw WRITE_FILE_ERROR;
	}
}

void Expression::OutPut(int start, int end) {
	vector<int> tmp;
	for (auto x = start; x <= end; x++) {
		tmp.push_back(CalcExpression(coef, x));
	}
	this->output = tmp;
}

void Expression::CreateOutPutSeq() {
	auto start = 0, end = 0;
	cout << "Please enter a range of x to check." << endl << "Start num: ";
	cin >> start;
	cout << endl << "End num: ";
	cin >> end;
	if (start >= end) {
		throw INPUT_ERROR;
		return;
	}
	this->OutPut(start, end);
	try {
		this->PrintOutPut();
	}
	catch (int err) {
		throw err;
	}
	cin.get();
}

//Test that the expression is correct.
//@param sample - Testing expression
bool Expression::tryVerify(int sample[MAX_TERMS]) {
	auto res = [=] {
		for (auto i = 0; i < output.size() ; i++) {
			int res = CalcExpression(sample, i);
			if (output[i] !=  res){
				return false;
			}
		}
		return true;
	}();
	if (res) {
		for (int i = 0; i < MAX_TERMS; i++) {
			this->coef[i] = sample[i];
		}
	}
	return res;
}

//find out the coefficients of a polynomial with unknown terms.
//@param sample - Coefficients already found
//@param terms_num - The term of the coefficient for this round of searching
//@param index - 
bool Expression::findOutCoefs(int sample[MAX_TERMS], int terms_num, int index) {
	if (MAX_POWER == index || MAX_TERMS-1 == terms_num) {
		if (0 == terms_num) {
			return false;
		}
		return this->tryVerify(sample);
	}
	//deep copy
	int tmp[MAX_TERMS] = {};
	for (int i = 0; i <= index; i++) {
		tmp[i] = sample[i];
	}
	index++;
	for (int a = MIN_COEF; a <= MAX_COEF; a++) {
		tmp[index] = a;
		int find_terms = 0;
		if (0 != a) {
			if (findOutCoefs(tmp, terms_num + 1, index)) {
				find_terms = terms_num + 1;
			}
		}
		else {
			if (findOutCoefs(tmp, terms_num, index)) {
				find_terms = terms_num;
			}
		}
		if (find_terms != 0) {
			if (0 == terms) {
				terms = find_terms;
			}
			return true;
		}
	}
	return false;
}

//find out the coefficients of a polynomial with known terms.
//@param need_coefs - number of coefficients required
//@param index - Index of coefficients for this round of searches
//@param sample - Coefficients already found
bool Expression::findOutCoefs(int need_coefs, int index,  int sample[MAX_TERMS] ) {
	//left coef not enough
	if (need_coefs > (MAX_TERMS - 1) - index) {
		return false;
	}
	if (0 == need_coefs || MAX_POWER == index) {
		return this->tryVerify(sample);
	}
	int tmp[MAX_TERMS] = {};
	for (int i = 0; i <= index; i++) {
		tmp[i] = sample[i];
	}
	index++;
	for (int a = MIN_COEF; a <= MAX_COEF; a++) {
		tmp[index] = a;
		if (0 != a) {
			if (true == findOutCoefs(need_coefs - 1, index, tmp)) {
				return true;
			}
		}
		else {
			if (true == findOutCoefs(need_coefs, index, tmp)) {
				return true;
			}
		}
	}
}

void Expression::FindOutCoefs() {
	//if terms == 0, it's unknown terms.
	if (0 == output.size()) {
		throw 0;
		return;
	}
	auto tmp = terms;
	if (MAX_TERMS <= tmp) {
		throw 1;
		return;
	}
	//default first parameter is 0.
	int sample[MAX_TERMS]{output[0]};
	if (0 != output[0]) {
		if (0 != terms) {
			tmp--;
			if (0 == tmp) {
				throw 1;
				return;
			}
		}
		else {
			tmp++;
		}
	}
	bool ret = false;
	if (0 == terms) {
		this->findOutCoefs(sample, tmp, 0 );
	}
	else {
		this->findOutCoefs(tmp, 0 , sample);
	}
}

void Expression::Solve() {
	try {
		this->FindOutCoefs();
	}
	catch (int ret) {
		switch (ret) {
		case 0:
			cout << "expression has no output." << endl;
			return;
		case 1:
			cout << "expression terms error, terms: " << terms << endl;
			return;
		default:
			cout << "ret number error, num: " << ret << endl;
			return;
		}
	}
}

void Expression::SetCoef(int i, int a) {
	if (i >= MAX_TERMS || i < 0) {
		return;
	}
	this->coef[i] = a;
}

bool Expression::CheckComplete() {
	return terms > 0 ? true : false;
}

void Expression::OperateExpression() {
	int input;
	while (true) {
		cout << "1. Make output." << endl << "2. Print output." << endl << "3. Write output to file." << endl << "0. Return to main menu." << endl;
		cin >> input;
		try {
			switch (input) {
			case 1:
				CreateOutPutSeq();
				break;
			case 2:
				PrintOutPut();
				break;
			case 3:
				WriteOutPut();
				break;
			case 0:
				return;
			}
		}
		catch (int err) {
			ExceptionHandling(err);
		}
		cin.get();
	}
}

void OneTermExpression::Solve() {
	auto len = output.size();
	if (len < SIMPLE_TERM_MIN_OUTPUT) {
		ExceptionHandling(SOLVE_EXAMPLE_ERROR);
		return;
	}
	if (output[0] != 0) {
		ExceptionHandling(SOLVE_EXAMPLE_ERROR);
		return;
	}
	auto a = output[1];
	auto n = CalcPower(output[2] / a, 2);
	if (n > MAX_POWER){
		throw 2;
		return;
	}
	this->coef[n] = a;
}

void TwoTermExpression::Solve() {
	auto len = output.size();
	if (len < SIMPLE_TERM_MIN_OUTPUT) {
		ExceptionHandling(SOLVE_EXAMPLE_ERROR);
		return;
	}
	int a, n;
	auto k = output[0];
	if (0 != k) {
		a = output[1] - k, n = CalcPower((output[2] - k) / a, 2);
		this->SetCoef(0, k);
		this->SetCoef(n, a);
	}
	else {
		Expression::Solve();
	}
}


int CalcExpression(int coef[MAX_TERMS], int x) {
	auto ret = coef[0]; //k-value
	for (int i = 1; i <= MAX_POWER; i++) {
		ret += coef[i] * pow(x, i);
	}
	return ret;
}

void InputExpression(int coef[MAX_TERMS] ) {
	int input = 0;
	while (true)
	{
		bool illegal = false;
		cout << "Please input coefficients, The order is from zero to the fourth power term." << endl;
		for (int i = 0; i < MAX_TERMS; i++) {
			cin >> input;
			if (MAX_COEF < input || MIN_COEF > input) {
				cout << "The number you have entered is illegal, please enter a number between -9 and 9." << endl;;
				illegal = true;
				break;
			}
			coef[i] = input;
		}
		if (illegal) {
			continue;
		}
		break;
	}
}

void MakeExpression() {
	system("cls");
	int coef[MAX_TERMS];
	InputExpression(coef);
	system("cls");
	Expression exp(coef);
	exp.PrintExpression();
	exp.OperateExpression();
}

void ShowExpressions(vector<Expression> exps) {
	if (0 == exps.size()) {
		return;
	}
	cout << "show expressions:" << endl;
	for (int i = 0; i < exps.size(); i++) {
		auto exp = exps[i];
		exp.FindOutCoefs();
		cout << i << ": ";
		exp.PrintExpression();
	}
}

void ReadExpression() {
	vector<string> outputs;
	if (!ReadOutputFile(outputs)) {
		cout << "read file error" << endl;
		return;
	}
	if (0 == outputs.size()) {
		return;
	}
	vector<Expression> exps;
	for (auto it : outputs) {
		exps.push_back(Expression(0, it));
	}
	ShowExpressions(exps);
}