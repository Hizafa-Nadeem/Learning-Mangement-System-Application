#include"withdraw_open.h"

withdraw_open::withdraw_open(section& s):state(s)
{

}
bool withdraw_open::withdraw_course(student& s)
{
	return sec->remove_student(s);
}
bool withdraw_open::availability() // for displaying offered courses
{
	return false;
}
int withdraw_open::register_section(student& s)
{
	return 1;
}
bool withdraw_open::rem_course(student& s)
{
	return false;
}