#include <iostream>
#include <vector>

// WinApi
#include <windows.h> 

// DirectX
#pragma warning(push) 
#pragma warning(disable:6000 28251) 
#include <dinput.h> 
#pragma warning(pop)

#include <xinput.h>

using namespace std;

int main()
{
	vector<XINPUT_STATE*> pStates;

	pStates.clear();
	pStates.resize(2);
	pStates[1] = nullptr;
	pStates[0] = nullptr;


	Sleep(10000);

	return 0;
}