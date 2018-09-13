#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include <windows.h>
#include <Xinput.h>
#include <mutex>

extern DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState);
extern "C" VOID WINAPI XInputEnable(BOOL enable);

static std::mutex my_mutex;

int main()
{
	std::cout << "MAIN" << std::endl;
	DWORD dwUserIndex = 0;
	XINPUT_STATE* pState = new XINPUT_STATE();

	XInputEnable(true);
	while (1)
	{
		my_mutex.lock();
		XInputGetState(dwUserIndex, pState);
		if (pState != nullptr)
		{
			std::cout << "GamePad" << pState->Gamepad.wButtons << std::endl;
		}
		else
		{
			std::cout << "No pState" << std::endl;
		}
		my_mutex.unlock();

		//std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return 0;
}