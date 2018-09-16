#pragma comment (lib,"dxguid")
#pragma comment (lib,"dinput8")

#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <string>

#include <windows.h>
#include <Xinput.h> // window.h should be before
#include <dinput.h>


extern DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState);

bool GUIDtoString(std::string* out, const GUID &g)
{
	std::unique_ptr<char[]> buffer(new char[40]);
	sprintf_s(buffer.get(), 40, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		g.Data1, g.Data2, g.Data3, g.Data4[0], g.Data4[1], g.Data4[2], g.Data4[3], g.Data4[4], g.Data4[5], g.Data4[6], g.Data4[7]);

	*out = buffer.get();
	return !out->empty();
}


// http://www.rastertek.com/dx11tut13.html
// https://www.gamedev.net/forums/topic/668528-idirectinput8enumdevices-bug/
// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ee417804(v%3dvs.85)
BOOL CALLBACK EnumFFDevicesCallback(const DIDEVICEINSTANCE* pInst, VOID* pContext)
{
	std::string guidProductStr;
	GUIDtoString(&guidProductStr, pInst->guidProduct);

	std::cout << pInst->tszProductName << " guidProduct" << guidProductStr.c_str() << std::endl;

	return DIENUM_CONTINUE;
}

int main()
{
	HRESULT result;
	DWORD dwUserIndex = 0;
	XINPUT_STATE* pState = new XINPUT_STATE();

	// Init DirectInput
	IDirectInput8* m_directInput = 0;
	// Initialize the main direct input interface.
	result = DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return 1;
	}
	
	while (1)
	{
		XInputGetState(dwUserIndex, pState);
		if (pState != nullptr)
		{
			std::cout << "GamePad >> " << pState->Gamepad.wButtons << std::endl;
		}
		else
		{
			std::cout << "No pState" << std::endl;
		}

		// Look for a force feedback device we can use 
		result = m_directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumFFDevicesCallback, (void*)1, DIEDFL_ALLDEVICES);
		if (FAILED(result))
		{
			return 1;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}


	m_directInput->Release();

	return 0;
}