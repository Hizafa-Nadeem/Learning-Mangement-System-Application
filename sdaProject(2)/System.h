#include<vector>
#include<string>
#pragma once
#include"section.h"
#include"person.h"
#include"course.h"
//#include"student.h"
#include <string>

class student;



enum Person
{
	Teacher, Student, Officer
};

class System
{
	vector<person*>persons;
	vector<course>courses;

	int off_count = 0, tc_count = 0, st_count = 0;
	int c_c = 0;
public:
	System();
	string getpassword(Person type);
	person* turn_on_session(string n, string pass);
	void signup_academicofficer(string n, string pass, string r);
	string generatecoursecode();
	bool display_offered_courses(string sem, int csem);
	vector<string> getCoursesems(string code);
	int register_course(int course, int section, student &s);
	int get_sectionid(char section);  //gives the index number of Section A,B,C in the  section array
	bool with_course(int c_code, int sec, student& s);
	int extract_id(string code);
	void AddStudenttoSection(student &s, char sect, string code);
	void AddAttendance(string code, char sec);
	void AddEvaluation(string code, char sec, int w, int assign, int totalmarks);
	void AddMarks(string code, int sec, int assign);
	void AssignGrades(string code, char sec);
	void viewMarksSheet(string code, char sec);


	course& get_course(int c_id);

	section& get_section(int c_id, int sec);

	int d_cour(int c, int sec, student& s);

	
	friend class academic_officer;

};
