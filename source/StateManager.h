#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_ 

struct StateManager
{
	// Struct members
	StateManager()
	{
	
	}

	HWND mHwnd;


	StateManager operator=(StateManager const& other)
	{
		StateManager res; 
		res.mHwnd = other.mHwnd; 
		return res;
	}
};

#endif 