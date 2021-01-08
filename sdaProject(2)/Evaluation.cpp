#include "Evaluation.h"

Evaluation::Evaluation()
{
	marksObtained = 0;
}

Evaluation::Evaluation(int assignNo, int w, int mObtained, int totalm)
	:assignmentno(assignNo), weightage(w), marksObtained(mObtained), totalmarks(totalm)
{
}

void Evaluation::updateAssignmentMarks(int m)
{
	marksObtained = m;
}
int Evaluation::getassignmentNo()
{
	return assignmentno;
}
int Evaluation::getTotalmarks()
{
	return totalmarks;
}
int Evaluation:: getStudentMarks()
{
	return marksObtained;
}
int Evaluation::getWeightage()
{
	return weightage;
}
Evaluation::~Evaluation()
{
}
