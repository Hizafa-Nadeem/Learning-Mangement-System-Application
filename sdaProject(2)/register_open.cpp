#include"register_open.h"

	register_open::register_open(section &s) :state(s)
	{}
	int register_open::register_section(student &s)
	{
		return sec->register_student(s);
	}
	bool register_open::availability()
	{
		return true;
	} 
	bool register_open::rem_course(student& s)
	{
		return sec->remove_student(s);
	}

	bool register_open::withdraw_course(student& s)
	{
		return false;
	}
	

