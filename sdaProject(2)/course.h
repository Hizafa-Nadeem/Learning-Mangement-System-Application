#pragma once
#include<string.h>
#include<string>
#include<vector>
using namespace std;
//#include "section.h"
class section;

class course
{
		//course will be placed in course array in id -1 inde
		// course id = number in course code
		string course_code;
		string course_title;
		int credit_hrs;
		int offeredTo;
		vector<string> sems_taught;
		vector<section> sections;
	public:
		course();
		course(string code, string title, int crd, vector<string> sem, int Offerto);
		void add_section(section s);
		bool open_registration();
		bool close_registration();
		void printCourse();
		bool open_withdraw();
		string getcoursecode();
		bool close_withdraw();
		bool operator==(string code);
		string get_title();
		friend class System;
	};
