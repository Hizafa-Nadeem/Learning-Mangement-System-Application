
#include"withdraw_close.h"
withdraw_close::withdraw_close(section& s) :state(s)
{}
bool withdraw_close::withdraw_course(student& s)
{
	return false;
}
bool withdraw_close::availability() // for displaying offered courses
{
	return false;
}
int withdraw_close::register_section(student& s)
{
	return 1;
}
bool withdraw_close::rem_course(student& s)
{
	return false;
}
