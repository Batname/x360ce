#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include <windows.h>
#include <Xinput.h>

extern DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState);

int main()
{
	std::cout << "MAIN" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	return 0;
}