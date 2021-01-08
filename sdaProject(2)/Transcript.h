#pragma once
#include <string>
#include <vector>
#include "SQLAPI.h"
#include <iostream>
using namespace std;

struct SemesterTranscript {
	float sgpa;
	string semester;
};

class Transcript
{
	float cgpa;
	vector<SemesterTranscript> semesters;
public:
	Transcript(float c);
	void addSem(float s, string sem);
	void printTranscript(string sem);
	void updateSemGpa(string sem, float s);
	void updateCGPA(float c);
	float getGPA();
	~Transcript();
};

