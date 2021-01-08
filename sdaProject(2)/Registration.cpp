#include"Registration.h"
#include"course.h"
#include"section.h"
#include "Evaluation.h"
#include<iostream>
#include<string.h>
using namespace std;

Registration::Registration()
{
	grade = '\0';
}
Registration::Registration(course &c, section& s, int sem)
{
	semester = sem;
	cour =	&c;
	sect = &s;
}
void Registration::PrintRegistration()
{
	
		cour->printCourse();
}

string Registration::getcoursecode()
{
	return cour->getcoursecode();
}
void Registration:: AddAttendance(Attendance& a)
{
	attendance.push_back(a);
}
void Registration:: AddEvaluation(Evaluation& e)
{
	evaluations.push_back(e);
}
void Registration::AddMarks(int assign, string rollnum)
{
	int marks = 0;
	cout << "Enter marks: ";
	for (int i = 0; i < evaluations.size(); i++)
	{
		if (evaluations[i].getassignmentNo() == assign)
		{
			cin >> marks;
			evaluations[i].updateAssignmentMarks(marks);


			SAConnection con;

			SACommand update(&con, _TSA("update Marks set marksObtained=:1 where assignNo=:2 and rollno=:3 and courseCode=:4"));

			update.Param(1).setAsInt64() = marks;
			update.Param(2).setAsInt64() = assign;
			update.Param(3).setAsString() = rollnum.c_str();
			update.Param(4).setAsString() = cour->getcoursecode().c_str();


			con.Connect(_TSA("LAPTOP-MAPPIP6B@LMS"), _TSA("sa"), _TSA("password"), SA_SQLServer_Client);

			update.Execute();
			update.Close();

			con.Disconnect();

		}
	}
}
void Registration:: AssignGrades(string rollnum)
{
	char g;
	cout << "Enter grade: ";
	cin >> g;
	grade = g;


	SAConnection con;

	SACommand update(&con, _TSA("update Registration set grade=:1 where rollno=:2 and courseCode=:3"));

	update.Param(1).setAsString() = g;
	update.Param(2).setAsString() = rollnum.c_str();
	update.Param(3).setAsString() = cour->getcoursecode().c_str();


	con.Connect(_TSA("LAPTOP-MAPPIP6B@LMS"), _TSA("sa"), _TSA("password"), SA_SQLServer_Client);

	update.Execute();
	update.Close();

	con.Disconnect();
}

void Registration::viewMarksSheet()
{
	int overalltotal = 0;
	int stotal = 0;
	for (int i = 0; i < evaluations.size(); i++)
	{
		overalltotal = overalltotal + evaluations[i].getTotalmarks();
		stotal = stotal + evaluations[i].getStudentMarks();
	}

	cout << "Student's Marks Obtained: " << stotal << " ";
	cout << "Total Marks: " << overalltotal << " " << endl;
}


char Registration:: getGrade()
{
	return grade;
}

void Registration::setGrade(char g)
{
	grade = g;
}

bool Registration::operator== (string code)
{
	if (*(cour) == code)
		return true;
	else
		return false;
}

void Registration::PrintAttendance()
{
	if (attendance.size() == 0)
	{
		cout << "\n No attendence record!\n";
	}
	else
	{
		for (int i = 0; i < attendance.size(); i++)
		{
			attendance[i].print_status();
		}
	}
	
}

void Registration::LoadData(string rollnum)
{
	SAConnection con;
	con.Connect("LAPTOP-MAPPIP6B@LMS", "sa", "password", SA_SQLServer_Client);

	SACommand cmd(
		&con,
		_TSA("select assignNo, weightage, totalmarks, marksObtained from Marks where rollno = :1 and courseCode=:2"));    // load Evaluations

	cmd.Param(1).setAsString() = rollnum.c_str();
	cmd.Param(2).setAsString() = cour->getcoursecode().c_str();

	cmd.Execute();

	while (cmd.FetchNext())
	{
		int a = cmd.Field(_TSA("assignNo")).asInt64();
		int w = cmd.Field(_TSA("weightage")).asInt64();
		int tm = cmd.Field(_TSA("totalmarks")).asInt64();
		int mo = cmd.Field(_TSA("marksObtained")).asInt64();

		Evaluation e(a, w, mo, tm);
		evaluations.push_back(e);
	}

	cmd.Close();

	SACommand cmd1(
		&con,
		_TSA("select ClassDay, letter from Attendance where rollno = :1 and courseCode=:2"));    // load Attendance

	cmd1.Param(1).setAsString() = rollnum.c_str();
	cmd1.Param(2).setAsString() = cour->getcoursecode().c_str();

	cmd1.Execute();

	while (cmd1.FetchNext())
	{
		string d = cmd1.Field(_TSA("ClassDay")).asString().GetMultiByteChars();
		string letter = cmd1.Field(_TSA("letter")).asString().GetMultiByteChars();

		char status = letter[0];
		Attendance a(d, status);
		attendance.push_back(a);
	}

	cmd1.Close();

}

Registration::~Registration()
{
	cour = nullptr;
	sect = nullptr;
}

