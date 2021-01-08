
#pragma once
#include"person.h"
#include"observer.h"
#include "Transcript.h"
#include "Attendance.h"
#include "Evaluation.h"
//#include"System.h"
class course;
class System;
class Registration;
class section;

class student :public person,public observer
{
	vector<Registration*> registrations;

	string name;
	string rollnum;
	string password;
	string program;
	System* sys;
	string username;
	string semester; 
	int currentSem;
	Transcript* transcript;

public:
	student();
	student(string n, string r, string u, string pass, string prog, string sem, int currSem, System *s);
	person* login(string n, string pass);
	void print();
	void display_menu();
	void display_Functionalities(int option);
	int add_course(string c_code, char sec);
	int drop_course(string c_code, char sec);
	int search_registrations(string c_code);
	void update(course* c, section* s);
	bool withdraw_course(string c_code, char sec);
	bool operator==(const student& s);
	void AddAttendance(Attendance& a, string code, char sec);
	void AddEvaluation(Evaluation& e, string code, char sec);
	void AddMarks(string code, int assign);
	void AssignGrades(string code);
	void viewMarksSheet(string code);
	//void AddTranscript();
	string getroll();
	void view_Registrations();
	void viewTranscript();
	void viewAttendance();
	//void viewMarks(string code);

	~student();
	friend class Transcript;

};

