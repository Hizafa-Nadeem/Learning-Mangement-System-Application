#pragma once
#include"person.h"
#include "System.h"

class teacher :public person
{
	string designation;
	int paygrade;
	string name;
	string username;
	string password;
	System* sys;

public:
	teacher(string name, string designation, string username, string password, int paygrade, System* s);
	person* login(string n, string pass);
	void print();
	void display_menu();
	void setAttendance(string code, char sec);
	void addEvaluation(string code, char sec, int w, int assign, int totalmarks);
	void addMarks(string code, char sec, int assign);
	void assigngrades(string code, char sec);
	void viewMarksSheet(string code, char sec);
	void display_Functionalities(int option);

};

