#include"course.h"
#include "register_open.h"
#include "register_close.h"
#include"withdraw_open.h"
#include"withdraw_close.h"
	course::course() {};
	course::course(string code, string title, int crd, vector<string> sem, int Offerto)	
	{
		course_code = code, course_title = title, credit_hrs = crd; offeredTo = Offerto;
		sems_taught = sem;
	}
	void course::add_section(section s) // setter 
	{
		sections.push_back(s);
	}
	bool course::open_registration()
	{
		for (int i = 0; i < sections.size(); i++)
		{
			state* s = new register_open(sections[i]);
			sections[i].change_reg_state(s);

			SAConnection con;

			SACommand update(&con, _TSA("update Section set regState=:1 where coursecode=:2"));

			update.Param(1).setAsInt64() = 1;
			update.Param(2).setAsString() = course_code.c_str();
			//update.Param(3).setAsString() = course_title.c_str();

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
		return true;
	}
	bool course::close_registration()
	{
		for (int i = 0; i < sections.size(); i++)
		{
			state* s = new register_close(sections[i]);
			sections[i].change_reg_state(s);//change of all sections will be changed

			SAConnection con;

			SACommand update(&con, _TSA("update Section set regState=:1 where coursecode=:2"));

			update.Param(1).setAsInt64() = 2;
			update.Param(2).setAsString() = course_code.c_str();

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
		return true;
	}
	bool course::open_withdraw()
	{
		for (int i = 0; i < sections.size(); i++)
		{
			state* s = new withdraw_open(sections[i]);
			sections[i].change_reg_state(s);//update open 

			SAConnection con;

			SACommand update(&con, _TSA("update Section set regState=:1 where coursecode=:2"));

			update.Param(1).setAsInt64() = 4;
			update.Param(2).setAsString() = course_code.c_str();

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
		return true;
	}
	bool course::close_withdraw()
	{
		for (int i = 0; i < sections.size(); i++)
		{
			state* s = new withdraw_close(sections[i]);
			sections[i].change_reg_state(s);//change of all sections will be changed

			SAConnection con;

			SACommand update(&con, _TSA("update Section set regState=:1 where coursecode=:2"));

			update.Param(1).setAsInt64() = 5;
			update.Param(2).setAsString() = course_code.c_str();

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
		return true;
	}
	string course::getcoursecode()
	{
		return course_code;
	}
	void course::printCourse()
	{
		cout << "\nCourse Code " << course_code << "\nCourse Title " << course_title << "\nCredit Hours " << credit_hrs<<" ";

	}

	bool course::operator==(string code)
	{
		if (course_code == code)
			return true;
		else
		return false;
	}
	string course::get_title()
	{
		return course_code;
	}


