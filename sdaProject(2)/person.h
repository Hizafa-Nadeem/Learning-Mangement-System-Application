#include <string>
#include<iostream>
using namespace std;
#pragma once
class person
{
protected:
	string name;
	string password;
public:
	person();
	person(string n, string pass);
	virtual person* login(string n, string pass)=0;
	virtual void print()=0;
	virtual void display_menu()=0;
	virtual void display_Functionalities(int option)=0;
};
