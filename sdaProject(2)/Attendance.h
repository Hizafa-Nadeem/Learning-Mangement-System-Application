#pragma once
#include <string>
#include "SQLAPI.h"
using namespace std;

class Attendance
{
	string classday;
	char status;
public:
	Attendance(string day, char s);
	void updateAttendance(char s);
	string getDate();
	char getLetter();
	void print_status();
	~Attendance();
};

