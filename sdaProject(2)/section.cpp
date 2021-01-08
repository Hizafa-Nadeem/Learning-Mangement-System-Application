#include"section.h"
#include"course.h"
#include "student.h"
#include"state.h"
#include"register_close.h"
#include "Attendance.h"
#include "Evaluation.h"
#include"section_full.h"

	section::section() {}
	section::section(char t, string r, int t_seats, course* co):title(t),room(r),total_seats(t_seats)
	{
		c = co;
		registration_stat = new register_close(*this); // when section is created default section registration state is closed

		//SAConnection con;

		//SACommand cmd(&con, _TSA("select * from observers"));

		//try {
		//	con.Connect(_TSA("LAPTOP-MAPPIP6B@LMS"), _TSA("sa"), _TSA("password"), SA_SQLServer_Client);

		//	cmd.Execute();

		//	while (cmd.FetchNext())
		//	{
		//		string cc = cmd.Field(_TSA("code")).asString().GetMultiByteChars();
		//		string rollno = cmd.Field(_TSA("rollno")).asString().GetMultiByteChars();
		//		string sect = cmd.Field(_TSA("section")).asString().GetMultiByteChars();

		//	}
		//	cmd.Close();

		//	con.Disconnect();
		//}
		//catch (SAException &x) {
		//	con.Rollback();
		//	printf("%s\n", x.ErrText().GetMultiByteChars());
		//}
	}
	void section::change_reg_state(state* rs)
	{
		if (registration_stat != nullptr)
		{
			//delete registration_stat;
		}
		registration_stat = rs;
	}

	int section::register_section(student &s)
	{
		return registration_stat->register_section(s);
	}
	int section::register_student(student& s)
	{
		
		registered_students.push_back(s);
		if (registered_students.size() == total_seats)
		{
			state *st = new section_full(*this);//Section full update in database, set as 3
			(this)->change_reg_state(st);

			SAConnection con;

			SACommand update(&con, _TSA("update Section set regState=:1 where coursecode=:2"));

			update.Param(1).setAsInt64() = 3;
			update.Param(2).setAsString() = c->getcoursecode().c_str();

			try {
				con.Connect(_TSA("LAPTOP-MAPPIP6B@LMS"), _TSA("sa"), _TSA("password"), SA_SQLServer_Client);

				update.Execute();
				update.Close();

				con.Disconnect();
			}
			catch (SAException &x) {
				con.Rollback();
				printf("%s\n", x.ErrText().GetMultiByteChars());
			}
		}
		return 0;
	}
	bool section::add_student_in_waiting(student& s)
	{
		observers.push(&s);
		//SAConnection con;
		//con.Connect("LAPTOP-MAPPIP6B@LMS", "sa", "password", SA_SQLServer_Client);

		//SACommand cmd(
		//	&con,
		//	_TSA("insert into observers (rollno, code, section) values (:1, :2, :3)"));    // Add evaluation

		//cmd.Param(1).setAsString() = s.getroll().c_str();
		//cmd.Param(2).setAsString() = c->getcoursecode().c_str();
		//cmd.Param(3).setAsString() = title;

		//cmd.Execute();

		//cmd.Close();
		return false;
	}
	bool section::availability()
	{
		return registration_stat->availability();
		
	}
	void section:: PrintSection()
	{
		cout << "Section: " << title << endl;
	}

	void section::printRegisteredStudents(string code)
	{
		char attend;
		char date[11];
		cout << "Enter date (yyyy-mm-dd): ";
		cin.getline(date, 11, '\n');
		
		cin.clear();

		for (int i = 0; i < registered_students.size(); i++)
		{
			registered_students[i].print();
			cout << "Mark Attendance for this student ";
			//cin.ignore();
			cin >> attend;
			Attendance a(date, attend);
			registered_students[i].AddAttendance(a, code, title);
		}
	}

	void section:: AddEvaluation(string code, int w, int assign, int totalmarks)
	{
		for (int i = 0; i < registered_students.size(); i++)
		{
			registered_students[i].print();
			Evaluation e(assign, w, 0, totalmarks);
			registered_students[i].AddEvaluation(e, code, title);
		}
	}

	void section:: AddMarks(string code, int assign)//update marks
	{
		for (int i = 0; i < registered_students.size(); i++)
		{
			registered_students[i].print();
			
			registered_students[i].AddMarks(code, assign);
		}

	}

	void section:: AssignGrades(string code, char sec)
	{
		for (int i = 0; i < registered_students.size(); i++)
		{
			registered_students[i].print();

			registered_students[i].AssignGrades(code);
		}
	}

	void section::viewMarksSheet(string code)
	{
		for (int i = 0; i < registered_students.size(); i++)
		{
			registered_students[i].print();

			registered_students[i].viewMarksSheet(code);
		}
	}

	bool section::removecourse(student &s)
	{
		return registration_stat->rem_course(s);
	}
	bool section::withdraw_course(student& s)
	{
		return registration_stat->withdraw_course(s);
	}
	bool section::remove_student(student& s)
	{
		for (int i = 0; i < registered_students.size(); i++)
		{
			if (s == registered_students[i])
			{
				registered_students.erase(registered_students.begin() + i);
				
				notify();
				return true;
			}
		}
		return false;
	}
	void section::notify()
	{
		if (observers.size() != 0)
		{
			observer* obs = observers.front();
			observers.pop();
			string Name = typeid(*(obs)).name();

			obs->update(c, &(*this));//course and section
		}

	}
	char section::get_title()
	{
		return title;
	}
