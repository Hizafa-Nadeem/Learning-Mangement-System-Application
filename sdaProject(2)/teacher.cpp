#include"teacher.h"

teacher::teacher(string name, string designation, string username, string password, int paygrade, System* s)
	:name(name), designation(designation), username(username), password(password), paygrade(paygrade), sys(s)
{
}
	person* teacher::login(string n, string pass)
	{
		person* p = &(*this);
		if (username == n && password == pass)
		{
			return p;
		}
		else
			return nullptr;
	}

	void teacher::print()
	{
		cout << " \nTeacher ";
		cout << "Name: " << name << " ";
		cout << "Designation: " << designation << " ";
		cout << "Username: " << username << " ";
		cout << "Pay Grade: " << paygrade << endl;

	}
	void teacher:: display_menu()
	{
		cout << "\n*******Menu **********\n1.Add Attendance Press 1\n2. Add Evaluation Press 2\n3. Add Marks  Press 3\n4. Views Marks Sheet Press 4\n5. Assign Grades Press 5\n6.To terminate the session Press 0\n" << endl;
	}
	void teacher::setAttendance(string code, char sec)
	{
		sys->AddAttendance(code, sec);
	}
	void teacher::addEvaluation(string code, char sec, int w, int assign, int totalmarks)
	{
		sys->AddEvaluation(code, sec, w, assign, totalmarks);
	}
	void teacher::addMarks(string code, char sec, int assign)
	{
		sys->AddMarks(code, sec, assign);
	}
	void teacher::assigngrades(string code, char sec)
	{
		sys->AssignGrades(code, sec);
	}
	void teacher::viewMarksSheet(string code, char sec)
	{
		sys->viewMarksSheet(code, sec);
	}
	void teacher::display_Functionalities(int option)
	{
		char sec;
		string c_code;
		while (option != 0)
		{
			if (option == 1)//Add Attendance
			{
				char coursecode[9], sect[2];
				cout << "\nEnter the Course for which you want to set attendance: ";
				cin.ignore();
				cin.getline(coursecode, 8, '\n');
				cout << "\nEnter the section of the course for which you want to set attendance: ";
				cin.ignore();
				cin.getline(sect, 1, '\n');
				string code(coursecode);
				setAttendance(code, sect[0]);
			}
			else if (option == 2)//Add Evaluation
			{
				int assignment, total, w;
				char coursecode[9];
				cout << "\nEnter Course code: ";
				cin.ignore();
				cin.getline(coursecode, 8, '\n');
				cout << "\nEnter section name: ";
				cin >> sec;
				cout << "\nEnter assignment number: ";
				cin >> assignment;
				cout << "\nEnter weightage of assignment: ";
				cin >> w;
				cout << "\nEnter Total marks for assignment: ";
				cin >> total;
				string code(coursecode);
				addEvaluation(code, sec, w, assignment, total);

				cout << "\nEvaluation added successfully." << endl;
			}
			else if (option == 3)//Add Marks
			{
				int assignment;
				char coursecode[9];
				cout << "\nEnter Course code: ";
				cin.ignore();
				cin.getline(coursecode, 8, '\n');
				cout << "\nEnter section name: ";
				cin >> sec;
				cout << "\nEnter assignment number: ";
				cin >> assignment;

				string code(coursecode);
				addMarks(code, sec, assignment);
			}
			else if (option == 4)//View Marks Sheet
			{
				char coursecode[9];
				cout << "\nEnter Course code: ";
				cin.ignore();
				cin.getline(coursecode, 8, '\n');
				cout << "\nEnter section name: ";
				cin >> sec;

				string code(coursecode);
				viewMarksSheet(code, sec);
			}
			else if (option == 5)//Assign Grades
			{
				char coursecode[9];
				cout << "\nEnter Course code: ";
				cin.ignore();
				cin.getline(coursecode, 8, '\n');
				cout << "\nEnter section name: ";
				cin >> sec;

				string code(coursecode);
				assigngrades(code, sec);

			}
			cout << "\nEnter Option from Menu\n";
			cin >> option;

		}


	}




