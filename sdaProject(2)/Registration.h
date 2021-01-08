#pragma once
class course;
class section;
#include<string.h>
#include<string>
#include "System.h"
#include "Attendance.h"
#include "Evaluation.h"
using namespace std;

class Registration
{
	course* cour;
	section* sect;
	char grade;
	vector<Evaluation>evaluations;
	vector<Attendance>attendance;
	int semester;
public:
	Registration();
	
	Registration(course &c, section& s, int sem);
	void AddAttendance(Attendance& a);
	void AddEvaluation(Evaluation& e);
	void AddMarks(int assign, string rollnum);
	void AssignGrades(string rollnum);
	string getcoursecode();
	void PrintRegistration();
	void setGrade(char g);
	void viewMarksSheet();
	void PrintAttendance();
	char getGrade();
	bool operator==(string code);
	void LoadData(string rollnum);
	
	~Registration();

	friend class Transcript;
};