#pragma once
#include"register_close.h"


	register_close::register_close(section &s) :state(s)
	{}
	int register_close:: register_section(student& s)
	{
		return 1; //means student wasn't registered 
	}
	bool register_close::availability() // for displaying offered courses
	{
		return false;
	}
	bool register_close::rem_course(student& s)
	{
		return false;
	}

	bool register_close::withdraw_course(student& s)
	{
		return false;
	}

