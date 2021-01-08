#pragma once
#include"state.h"
#include"section.h"
#include"student.h"
class register_close :public state
{
public:
		register_close(section& s);
		int register_section(student& s);
		bool availability();
		bool rem_course(student& s);
		bool withdraw_course(student& s);
		
};
