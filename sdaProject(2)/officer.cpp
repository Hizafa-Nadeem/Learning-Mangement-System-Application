#include"officer.h"
#include "SQLAPI.h"
#include "teacher.h"


	academic_officer::academic_officer()
	{}

	academic_officer::academic_officer(string n, string pass, string r, System &s) :person(n, pass)
	{
		rank = r;
		sys = &s;
	}
	person* academic_officer::login(string n, string pass)
	{
		person* p = &(*this);
		if (name == n && password == pass)
		{
			return p;
		}
		else
			return nullptr;
	}
	void academic_officer::print()
	{
		cout << " \n Academic Officer\n";
		cout << " -----------------\n";
		cout << " Name: " << name;
		cout << " Rank: " << rank;
	}
	void academic_officer::display_menu()
	{
		cout << "\n*******Menu **********\n1.Add Course Press 1\n2.Add teacher Press 2\n3.Add Student Press 3\n4.Add Section Press 4\n5.Open Registration Press 5 \n6.Close Registration Press 6\n7.Open withdrawal period Press 7 \n8.Close withdrawal period Press 8 \n9.To terminate the session Press 0\n";
	}

	int academic_officer::add_course(string code, string title, int credit_hrs, vector<string> sems, int semOffered, int semcode)
	{
		course c(code, title, credit_hrs, sems, semOffered);
		sys->courses.push_back(c); //course is added n all course list
		
		//sys->persons[0]->display_menu();
		//sys->courses.push_back(c); //course is added in all course list


		SAConnection con; // connection object

		SACommand cmd(
			&con,
			_TSA("insert into Course(courseTitle, code, credHrs, OfferedtoSem) values (:1, :2, :3, :4)"));    // command object

		//int length = title.length() + 1;
		char t[200];
		char cCode[7];

		strcpy(t, title.c_str());
		strcpy(cCode, code.c_str());

		cmd.Param(1).setAsString() = t;
		cmd.Param(2).setAsString() = cCode;
		cmd.Param(3).setAsInt64() = credit_hrs;
		cmd.Param(4).setAsInt64() = semOffered;

		SACommand cmd1(
			&con,
			_TSA("select * from Course"));    // command object

		SACommand cmd2(
			&con,
			_TSA("insert into CourseinSem values(:1, :2, :3)"));    // command object


		try
		{
			// connect to database (Oracle in our example)
			con.Connect("LAPTOP-MAPPIP6B@LMS", "sa", "password", SA_SQLServer_Client);

			// Select from our test table
			cmd.Execute();
			cmd.Close();

			cmd1.Execute();
			// fetch results row by row and print results
			while (cmd1.FetchNext())
			{
				printf("Course Title =  %s, code = %s credit Hrs = %d Semester Offered = %d\n",
					cmd1.Field(_TSA("courseTitle")).asString().GetMultiByteChars(),
					cmd1.Field(_TSA("code")).asString().GetMultiByteChars(),
					cmd1.Field(_TSA("credHrs")).asInt64(),
					cmd1.Field(_TSA("OfferedtoSem")).asInt64()
				);
			}

			// commit changes on success
			con.Commit();
		}
		catch (SAException &x)
		{
			// SAConnection::Rollback()
			// can also throw an exception
			// (if a network error for example),
			// we will be ready
			try
			{
				// on error rollback changes
				con.Rollback();
			}
			catch (SAException &)
			{
			}
			// print error message
			printf("%s\n", x.ErrText().GetMultiByteChars());
		}

		try
		{
			//// connect to database (Oracle in our example)
			//con.Connect("LAPTOP-MAPPIP6B@LMS", "sa", "password", SA_SQLServer_Client);

			for (int i = 0; i < sems.size(); i++)
			{
				char temp[20];
				strcpy(temp, sems[i].c_str());
				cmd2.Param(1).setAsInt64() = semcode;
				cmd2.Param(2).setAsString() = cCode;
				cmd2.Param(3).setAsString() = temp;

				cmd2.Execute();
				cmd2.Close();
			}
			
			// commit changes on success
			con.Commit();
		}
		catch (SAException &x)
		{
			// SAConnection::Rollback()
			// can also throw an exception
			// (if a network error for example),
			// we will be ready
			try
			{
				// on error rollback changes
				con.Rollback();
			}
			catch (SAException &)
			{
			}
			// print error message
			printf("%s\n", x.ErrText().GetMultiByteChars());
		}

		cout << "Course successfully added." << endl;

		return 0;
	}
	void academic_officer::add_teacher(string name, string desig, string uname, string pass, int pay)
	{
		teacher* t=new teacher(name, desig, uname, pass, pay, sys);
		sys->persons.push_back(t);


		SAConnection con; // connection object

		SACommand cmd(
			&con,
			_TSA("insert into Teacher(uname, Name, Designation, payGrade) values (:1, :2, :3, :4)"));    // command object

		SACommand cmd2(
			&con,
			_TSA("insert into Person(username, password) values (:1, :2)"));    // command object
		
		char n[200];
		char d[200];
		char username[200];
		char password[30];

		strcpy(n, name.c_str());
		strcpy(d, desig.c_str());
		strcpy(username, uname.c_str());
		strcpy(password, pass.c_str());

		cmd.Param(1).setAsString() = username;
		cmd.Param(2).setAsString() = n;
		cmd.Param(3).setAsString() = d;
		cmd.Param(4).setAsInt64()=pay;

		cmd2.Param(1).setAsString() = username;
		cmd2.Param(2).setAsString() = password;

		SACommand cmd1(
			&con,
			_TSA("select * from Teacher"));    // command object

		try
		{
			// connect to database 
			con.Connect("LAPTOP-MAPPIP6B@LMS", "sa", "password", SA_SQLServer_Client);

			// Select from our table
			cmd.Execute();
			cmd.Close();

			cmd2.Execute();
			cmd2.Close();

			cmd1.Execute();
			// fetch results row by row and print results
			while (cmd1.FetchNext())
			{
				printf("Username:  %s, Teacher Name = %s Designation = %s Pay Grade = %d \n",
					cmd1.Field(_TSA("uname")).asString().GetMultiByteChars(),
					cmd1.Field(_TSA("Name")).asString().GetMultiByteChars(),
					cmd1.Field(_TSA("Designation")).asString().GetMultiByteChars(),
					cmd1.Field(_TSA("payGrade")).asInt64()
				);
			}

			// commit changes on success
			cout << "Teacher successfully added." << endl;
			con.Commit();
		}
		catch (SAException &x)
		{
			// SAConnection::Rollback()
			// can also throw an exception
			// (if a network error for example),
			// we will be ready
			try
			{
				// on error rollback changes
				con.Rollback();
			}
			catch (SAException &)
			{
			}
			// print error message
			printf("%s\n", x.ErrText().GetMultiByteChars());
		}

		
	}
	void academic_officer::add_student(string n, string pass, string rollno, string prog, string u, string sem, int csem)
	{
		student* s = new student(n, rollno, u, pass, prog, sem, csem, sys);
		sys->persons.push_back(s);


		SAConnection con; // connection object

		SACommand cmd(
			&con,
			_TSA("insert into Student(name, rollno, program, uname, semester, currSem) values (:1, :2, :3, :4, :5, :6)"));    // command object

		SACommand cmd2(
			&con,
			_TSA("insert into Person(username, password) values (:1, :2)"));    // command object

		char t[200];
		char roll[9];
		char pr[200];
		char uname[200];
		char password[30];
		char semester[21];

		strcpy(t, n.c_str());
		strcpy(roll, rollno.c_str());
		strcpy(pr, prog.c_str());
		strcpy(uname, u.c_str());
		strcpy(password, pass.c_str());
		strcpy(semester, sem.c_str());

		cmd.Param(1).setAsString() = t;
		cmd.Param(2).setAsString() = roll;
		cmd.Param(3).setAsString() = pr;
		cmd.Param(4).setAsString() = uname;
		cmd.Param(5).setAsString() = semester;
		cmd.Param(6).setAsInt64() = csem;

		cmd2.Param(1).setAsString() = uname;
		cmd2.Param(2).setAsString() = password;

		SACommand cmd1(
			&con,
			_TSA("select * from Student"));    // command object

		try
		{
			// connect to database 
			con.Connect("LAPTOP-MAPPIP6B@LMS", "sa", "password", SA_SQLServer_Client);

			// Select from our table
			cmd2.Execute();
			cmd2.Close();

			cmd.Execute();
			cmd.Close();

			cmd1.Execute();
			// fetch results row by row and print results
			while (cmd1.FetchNext())
			{
				printf("Student Name:  %s, Roll number = %s Program = %s Username = %s Semester in = %s\n",
					cmd1.Field(_TSA("name")).asString().GetMultiByteChars(),
					cmd1.Field(_TSA("rollno")).asString().GetMultiByteChars(),
					cmd1.Field(_TSA("program")).asString().GetMultiByteChars(),
					cmd1.Field(_TSA("uname")).asString().GetMultiByteChars(),
					cmd1.Field(_TSA("semester")).asString().GetMultiByteChars()
				);
			}

			// commit changes on success
			cout << "Student successfully added." << endl;
			con.Commit();
		}
		catch (SAException &x)
		{
			// SAConnection::Rollback()
			// can also throw an exception
			// (if a network error for example),
			// we will be ready
			try
			{
				// on error rollback changes
				con.Rollback();
			}
			catch (SAException &)
			{
			}
			// print error message
			printf("%s\n", x.ErrText().GetMultiByteChars());
		}

	}
	int academic_officer::add_section(char name, string room, int t_seats, string course_code)
	{
		section s(name, room, t_seats, &(sys->courses[sys->extract_id(course_code)-1])); // reference of the course is provided
		sys->courses[sys->extract_id(course_code)-1].add_section(s); // section is added in courses present in the system

		SAConnection con; // connection object

		SACommand cmd(
			&con,
			_TSA("insert into Section(section, coursecode, room, totalseats, regState) values (:1, :2, :3, :4, :5)"));    // command object


		char c[7];
		char r[50];

		strcpy(c, course_code.c_str());
		strcpy(r, room.c_str());

		cmd.Param(1).setAsString() = name;
		cmd.Param(2).setAsString() = c;
		cmd.Param(3).setAsString() = r;
		cmd.Param(4).setAsInt64() = t_seats;
		cmd.Param(5).setAsInt64() = 1;

		try
		{
			// connect to database 
			con.Connect("LAPTOP-MAPPIP6B@LMS", "sa", "password", SA_SQLServer_Client);


			cmd.Execute();
			cmd.Close();

			// commit changes on success
			cout << "Section successfully added." << endl;
			con.Commit();
		}
		catch (SAException &x)
		{
			// SAConnection::Rollback()
			// can also throw an exception
			// (if a network error for example),
			// we will be ready
			try
			{
				// on error rollback changes
				con.Rollback();
			}
			catch (SAException &)
			{
			}
			// print error message
			printf("%s\n", x.ErrText().GetMultiByteChars());
		}

		return 0;
	}
	void academic_officer::open_registration(string course)
	{
		sys->courses[sys->extract_id(course) - 1].open_registration();

	}
	void academic_officer::close_registration(string course)
	{
		sys->courses[sys->extract_id(course) - 1].close_registration();
	}
	void academic_officer::open_withdraw(string course)
	{
		sys->courses[sys->extract_id(course) - 1].open_withdraw();
	}
	void academic_officer::close_withdraw(string course)
	{
		sys->courses[sys->extract_id(course) - 1].close_withdraw();
	}
	void academic_officer::display_Functionalities(int option)
	{
		int  credit, t_seats;
		char name;
		char title[200 + 1];
		char code[7];
		char s[20];
		string program, room, c, c_code, rollno, u;

		while (option != 0)
		{

			//academic_officer* ac = dynamic_cast<academic_officer*>(person);//returns pointer of type academic officer

			if (option == 1)
			{
				int OfferedinSem, semcode;
				cout << "Enter course title: " << endl;
				cin.ignore();
				cin.getline(title, 200, '\n');
				cout << "Enter credit hours: " << endl;
				cin >> credit;
				cin.ignore();
				cout << "Semesters taught: ";
				cin.getline(s, 19, '\n');
				cout << "Enter Course Code: ";
				//cin.ignore();
				cin.getline(code, 7, '\n');

				cout << "Enter the semester of degree in which course offered to students: ";
				cin >> OfferedinSem;
				cout << "Enter the semester code: ";
				cin >> semcode;

				string sCode(code);
				vector <string> semesters;
				string sem(s);
				semesters.push_back(sem);

				
				string c = sys->generatecoursecode();
				if (add_course(sCode, title, credit, semesters, OfferedinSem, semcode) == 0)
				{
					cout << "\nCourse has been added sucessfully! Course Code is " << c << endl;
				}

			}
			else if (option == 2)
			{
				char n[200], pass[30], username[200], desig[200];
				int pay;

				cout << "Enter name of Teacher: ";
				cin.ignore();
				cin.getline(n, 200, '\n');
				cout << "\nEnter designation of Teacher: ";
				//cin.ignore();
				cin.getline(desig, 200, '\n');
				cout << "\nEnter pay grade of teacher: ";
				cin >> pay;
				cout << "\nEnter username of teacher: ";
				cin.ignore();
				cin.getline(username, 200, '\n');
				cout << "\nEnter password of teacher: "<<endl;
				//cin.ignore();
				cin.getline(pass, 30, '\n');

				//string pa = sys->getpassword(Teacher);
				string title(n), password(pass), u(username), d(desig);

				add_teacher(title, desig, username, password, pay);
				
			}
			else if (option == 3)
			{
				char n[200], p[200], r[200], username[200], sem[21], pass[30];
				int csem;

				cout << "Enter name of Student: ";
				cin.ignore();
				cin.getline(n, 200, '\n');
				cout << "\nEnter Program in which student enrolled!";
				//cin.ignore();
				cin.getline(p, 200, '\n');
				cout << "\nEnter Roll number of student: ";
				//cin.ignore();
				cin.getline(r, 200, '\n');
				cout << "\nEnter username of student: ";
				//cin.ignore();
				cin.getline(username, 200, '\n');
				cout << "\nEnter password for student: ";
				//cin.ignore();
				cin.getline(pass, 30, '\n');
				cout << "\nEnter description for ongoing semester: ";
				//cin.ignore();
				cin.getline(sem, 20, '\n');
				cout << "\nEnter the number semester the student is currently in: ";
				//cin.ignore();
				cin >> csem;

				//string pass = sys->getpassword(Student);
				string title(n), rollno(r), program(p), u(username), semester(sem), password(pass);

				add_student(title, password, rollno, program, u, semester, csem);
				
			}
			else if (option == 4)
			{
				cout << "\nNote: first add section A then B then C and so on in sequence!\n";
				cout << "Enter Section Title: ";
				cin >> name;
				cout << "Enter Course code for which section is added : "<<endl;
				cin >> c_code;
				cout << "Enter Room for the section: "<<endl;
				cin >> room;
				cout << "Enter total  seats for the section: ";
				cin >> t_seats;

				add_section(name, room, t_seats, c_code);
				cout << "\nSection has been added successfully:\n";

			}
			else if (option == 5)
			{
				// open registration
				cout << "\nEnter course code: ";
				cin >> c_code;
				open_registration(c_code);
				cout << "\nRegistration opened for course" << c_code << endl;//close registration
			}
			else if (option == 6)
			{
				cout << "\nEnter course code (CS1): ";
				cin >> c_code;
				close_registration(c_code);
				cout << "\nRegistration closed for course" << c_code << endl;//close registration
			}
			else if (option == 7)
			{
			cout << "\nEnter course code (CS1): ";
			cin >> c_code;
			open_withdraw(c_code);
			cout << "\nWithdrawal open for course" << c_code << endl;//close registration
			}
			else if (option == 8)
			{
			cout << "\nEnter course code (CS1): ";
			cin >> c_code;
			close_withdraw(c_code);
			cout << "\nWithdrawal closed for course" << c_code << endl;//close registration
			}


			cout << "\nEnter Option from Menu: \n";
			cin >> option;

		}
		
	}

