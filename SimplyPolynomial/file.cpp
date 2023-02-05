#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<mutex>
#include"file.h"

using namespace std;

fstream g_file;
mutex file_mtx;
string g_file_path = "test.txt";

//Write the calculated expressions to a file.
//@param buff - Output of expression
bool WriteDownOutput(vector<int> buff) {
	if (0 == buff.size()) {
		return false;
	}
	file_mtx.lock();
	g_file.open(g_file_path, ios::out | ios::app);
	if (!g_file.is_open()) {
		file_mtx.unlock();
		return false;
	}
	for (auto it = buff.begin(); it != buff.end(); it++) {
		if (it != buff.begin()) {
			g_file << ", ";
		}
		g_file << *it;
	}
	g_file << endl;
	g_file.close();
	file_mtx.unlock();
	cout << "save finished." << endl;
	return true;
}

//Reads files whom contents are the output of an expression.
//@param outputs - buffer
 bool ReadOutputFile(vector<string> &outputs) {
	file_mtx.lock();
	g_file.open(g_file_path, ios::in);
	if (!g_file.is_open()) {
		file_mtx.unlock();
		return false;
	}
	string buff;
	while(getline(g_file, buff)) {
		outputs.push_back(buff);
	}
	g_file.close();
	file_mtx.unlock();
	return true ;
}