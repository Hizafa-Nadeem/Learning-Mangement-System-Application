#include<iostream>
#include<vector>
#include<string>
#pragma once
#include"person.h"
#include"System.h"
#include"course.h"
#include"officer.h"
#include"register_close.h"
#include"register_open.h"
#include"section.h"
#include"section_full.h"
#include"state.h"
#include"student.h"
#include"teacher.h"
#include"observer.h"
using namespace std;

int main()
{

	System sys_tem;
	int option;
	while (1)
	{
		cout << "Press 1 for Sign up\nPress 2 for Login !\nPress 0 to terminate session" << endl;
		cin >> option;

		string n, pass, r;
		if (option == 0)
			return 0;
		else if (option == 1)
		{

			cout << "\nSign up Academic officer\n";
			cout << "Enter username: ";
			cin >> n;
			cout << "Enter rank  ";
			cin >> r;
			pass = sys_tem.getpassword(Officer); // system will provide recommended password when user wants to sign up as academic officer. 
			cout << "\nYour password is " << pass;
			sys_tem.signup_academicofficer(n, pass, r);

		}
		else if (option == 2)
		{

			person* p = nullptr;

			cout << "\nLOGIN \n";
			cout << "Enter username: ";
			cin >> n;
			cout << "Enter password: ";
			cin >> pass;

			p = sys_tem.turn_on_session(n, pass); // system will on the session of the person which is logged in.
			if (p == nullptr)
			{
				cout << "\nIncorrect Credentials!\n";
			}
			else
			{
				cout << "\n\nYou have been successfully logged in  !\n";
				p->print();
				p->display_menu();// who will identify that which person is logged in
				cout << "Enter Option from menu ";
				cin >> option;
				p->display_Functionalities(option);//system will display functionalities according to the person 

			}

		}
		else if (option == 3)
			return 0;
	}
}