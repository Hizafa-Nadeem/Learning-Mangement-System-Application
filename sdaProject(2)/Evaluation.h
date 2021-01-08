#pragma once
class Evaluation
{
	int assignmentno;
	int weightage;
	int marksObtained;
	int totalmarks;
public:
	Evaluation();
	Evaluation(int aNo, int w, int mObtained, int totalm);
	void updateAssignmentMarks(int marks);
	int getassignmentNo();
	int getTotalmarks();
	int getWeightage();
	int getStudentMarks();
	~Evaluation();
};

