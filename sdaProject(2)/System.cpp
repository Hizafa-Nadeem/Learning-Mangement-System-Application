#include"System.h"
#include"officer.h"
#include "teacher.h"
#include"student.h"
#include "course.h"
#include "register_open.h"
#include "register_close.h"
#include "section_full.h"
#include "withdraw_close.h"
#include "withdraw_open.h"
#include "SQLAPI.h"

	System::System()
	{
		SAConnection con;
		con.Connect("LAPTOP-MAPPIP6B@LMS", "sa", "password", SA_SQLServer_Client);

		SACommand LoadObjects(
			&con,
			_TSA("select Person.username, Person.password, AO. rank from Person join AO on Person.username = AO.name"));    // loading academic officers into array


		//select from table
		LoadObjects.Execute();

		//fetch results row by row and print results
		while (LoadObjects.FetchNext())
		{
			person* per = new academic_officer((string)LoadObjects.Field(_TSA("username")).asString().GetMultiByteChars(), (string)LoadObjects.Field(_TSA("password")).asString().GetMultiByteChars(),
				(string)LoadObjects.Field(_TSA("rank")).asString().GetMultiByteChars(), *this);
			persons.push_back(per);
		}

		LoadObjects.Close();

		SACommand cmd3(
			&con,
			_TSA("select * from Course"));    // loading Courses into array

		char codeparam[9];
		//select from table
		cmd3.Execute();
		vector<string> sems;

		//fetch results row by row and print results
		while (cmd3.FetchNext())
		{
			string c = (string)cmd3.Field(_TSA("code")).asString().GetMultiByteChars();
			string t = (string)cmd3.Field(_TSA("courseTitle")).asString().GetMultiByteChars();
			int cred = cmd3.Field(_TSA("credHrs")).asInt64();
			int offersem = cmd3.Field(_TSA("OfferedtoSem")).asInt64();

			course course1(c, t, cred, sems, offersem);
			courses.push_back(course1);
		}

		cmd3.Close();

		SACommand cmd4(
			&con,
			_TSA("select coursecode, semester from CourseinSem where coursecode=:1"));    // load courseinsem

		for (int i = 0; i < courses.size(); i++)
		{
			strcpy(codeparam, courses[i].course_code.c_str());

			cmd4.Param(1).setAsString() = codeparam;

			cmd4.Execute();

			while (cmd4.FetchNext())
			{
				string cc = (string)cmd4.Field(_TSA("coursecode")).asString().GetMultiByteChars();
				string s = (string)cmd4.Field(_TSA("semester")).asString().GetMultiByteChars();
				courses[i].sems_taught.push_back(s);
			}

			cmd4.Close();
		}

		//Loading Sections

		SACommand cmd6(
			&con,
			_TSA("select * from Section where coursecode=:1"));    // loading sections into array


		//fetch results row by row and print results
		for (int i = 0; i < courses.size(); i++)
		{
			cmd6.Param(1).setAsString() = courses[i].course_code.c_str();
			cmd6.Execute();

			while (cmd6.FetchNext())
			{
				string sect = (string)cmd6.Field(_TSA("section")).asString().GetMultiByteChars();
				string cc = (string)cmd6.Field(_TSA("coursecode")).asString().GetMultiByteChars();
				string r = (string)cmd6.Field(_TSA("room")).asString().GetMultiByteChars();
				int t = cmd6.Field(_TSA("totalseats")).asInt64();
				int regstate = cmd6.Field(_TSA("regState")).asInt64();

				char* temp = new char[2];
				strcpy(temp, sect.c_str());
				char letter;
				letter = temp[0];

				section* s = new section(letter, r, t, &(courses[i]));

				if (regstate == 1)
				{

					register_open *r = new register_open(*s);
					s->change_reg_state(r);

				}
				else if (regstate == 2)
				{
					register_close *rc = new register_close(*s);
					s->change_reg_state(rc);
				}
				else if (regstate == 3)
				{
					section_full *sf = new section_full(*s);
					s->change_reg_state(sf);
				}
				else if (regstate == 4)
				{
					withdraw_open *wo = new withdraw_open(*s);
					s->change_reg_state(wo);
				}
				else if (regstate == 5)
				{
					withdraw_close *wr = new withdraw_close(*s);
					s->change_reg_state(wr);
				}

				courses[i].sections.push_back(*s);

			}

			cmd6.Close();
		}

		SACommand cmd(
			&con,
			_TSA("select Person.username, Person.password, Student.name, Student.rollno, Student.program, Student.semester, Student.currSem from Person join Student on Person.username = Student.uname"));    // loading Students into array


		//select from table
		cmd.Execute();//loading students

		//fetch results row by row and print results
		while (cmd.FetchNext())
		{
			person* per = new student(cmd.Field(_TSA("name")).asString().GetMultiByteChars(), cmd.Field(_TSA("rollno")).asString().GetMultiByteChars(), cmd.Field(_TSA("username")).asString().GetMultiByteChars(), 
									cmd.Field(_TSA("password")).asString().GetMultiByteChars(), cmd.Field(_TSA("program")).asString().GetMultiByteChars(),
										cmd.Field(_TSA("semester")).asString().GetMultiByteChars(), cmd.Field(_TSA("currSem")).asInt64(), this);
			
			persons.push_back(per);
		}
		
		cmd.Close();

		SACommand cmd2(
			&con,
			_TSA("select P.username, P.password, T.name, T.designation, T.paygrade from Person P join Teacher T on P.username = T.uname"));    // loading teachers into array


		//select from table
		cmd2.Execute();

		//fetch results row by row and print results
		while (cmd2.FetchNext())
		{
			person* per = new teacher((string)cmd2.Field(_TSA("name")).asString().GetMultiByteChars(), (string)cmd2.Field(_TSA("designation")).asString().GetMultiByteChars(), (string)cmd2.Field(_TSA("username")).asString().GetMultiByteChars(),
				(string)cmd2.Field(_TSA("password")).asString().GetMultiByteChars(), cmd2.Field(_TSA("paygrade")).asInt64(), this);
			persons.push_back(per);
		}

		cmd2.Close();

		SACommand variables(
			&con,
			_TSA("select * from variables"));    // loading variables into array


		//select from table
		variables.Execute();

		//fetch results row by row and print results
		while (variables.FetchNext())
		{
			tc_count = variables.Field(_TSA("tcount")).asInt64();
			off_count = variables.Field(_TSA("ocount")).asInt64();
			st_count = variables.Field(_TSA("scount")).asInt64();
			c_c = variables.Field(_TSA("ccount")).asInt64();
			
		}

		variables.Close();
	}
	string System::getpassword(Person type)
	{

		string password;
		if (type == Officer)
		{
			password = "off";
			off_count++;
			password = password + std::to_string(off_count);
		}
		else if (type == Teacher)
		{
			password = "tc";
			tc_count++;
			password = password + std::to_string(tc_count);
		}
		else if (type == Student)
		{
			password = "st";
			st_count++;
			password = password + std::to_string(st_count);
		}
		return password;
	}
	person* System::turn_on_session(string n, string pass)
	{
		person* per = nullptr;
		SAConnection con;
		con.Connect("LAPTOP-MAPPIP6B@LMS", "sa", "password", SA_SQLServer_Client);

		SACommand cmd1(
			&con,
			_TSA("select username, password from Person"));    // command object

		char* name = new char[100];
		char* password = new char[50];
		strcpy(name, n.c_str());
		strcpy(password, pass.c_str());
		string r;

		//SACommand cmd2(
		//	&con,
		//	_TSA("select name, rank from AO where name=:1"));    // command object

		//cmd2.Param(1).setAsString() = _TSA(name);

		//select from table
		cmd1.Execute();

		//fetch results row by row and print results
		while (cmd1.FetchNext())
		{

			if (pass.compare((string)cmd1.Field(_TSA("password")).asString().GetMultiByteChars()) == 0 && n.compare((string)cmd1.Field(_TSA("username")).asString().GetMultiByteChars()) == 0)
			{
				cmd1.Close();
				System test;
				//cmd2.Execute();

				/*while (cmd2.FetchNext())
				{
					r = cmd2.Field(_TSA("rank")).asString().GetMultiByteChars();
				}*/

				//per = new academic_officer(n, pass, r, test);
				bool person_pointer_fetched = false;
				for (int i = 0; i < persons.size() && person_pointer_fetched == false; i++)
				{
					per = persons[i]->login(n, pass);
					if (per != nullptr)
					{
						person_pointer_fetched = true;
					}
				}
				break;
			}
			else
				per = nullptr;

		}

		return per; //return pointer to the object which has to be logged in 
	}
	void System::signup_academicofficer(string n, string pass, string r)
	{
		
		person* p = new academic_officer(n, pass, r, *this);
		persons.push_back(p);

		SAConnection con;
		con.Connect("LAPTOP-MAPPIP6B@LMS", "sa", "password", SA_SQLServer_Client);

		SACommand cmd1(
			&con,
			_TSA("insert into Person(username, password) values(:1, :2)"));    // command object

		char* name = new char[100];
		char* password = new char[50];
		strcpy(name, n.c_str());
		strcpy(password, pass.c_str());
		cmd1.Param(1).setAsString() = name;
		cmd1.Param(2).setAsString() = password;

		SACommand insert(
			&con,
			_TSA("insert into AO(name, rank) values(:1, :2)"));    // command object

		char* rank = new char[50];
		strcpy(rank, r.c_str());
		insert.Param(1).setAsString() = name;
		insert.Param(2).setAsString() = rank;

		// insert into table
		cmd1.Execute();
		insert.Execute();

		cout << "\nYou have signed up sucessfully!\n";
	}
	string System::generatecoursecode()
	{
		string code = "CS";
		c_c = c_c+ 1;
		code = code + std::to_string(c_c);
		return code;
	}
	// student functions
	bool System::display_offered_courses(string sem, int csem)//flag changed***********
	{
		bool flag = false;
		for (int i = 0; i < courses.size(); i++)
		{
			for (int j = 0; j < courses[i].sections.size(); j++)
			{
				if (courses[i].sections[j].availability() == true and courses[i].offeredTo==csem)
				{
					courses[i].printCourse();
					courses[i].sections[j].PrintSection();
					flag = true;

				}
			}

		}
		return flag;
	}

	vector<string> System:: getCoursesems(string code)
	{
		for (int i = 0; i < courses.size(); i++)
		{
			if (courses[i].course_code == code)
				return courses[i].sems_taught;
		}
	}

	int System::register_course(int c, int si, student& s) //pass by reference 
	{
		
		return courses[c-1].sections[si].register_section(s);//************-1
	}

	


	int System::get_sectionid(char section)
	{
			int id = 0;
			id = (section - 65);
			//id = (id - 48);
			return id;
	
	}
	int System::extract_id(string code)
	{
		string id = code.substr(2, code.size() - 2);
		stringstream geek(id);
		int x = 0;
		geek >> x;
		return x;
	}


	void System::AddStudenttoSection(student &s, char sect, string code)
	{
		int courseindex = 0;
		int sectindex = 0;
		for (int i = 0; i < courses.size(); i++)
		{
			if (courses[i].course_code == code)
			{
				courseindex = i;
				break;
			}
		}

		for (int i = 0; i < courses[courseindex].sections.size(); i++)
		{
			if (courses[courseindex].sections[i].title == sect)
			{
				sectindex = i;
				break;
			}
		}

		courses[courseindex].sections[sectindex].register_student(s);
	}

	void System::AddAttendance(string code, char sec)
	{
		int courseindex = 0;
		int sectindex = 0;
		for (int i = 0; i < courses.size(); i++)
		{
			if (courses[i].course_code == code)
			{
				courseindex = i;
				break;
			}
		}

		for (int i = 0; i < courses[courseindex].sections.size(); i++)
		{
			if (courses[courseindex].sections[i].title == sec)
			{
				sectindex = i;
				break;
			}
		}
		courses[courseindex].sections[sectindex].printRegisteredStudents(code);
	}

	void System:: AddEvaluation(string code, char sec, int w, int assign, int totalmarks)
	{
		int courseindex = 0;
		int sectindex = 0;
		for (int i = 0; i < courses.size(); i++)
		{
			if (courses[i].course_code == code)
			{
				courseindex = i;
				break;
			}
		}

		for (int i = 0; i < courses[courseindex].sections.size(); i++)
		{
			if (courses[courseindex].sections[i].title == sec)
			{
				sectindex = i;
				break;
			}
		}
		courses[courseindex].sections[sectindex].AddEvaluation(code, w, assign, totalmarks);
	}

	void System::AddMarks(string code, int sec, int assign)
	{
		int courseindex = 0;
		int sectindex = 0;
		for (int i = 0; i < courses.size(); i++)
		{
			if (courses[i].course_code == code)
			{
				courseindex = i;
				break;
			}
		}

		for (int i = 0; i < courses[courseindex].sections.size(); i++)
		{
			if (courses[courseindex].sections[i].title == sec)
			{
				sectindex = i;
				break;
			}
		}
		courses[courseindex].sections[sectindex].AddMarks(code, assign);
	}
	void System:: AssignGrades(string code, char sec)
	{
		int courseindex = 0;
		int sectindex = 0;
		for (int i = 0; i < courses.size(); i++)
		{
			if (courses[i].course_code == code)
			{
				courseindex = i;
				break;
			}
		}

		for (int i = 0; i < courses[courseindex].sections.size(); i++)
		{
			if (courses[courseindex].sections[i].title == sec)
			{
				sectindex = i;
				break;
			}
		}
		courses[courseindex].sections[sectindex].AssignGrades(code, sec);
	}
	void System:: viewMarksSheet(string code, char sec)
	{
		int courseindex = 0;
		int sectindex = 0;
		for (int i = 0; i < courses.size(); i++)
		{
			if (courses[i].course_code == code)
			{
				courseindex = i;
				break;
			}
		}

		for (int i = 0; i < courses[courseindex].sections.size(); i++)
		{
			if (courses[courseindex].sections[i].title == sec)
			{
				sectindex = i;
				break;
			}
		}
		courses[courseindex].sections[sectindex].viewMarksSheet(code);
	}
	course& System::get_course(int c_id) // function for getting course reference
	{
		return courses[c_id-1];// direct indexing
	}
	section& System::get_section(int c_id,int sec) //function for getting section reference
	{
		return courses[c_id-1].sections[sec];// direct indexing
	}
	int System::d_cour(int c, int sec,student& s)// drop course
	{
		return courses[c - 1].sections[sec].removecourse(s);
	}
	bool System::with_course(int c_code, int sec, student& s)
	{
		return courses[c_code - 1].sections[sec].withdraw_course(s);
	}