#include"state.h"



	state::state(section &s)
	{
		sec = &s;
	}

	 state::~state()
	{
		if (sec != nullptr)
		{
			sec = nullptr;
		}
	}

	