#include<string>
#pragma once
#include"person.h"
#include "System.h"
#include"student.h"
#include"course.h"
#include"section.h"
#include<iostream>
#include <sstream>
using namespace std;



class academic_officer :public person
{
	System* sys; //academic officer has access to the system
	string rank;

public:

	academic_officer();

	academic_officer(string n, string pass, string r, System &s);
	
	person* login(string n, string pass);
	void print();
	void  display_menu();

	int add_course(string code, string title, int credit_hrs, vector<string> sems, int semOffered, int semcode);
	void add_teacher(string name, string desig, string uname, string pass, int pay);
	void add_student(string n, string pass, string rollno, string prog, string u, string sem, int csem);
	int add_section(char name, string room, int t_seats, string course_code);
	void open_registration(string course);
	void close_registration(string course);
	void display_Functionalities(int option);
	void open_withdraw(string course);
	void close_withdraw(string course);

};
