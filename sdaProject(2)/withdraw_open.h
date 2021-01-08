#pragma once
#include "state.h"
class withdraw_open: public state
{

public:
	withdraw_open(section& s);
	bool withdraw_course(student& s);

	bool availability();

	int register_section(student& s);

	bool rem_course(student& s);

};