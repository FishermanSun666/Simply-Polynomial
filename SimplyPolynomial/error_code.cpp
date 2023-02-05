#include<iostream>

#include "error_code.h"

using namespace std;

void ExceptionHandling(int code) {
	switch (code) {
	case PROCESS_ERROR:
		cout << "Something error." << endl;
		return;
	case SOLVE_EXAMPLE_ERROR:
		cout << "Solve example expression error." << endl;
		return;
	case INPUT_ERROR:
		cout << "Input error, please re-enter." << endl;
		return;
	case OUTPUT_IS_EMPTY:
		cout << "Output sequence is empty." << endl;
		return;
	case EXPERSSION_NOT_COMPLETE:
		cout << "Expression not complete." << endl;
		return;
	case WRITE_FILE_ERROR:
		cout << "Write file error." << endl;
		return;

	default:
		cout << "Code error, code: " << code << endl;
		return;
	}
	cin.get();
}