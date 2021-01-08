
#include"section_full.h"

section_full::section_full(section& s):state(s)
{}
	int section_full :: register_section(student& s)
	{
		sec->add_student_in_waiting(s);
		return 2; //means student wasn't registered 
	}
	bool section_full::availability()
	{
		return true;
	}
	bool section_full::rem_course(student& s)
	{
		return sec->remove_student(s);
	}
	bool section_full::withdraw_course(student& s)
	{
		return false;
	}