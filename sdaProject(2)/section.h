

#pragma once
//#include"state.h"
//#include "course.h"
#include"student.h"
#include<string>
#include<vector>
#include"subject.h"
#include<queue>
class register_close;
class state;
class course;
class student;
class section_full;

class section :public subject
{
	char title;
	string room;
	int total_seats;
	course* c;
	state* registration_stat;
	vector<student> registered_students;//translates into the registration table 

public:
	section();
	section(char t, string r, int t_seats,course* course);
	void change_reg_state(state* rs);
	int register_section(student &s);
	int register_student(student& s);
	bool add_student_in_waiting(student& s);
	bool availability();
	void PrintSection();
	bool removecourse(student& s);
	bool remove_student(student& s);
	void printRegisteredStudents(string code);
	void AddEvaluation(string code, int w, int assign, int totalmarks);
	void AddMarks(string code, int assign);
	void AssignGrades(string code, char sec);
	void viewMarksSheet(string code);
	bool withdraw_course(student& s);
	void notify();
	char get_title();
	friend class System;
};

