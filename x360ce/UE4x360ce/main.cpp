#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include <windows.h>
#include <Xinput.h>

extern DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState);
extern "C" VOID WINAPI XInputEnable(BOOL enable);

int main()
{
	std::cout << "MAIN" << std::endl;
	DWORD dwUserIndex = 0;
	XINPUT_STATE* pState = new XINPUT_STATE();

	while (1)
	{
		XInputGetState(dwUserIndex, pState);
		if (pState != nullptr)
		{
			std::cout << "GamePad" << pState->Gamepad.wButtons << std::endl;
		}
		else
		{
			std::cout << "No pState" << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return 0;
}