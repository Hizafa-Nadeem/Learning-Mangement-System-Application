#ifndef observer_HEADER
#define observer_HEADER
#include"course.h"
class observer
{
public:
	virtual void update(course* c, section* s) = 0;
	observer();
};
#endif