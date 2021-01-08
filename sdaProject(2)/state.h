#pragma once
#include"student.h"
#include"section.h"

class state
{
protected:
	section* sec;
public:
	state(section &s);
	virtual int register_section(student &s)=0;
	virtual bool availability()= 0;
	virtual bool rem_course(student& s) = 0;
	virtual bool withdraw_course(student& s) = 0;
	~state();
};