#pragma once
#include"state.h"
#include"student.h"
class register_open :public state
{
public:
	register_open(section& s);
	int register_section(student& s);

	bool availability();


	bool rem_course(student& s);
	bool withdraw_course(student& s);
	

};

