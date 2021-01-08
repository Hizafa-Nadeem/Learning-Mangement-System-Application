#pragma once
#include"state.h"

class withdraw_close: public state
{
public:
	withdraw_close(section& s);
	bool withdraw_course(student& s);

	bool availability();

	int register_section(student& s);

	bool rem_course(student& s);
	
};

