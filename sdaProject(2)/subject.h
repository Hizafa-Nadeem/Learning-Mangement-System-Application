#include"observer.h"
#include<queue>
using namespace std;

class subject
{
protected:
	queue<observer*> observers;
public:
	virtual void notify() = 0;
};