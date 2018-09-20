#include "UE4x360ce.h"

#include "XInputModuleManager.h"
#include "Utils.h"

UE4x360ce* UE4x360ce::self = nullptr;
ControllerManager* ControllerManager::ControllerManagerInst = nullptr;

UE4x360ce::UE4x360ce()
	: bIsInputLoopRunning(false)
	, nDevices(0)
	, MaxControlelrs(4)
	, dwUserIndex(0)
	, pState(new XINPUT_STATE())
	, m_directInput(nullptr)
{
	std::cout << "UE4x360ce" << std::endl;

	if (UE4x360ce::self == nullptr)
	{
		UE4x360ce::self = this;
	}
}

UE4x360ce::~UE4x360ce()
{
	SAFE_DELETE(pState);
}

DWORD UE4x360ce::XInputGetState(DWORD dwUserIndex, XINPUT_STATE * pState)
{
	ControllerBase* pController;
	if (!pState)
		return ERROR_BAD_ARGUMENTS;
	u32 initFlag = ControllerManager::Get()->DeviceInitialize(dwUserIndex, &pController);
	if (initFlag != ERROR_SUCCESS)
		return initFlag;

	return pController->GetState(pState);
}

int UE4x360ce::Run()
{
	result = DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return 1;
	}

	if (bIsInputLoopRunning == false)
	{
		bIsInputLoopRunning = true;

		TInputLoop = thread([&]() {
			InputThreadLoop();
		});
	}

	return 1;
}

int UE4x360ce::Stop()
{
	if (bIsInputLoopRunning == true)
	{
		bIsInputLoopRunning = false;

		TInputLoop.join();
	}

	return 1;
}

int UE4x360ce::InputThreadLoop()
{
	ControllerManager::ControllerManagerInst = new ControllerManager();
	ControllerManager::ControllerManagerInst->Init();

	while (bIsInputLoopRunning)
	{
		UINT nDevicesLocal = 0;

		// Update devices only on start or if plug un-plug during the game
		GetRawInputDeviceList(NULL, &nDevicesLocal, sizeof(RAWINPUTDEVICELIST));
		if (nDevicesLocal != nDevices)
		{
			// Reset variables
			nDevices = nDevicesLocal;
			TMutex.lock();
			ue4x360ceFrame.ControllersCount = 0;
			TMutex.unlock();

			// Look for a force feedback device we can use 
			result = m_directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, UE4x360ce::EnumFFDevicesCallback, (void*)1, DIEDFL_ALLDEVICES);
			if (FAILED(result))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
				continue;
			}
		}

		// recieve input from all controlelrs
		for (int i = 0; i < ue4x360ceFrame.ControllersCount; ++i)
		{
			XInputGetState(i, pState);
			if (pState != nullptr)
			{
				if (pState->Gamepad.wButtons)
				{
					std::cout << "GamePad >> " << pState->Gamepad.wButtons << std::endl;
				}
			}
			else
			{
				std::cout << "No pState" << std::endl;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	return 1;
}

BOOL CALLBACK UE4x360ce::EnumFFDevicesCallback(const DIDEVICEINSTANCE* pInst, VOID* pContext)
{
	std::string guidProductStr;
	GUIDtoString(&guidProductStr, pInst->guidProduct);

	std::cout << pInst->tszProductName << " guidProduct" << guidProductStr.c_str() << std::endl;

	if (UE4x360ce::self->ue4x360ceFrame.ControllersCount < UE4x360ce::self->MaxControlelrs)
	{
		UE4x360ce::self->TMutex.lock();
		UE4x360ce::self->ue4x360ceFrame.ControllersCount++;
		UE4x360ce::self->TMutex.unlock();
	}

	return DIENUM_CONTINUE;
}