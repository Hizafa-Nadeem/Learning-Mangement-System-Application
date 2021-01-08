#include "Attendance.h"
#include <iostream>

Attendance::Attendance(string day, char s)
{
	classday = day;
	status = s;
}

void Attendance::updateAttendance(char s)
{
	status = s;
}

string Attendance::getDate()
{
	return classday;
}
char Attendance:: getLetter()
{
	return status;
}

void Attendance::print_status()
{
	cout<<"Date of Class "<<classday<<" Status " << status<<endl;
}

Attendance::~Attendance()
{
}