#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include <windows.h>
#include <Xinput.h>

extern DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState);
extern "C" VOID WINAPI XInputEnable(BOOL enable);

int main()
{
	DWORD dwUserIndex = 0;
	XINPUT_STATE* pState = new XINPUT_STATE();
	//~~~~~~~~~~~~~~~~~~~ Update DLL form


	//~~~~~~~~~~~~~~~~~~~ Loop throught all devices
	// DirectInput in main app // C:\Users\denys.dubinin\AppData\Local\Temp\MetadataAsSource\01a98b802bf74db5b490cf94468ea36a\559446ddfdc948d5bc81210adf40afd9\DirectInput.cs
	// NewDeviceForm if new device detected
	// x360ce.App MainForm::RefreshCurrentInstances -> UpdateForm3 -> UpdateTimer_Elapsed
	// DefaultPoolingInterval 50 ms
	// DeviceInstance[] GetDevices()
	// // Must find better way to find Device than by Vendor ID and Product ID.
	//	var devs = DeviceDetector.GetDevices(classGuid, DIGCF.DIGCF_ALLCLASSES, null, j.Properties.VendorId, j.Properties.ProductId, 0);

	// ~~~~~~~~~~~~~~~~~~ Try to find device config, othervice load from web
	// forceRecountDevices = true by default, thet is why it load on the beginning and set false after first load try
	// detector_DeviceChanged
	// UpdateForm1() -> new DeviceDetector.DeviceDetectorEventHandler(detector_DeviceChanged);
	// public void WndProc(ref Message m)
	/**
			/// <summary>
		/// This function receives all the windows messages for this window (form).
		/// We call the DeviceDetector from here so that is can pick up the messages about
		/// drives arrived and removed.
		/// </summary>
		protected override void WndProc(ref Message m)
	*/

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

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	return 0;
}