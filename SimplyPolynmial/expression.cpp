#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<cmath>
#include<thread>
#include <mutex>
#include<algorithm>
#include"tools.h"
#include"expression.h"
#include"file.h"


using namespace std;

map<char,Expression> g_modules;
map<char, vector<int>> g_output;

void InitModules() {
	g_output['A'] = { 0, 3, 24, 81, 192, 375, 648, 1029, 1536, 2187, 3000, 3993, 5184, 6591, 8232, 10125, 12288, 14739, 17496, 20577, 24000 };
	g_output['B'] = { -9, -4, 71, 396, 1271, 3116, 6471, 11996, 20471, 32796, 49991, 73196, 103671, 142796, 192071, 253116, 327671, 417596, 524871, 651596, 799991 };
	g_output['C'] = { 0, 14, 96, 300, 680, 1290, 2184, 3416, 5040, 7110, 9680, 12804, 16536, 20930, 26040, 31920, 38624, 46206, 54720, 64220, 74760 };
	g_output['D'] = { 7, 13, 17, 13, -5, -43, -107, -203, -337, -515, -743, -1027, -1373, -1787, -2275, -2843, -3497, -4243, -5087, -6035, -7093 };
	g_output['E'] = { 114, 110, 100, 78, 38, -26, -120, -250, -422, -642, -916, -1250, -1650, -2122, -2672, -3306, -4030, -4850, -5772, -6802, -7946 };
	g_output['F'] = { 715, 625, 523, 409, 283, 145, -5, -167, -341, -527, -725, -935, -1157 };
}

Expression::Expression(int arr[NUM_TERMS]) {
	for (int i = 0; i < NUM_TERMS; i++) {
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
	if (!this->FinishFindout()) {
		cout << "not find out expression. " << endl;
		return;
	}
	string buff = "";
	for (int i = MAX_POWER; i >= 0 ; i--) {
		auto val = coef[i];
		if (val == 0) {
			continue;
		}
		if (buff != "" && val > 0 ) {
			buff += "+";
		}
		//if coef == 1, do not print it.
		if (abs(val) != 1 || i == 0) {
			buff += to_string(val);
		}
		if (0 == i) {
			break;
		}
		buff += "x";
		if (1 != i) {
			buff += "^" + to_string(i);
		}
	}
	cout << buff << endl;
}

string Expression::PrintOutPut() {
	string ret = "";
	if (0 == output.size()) {
		cout << "Out put is empty" << endl;
		system("pause");
		system("cls");
		return ret;
	}
	for (auto it = output.begin(); it != output.end(); it++) {
		if (it != output.begin()) {
			cout << ", ";
			ret += ", ";
		}
		cout << *it;
		ret += *it;
	}
	cout << endl;
	return ret;
}

void Expression::WriteOutPut() {
	try {
		SaveToFile(this->output);
	}
	catch (bool ret) {
		cout << "write file error" << endl;
		return;
	}
}

void Expression::Calculate(int start, int end) {
	vector<int> tmp;;
	for (auto x = start; x <= end; x++) {
		tmp.push_back(CalcExpression(coef, x));
	}
	this->output = tmp;
}

void Expression::MakeOutPut() {
	auto start = 0, end = 0;
	cout << "Please enter a range of x to check." << endl << "Start num: ";
	cin >> start;
	cout << endl << "End num: ";
	cin >> end;
	if (start > end) {
		throw 0;
		return;
	}
	this->Calculate(start, end);
	this->PrintOutPut();
}

bool Expression::test(int sample[NUM_TERMS]) {
	//lamda
	auto res = [=] {
		for (auto i = 0; i < output.size() ; i++) {
			auto res = CalcExpression(sample, i);
			if (output[i] !=  res){
				return false;
			}
		}
		return true;
	}();
	if (res) {
		for (int i = 0; i < NUM_TERMS; i++) {
			this->coef[i] = sample[i];
		}
	}
	return res;
}

//unknown terms.
bool Expression::findOutCoefs(int sample[NUM_TERMS], int terms_num, int index) {
	if (MAX_POWER == index || MAX_POWER == terms_num) {
		if (0 == terms_num) {
			return false;
		}
		return this->test(sample);
	}
	int tmp[NUM_TERMS] = {};
	for (int i = 0; i <= index; i++) {
		tmp[i] = sample[i];
	}
	index++;
	for (int a = MIN_COEF; a <= MAX_COEF; a++) {
		tmp[index] = a;
		if (0 != a) {
			if (true == findOutCoefs(tmp, terms_num + 1, index)) {
				if (0 == this->terms) {
					this->terms = terms_num + 1;
				}
				return true;
			}
		}
		else {
			if (true == findOutCoefs(tmp, terms_num, index)) {
				if (0 == this->terms) {
					this->terms = terms_num;
				}
				return true;
			}
		}
	}
	return false;
}

bool Expression::findOutCoefs(int need_coefs, int index,  int sample[NUM_TERMS] ) {
	//left coef not enough
	if (need_coefs > (NUM_TERMS - 1) - index) {
		return false;
	}
	if (0 == need_coefs || MAX_POWER == index) {
		return this->test(sample);
	}
	int tmp[NUM_TERMS] = {};
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


//if terms == 0, it's unknown terms.
void Expression::FindOut() {
	if (0 == output.size()) {
		throw 0;
		return;
	}
	auto tmp = terms;
	if (NUM_TERMS <= tmp) {
		throw 1;
		return;
	}
	//default first parameter is 0.
	int sample[NUM_TERMS]{output[0]};
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
		this->FindOut();
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

void Expression::setCoef(int i, int a) {
	if (i >= NUM_TERMS || i < 0) {
		return;
	}
	this->coef[i] = a;
}

bool Expression::FinishFindout() {
	//ternary operator
	return terms > 0 ? true : false;
}

void OneTermExpression::Solve() {
	auto len = output.size();
	if (len < SIMPLE_TERM_MIN_OUTPUT) {
		throw 0;
		return;
	}
	if (output[0] != 0) {
		throw 1;
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
		throw 0;
		return;
	}
	int a, b, n;
	auto k = output[0];
	if (0 != k) {
		a = output[1] - k, n = CalcPower((output[2] - k) / a, 2);
		this->setCoef(0, k);
		this->setCoef(n, a);
	}
	else {
		Expression::Solve();
	}
}


int CalcExpression(int coef[NUM_TERMS], int x) {
	auto ret = coef[0]; //k
	for (int i = 1; i <= MAX_POWER; i++) {
		ret += coef[i] * pow(x, i);
	}
	return ret;
}

void PrintModules(map<char, Expression> modules) {
	for (auto it = modules.begin(); it != modules.end(); it++) {
		cout << it->first << ": ";
		it->second.PrintExpression();
		cout << endl;
	}
}


void SolveModule(char key, int terms) {
	if (0 == g_output.count(key)) {
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

void SolveModules() {
	if (0 != g_modules.size()) {
		PrintModules(g_modules);
		return;
	}
	vector<thread> td_pool;
	td_pool.push_back(thread(&SolveModule, 'A', 1));
	td_pool.push_back(thread(&SolveModule, 'B', 2));
	td_pool.push_back(thread(&SolveModule, 'C', 3));
	td_pool.push_back(thread(&SolveModule, 'D', 4));
	td_pool.push_back(thread(&SolveModule, 'E', 0));
	td_pool.push_back(thread(&SolveModule, 'F', 0));
	//thread join
	if (0 == td_pool.size()) {
		return;
	}
	for (int i = 0; i < td_pool.size(); i++) {
		td_pool[i].join();
	}
	PrintModules(g_modules);
}
