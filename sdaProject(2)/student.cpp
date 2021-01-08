#include"student.h"
#include"person.h"
#include"System.h"
#include"Registration.h"
#include "state.h"
#include "Attendance.h"
#include"section.h"
#include"string.h"
#include "SQLAPI.h"

	student::student(){}
	student::student(string n, string r, string u, string pass, string prog, string sem, int currSem, System *s):name(n), rollnum(r), username(u), password(pass), 
		program(prog), semester(sem), currentSem(currSem), sys(s)
	{
		transcript = new Transcript(rand() % 4);
		SAConnection con;
		con.Connect("LAPTOP-MAPPIP6B@LMS", "sa", "password", SA_SQLServer_Client);
	
		SACommand cmd(
			&con,
			_TSA("select * from Registration where rollno=:1"));    // load Registrations

		cmd.Param(1).setAsString() = rollnum.c_str();

		cmd.Execute();

		while (cmd.FetchNext())
		{
			string cc = (string)cmd.Field(_TSA("courseCode")).asString().GetMultiByteChars();
			string rollno = (string)cmd.Field(_TSA("rollno")).asString().GetMultiByteChars();
			string sect = cmd.Field(_TSA("section")).asString().GetMultiByteChars();
			string g = cmd.Field(_TSA("grade")).asString().GetMultiByteChars();
			int sem = cmd.Field(_TSA("semcode")).asInt64();

			char gr = g[0];
			char temp[2];
			strcpy(temp, sect.c_str());
			char temp1 = temp[0];
			vector<string> sems = sys->getCoursesems(cc);
			
			char sec = sect[0];

			int cid = sys->extract_id(cc);
			int sid = sys->get_sectionid(sec);
			course *c =&(sys->get_course(cid));
			section* st = &(sys->get_section(cid, sid));
			Registration* r = new Registration(*c, *st, sem);

			registrations.push_back(r);
			r->setGrade(gr);
			r->LoadData(rollnum);
			sys->AddStudenttoSection(*this, sect[0], cc);
		}

		cmd.Close();
	
	}
	
	person* student::login(string n, string pass)
	{
		person* p = &(*this);
		if (username == n && password == pass)
		{
			return p;
		}
		else
			return nullptr;
	}
	void student::print() {
		cout << " \nStudent ";
		cout << "Username: " << username << " ";
		cout << " Name: " << name << " ";
		cout << " Program " << program<<endl;
	
	}
	void student::display_menu() {
		cout << "\n*******Menu **********\n1.View Offered Courses Press 1\n2.Register Course Press 2\n3.Drop course  Press 3\n4.Withdraw course Press 4\n5.View Transcript Press 5\n6.View Attendance Press 6\n7 View Registrations Press 7\n8.To terminate the session Press 0\n" << endl;
	}
	void student::display_Functionalities(int option){
		char sec;
		char code[7];
		string c_code;
		while (option != 0)
		{
			if (option == 1)
			{
				if (sys->display_offered_courses(this->semester, this->currentSem) == false)
				{
					cout << "\nNo Courses are Offered!\n ";
				}
			}
			else if (option == 2)
			{
				int sem;
				cout << "\nEnter Course code: ";
				cin.ignore();
				cin.getline(code, 6, '\n');
				cout << "\nEnter section name: ";
				cin >> sec;
				cin.ignore();
				//cout << "\nEnter semester code: ";
				//cin >> sem;

				//Registered in section
				string ccode(code);
				int id = add_course(code, sec);
				if (id == 0)// student will be added in section
				{
					cout << "\nRegisteration Successful!\n";
				}
				else if (id == 1)
				{
					cout << "\nRegisteration period is closed!!\n";
				}
				else if (id == 2)
				{
					cout << "\nSection is Full! You have been added in waiting list!\n";
				}

			}
			else if (option == 3)
			{
				// drop course
				cout << "\nEnter course code: ";
				cin >> c_code;
				cout << "\nEnter section name: ";
				cin >> sec;

				if (drop_course(c_code, sec) == false)
				{
					cout << "\nCannot drop Course!Registration Period is not active\n";
				}
				else
				{
					cout << "\nCourse has been dropped!\n";
				}

			}
			else if (option == 4)
			{
				// withdraw course
				cout << "\nEnter course code: ";
				cin >> c_code;
				cout << "\nEnter section name: ";
				cin >> sec;

				if (withdraw_course(c_code, sec) == false)
				{
					cout<<"\nCannot Withdraw course!Period is not active!\n";
				}
				else
				{
					cout<<"\nWithdrawal Successful course!\n";
				}


				// withdraw course
			}
			else if (option == 5)//view transcript
			{
				cout << "\n***********Transcript******************\n";
				viewTranscript();
				cout << endl;

			}
			else if (option == 6)// view attendance
			{

				viewAttendance();

			}
			else if (option == 7)//view registrations
			{
				view_Registrations();
				
			}
			cout << "\nEnter Option from Menu\n";
			cin >> option;


		}
		
	
	}
	int student::add_course(string c_code, char sec)
	{
		int c = sys->extract_id(c_code);
		int si = sys->get_sectionid(sec);

		int id = sys->register_course(c, si, *this);
		sys->AddStudenttoSection(*this, sec, c_code);
		if (id == 0)
		{
			Registration* r = new Registration(sys->get_course(c), sys->get_section(c, si), c);
			registrations.push_back(r);

			SAConnection con;

			SACommand select(&con, _TSA("select semcode from CourseinSem where semester =:1 and coursecode =:2"));

			select.Param(1).setAsString() = semester.c_str();
			select.Param(2).setAsString() = c_code.c_str();

			select.Execute();
			select.FetchFirst();
			int sem = select.Field(_TSA("semcode")).asInt64();

			SACommand insert(&con, _TSA("INSERT INTO Registration (courseCode, rollno, section, grade, semcode) VALUES (:1, :2, :3, NULL, :5)"));

			insert.Param(1).setAsString() = c_code.c_str();
			insert.Param(2).setAsString() = rollnum.c_str();
			insert.Param(3).setAsString() = sec;
			insert.Param(5).setAsInt64() = sem;

			try {
				con.Connect(_TSA("LAPTOP-MAPPIP6B@LMS"), _TSA("sa"), _TSA("password"), SA_SQLServer_Client);

				insert.Execute();
				insert.Close();

				con.Disconnect();
			}
			catch (SAException &x) {
				con.Rollback();
				printf("%s\n", x.ErrText().GetMultiByteChars());
			}
		}

		return id;
	}

	int student::drop_course(string c_code,char sec)
	{
		int c = sys->extract_id(c_code);
		int si = sys->get_sectionid(sec);
		int flag =sys->d_cour(c, si,*this);
		if (flag == true)
		{
			int ind = search_registrations(c_code);
			if (ind != -1)
			{
				registrations.erase(registrations.begin() + ind);
			}

			SAConnection con;

			SACommand insert(&con, _TSA("delete from Registration where rollno=:1 and courseCode=:2"));

			insert.Param(1).setAsString() = rollnum.c_str();
			insert.Param(2).setAsString() = c_code.c_str();

			try {
				con.Connect(_TSA("LAPTOP-MAPPIP6B@LMS"), _TSA("sa"), _TSA("password"), SA_SQLServer_Client);

				insert.Execute();
				insert.Close();

				con.Disconnect();
			}
			catch (SAException &x) {
				con.Rollback();
				printf("%s\n", x.ErrText().GetMultiByteChars());
			}
		}
		return flag;
	}
	bool student::withdraw_course(string c_code, char sec)
	{
		int c = sys->extract_id(c_code);
		int si = sys->get_sectionid(sec);
		bool flag = sys->with_course(c, si, *this);
		if (flag == true)
		{

			int ind = search_registrations(c_code);
			if (ind != -1)
			{
				registrations.erase(registrations.begin() + ind);
			}

			SAConnection con;

			SACommand insert(&con, _TSA("delete from Registration where rollno=:1 and courseCode=:2"));

			insert.Param(1).setAsString() = rollnum.c_str();
			insert.Param(2).setAsString() = c_code.c_str();

			try {
				con.Connect(_TSA("LAPTOP-MAPPIP6B@LMS"), _TSA("sa"), _TSA("password"), SA_SQLServer_Client);

				insert.Execute();
				insert.Close();

				con.Disconnect();
			}
			catch (SAException &x) {
				con.Rollback();
				printf("%s\n", x.ErrText().GetMultiByteChars());
			}

		}
		return flag;

	}
	int student:: search_registrations(string c_code)
	{
		int index = -1;
		for (int i = 0; i < registrations.size()&& index ==-1; i++)
		{
			if (*(registrations[i])==(c_code))//matching course code in registration object
			{
				index = i;
			}
		}
		return index;
	}

	void student::update(course *c,section*s)
	{
		Registration* r = new Registration(*c, *s, this->currentSem);
		registrations.push_back(r);
		
		add_course(c->get_title(), s->get_title());

	}

	bool student:: operator==(const student& s)
	{
		if (password == s.password)
			return true;
		else 
			return false;
	}
	void student::AddAttendance(Attendance& a, string code, char sec)
	{
		for (int i = 0; i < registrations.size(); i++)
		{
			if (registrations[i]->getcoursecode() == code)
			{
				registrations[i]->AddAttendance(a);

				SAConnection con;
				con.Connect("LAPTOP-MAPPIP6B@LMS", "sa", "password", SA_SQLServer_Client);

				SACommand cmd(
					&con,
					_TSA("insert into Attendance (ClassDay, letter, courseCode, rollno, section) values (:1, :2, :3, :4, :5)"));    // Add attendance

				string at, sect;
				at.push_back(a.getLetter());
				sect.push_back(sec);

				cmd.Param(1).setAsString() = a.getDate().c_str();
				cmd.Param(2).setAsString() = at.c_str();
				cmd.Param(3).setAsString() = code.c_str();
				cmd.Param(4).setAsString() = rollnum.c_str();
				cmd.Param(5).setAsString() = sect.c_str();

				cmd.Execute();

				cmd.Close();

			}
		}
	}
	void student:: AddEvaluation(Evaluation& e, string code, char sec)
	{
		for (int i = 0; i < registrations.size(); i++)
		{
			if (registrations[i]->getcoursecode() == code)
			{
				registrations[i]->AddEvaluation(e);

				SAConnection con;
				con.Connect("LAPTOP-MAPPIP6B@LMS", "sa", "password", SA_SQLServer_Client);

				SACommand cmd(
					&con,
					_TSA("insert into Marks (assignNo, weightage, totalmarks, marksObtained, courseCode, rollno, section) values (:1, :2, :3, :4, :5, :6, :7)"));    // Add evaluation
				string s;
				s.push_back(sec);
				cmd.Param(1).setAsInt64() = e.getassignmentNo();
				cmd.Param(2).setAsInt64() = e.getWeightage();
				cmd.Param(3).setAsInt64() = e.getTotalmarks();
				cmd.Param(4).setAsInt64() = e.getStudentMarks();
				cmd.Param(5).setAsString() = code.c_str();
				cmd.Param(6).setAsString() = rollnum.c_str();
				cmd.Param(7).setAsString() = s.c_str();

				cmd.Execute();

				cmd.Close();
			}
		}
	}
	void student::AddMarks(string code, int assign)
	{
		int index = search_registrations(code);
		registrations[index]->AddMarks(assign, rollnum);
	}
	void student::AssignGrades(string code)
	{
		int index = search_registrations(code);
		registrations[index]->AssignGrades(rollnum);
	}

	string student::getroll()
	{
		return rollnum;
	}

	void student ::viewMarksSheet(string code)
	{
		int index = search_registrations(code);
		registrations[index]->viewMarksSheet();
	}

	/*void student::AddTranscript()
	{
	}
*/
	void student::view_Registrations()
	{
		//just for showing output
		int n = registrations.size();
		if (n == 0)
		{
			cout << "\nNo registrations!\n";
		}
		for (int i = 0; i < n; i++)
		{
			registrations[i]->PrintRegistration();
		}

	}
	void student::viewTranscript()
	{
	
		for (int i = 0; i < registrations.size(); i++)
		{
			cout << registrations[i]->getcoursecode() << " ";
			cout << registrations[i]->getGrade() << " ";
			cout<<transcript->getGPA();
		}	
	}
	void student::viewAttendance()
	{
		for (int i = 0; i<registrations.size(); i++)
		{
			cout << registrations[i]->getcoursecode() << " ";
			registrations[i]->PrintAttendance();
			
		}
	}
	/*void student::viewMarks(string code)
	{
		for (int i = 0; i < registrations.size(); i++)
		{
			cout << registrations[i]->getcoursecode() << " ";
			registrations[i]->PrintAttendance();

		}
	}*/
	//void teacher::viewMarks(string code)
	//{
	//	SAConnection con;
	//	con.Connect("LAPTOP-MAPPIP6B@LMS", "sa", "password", SA_SQLServer_Client);

	//	SACommand cmd1(
	//		&con,
	//		_TSA("select M.assignNo, M.weightage, M.totalmarks, M.marksObtained"
	//			"from Marks M join Registration R on M.rollno = R.rollno AND M.courseCode = R.courseCode"
	//			"where R.courseCode = :1 and R.rollno = :2"));    // command object

	//	char* rollno = new char[9];
	//	char* c = new char[7];
	//	strcpy(rollno, rollnum.c_str());
	//	strcpy(c, code.c_str());

	//	cmd1.Param(1).setAsString() = c;
	//	cmd1.Param(2).setAsString() = rollno;

	//	//select from table
	//	cmd1.Execute();
	//	cout << "Marks for Course: " << c << endl;
	//	//fetch results row by row and print results
	//	while (cmd1.FetchNext())
	//	{

	//		printf("Assignment Number =  %d , weightage = %d , marksObtained = %d , Total Marks = %d \n",
	//			cmd1.Field(_TSA("assignNo")).asInt64(),
	//			cmd1.Field(_TSA("weightage")).asInt64(),
	//			cmd1.Field(_TSA("marksObtained")).asInt64(),
	//			cmd1.Field(_TSA("totalmarks")).asInt64()
	//		);

	//	}
	//	cmd1.Close();
	//	con.Disconnect();

	//}
	student::~student()
	{
		//will be empty 
	}


