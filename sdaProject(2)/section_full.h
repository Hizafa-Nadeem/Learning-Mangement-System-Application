#pragma once
#include"student.h"
#include"state.h"
class section_full : public state
{
public:
	section_full(section& s);
	int register_section(student &s);

	bool availability();

	bool rem_course(student& s);
	bool withdraw_course(student& s);

};
