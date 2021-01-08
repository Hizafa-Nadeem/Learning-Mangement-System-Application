#include "Transcript.h"

Transcript::Transcript(float c)
	:cgpa(c)
{
}
void Transcript::addSem(float s, string sem)
{
	SemesterTranscript temp;
	temp.sgpa = s;
	temp.semester = sem;
	semesters.push_back(temp);
}
void Transcript::printTranscript(string sem)
{
	for (int i = 0; i < semesters.size(); i++)
	{
		if (semesters[i].semester == sem)
		{
			cout << "SGPA " << semesters[i].sgpa << " ";
		}
	}
}
void Transcript::updateSemGpa(string sem, float s)
{
	for (int i = 0; i < semesters.size(); i++)
	{
		if (semesters[i].semester == sem)
		{
			semesters[i].sgpa = s;
			break;
		}
	}
}
void Transcript::updateCGPA(float c)
{
	cgpa = c;
}
float Transcript::getGPA()
{
	return cgpa;
}
Transcript::~Transcript()
{
}
