#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include <windows.h>
#include <Xinput.h>
#include <sstream>
#include <dbt.h>

extern DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState);
extern "C" VOID WINAPI XInputEnable(BOOL enable);
int messageloop();

static const GUID interfaceClassGuid = { 0x53f56307, 0xb6bf, 0x11d0,{ 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } };

LRESULT CALLBACK WndProc(HWND hwnd,    //Default windows procedure
	UINT msg,
	WPARAM wParam,
	LPARAM lParam) 
{
	switch (msg)    //Check message
	{

	case WM_CREATE:
	{
		DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
		ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
		NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
		NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
		NotificationFilter.dbcc_classguid = interfaceClassGuid;

		HDEVNOTIFY hDeviceNotify = RegisterDeviceNotification(hwnd, &NotificationFilter, DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);

		if (hDeviceNotify == NULL)
			printf("hDeviceNotify = NULL \n");
	}
	return 0;

	case WM_KEYDOWN:    //For a key down
						//if escape key was pressed, display popup box
		if (wParam == VK_ESCAPE) {
			if (MessageBox(0, "Are you sure you want to exit?",
				"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)

				//Release the windows allocated memory  
				DestroyWindow(hwnd);
		}
		return 0;

	case WM_DEVICECHANGE: {     
		PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;

		switch (wParam) {
			case DBT_DEVICEARRIVAL:
				MessageBox(hwnd, "A device has been inserted.", "USB Notice", MB_OK);
				break;
			case DBT_DEVICEREMOVECOMPLETE:
				MessageBox(hwnd, "A device has been removed.", "USB Notice", MB_OK);
				break;
		}

		return 0;
	}

	case WM_DESTROY:    //if x button in top right was pressed
		PostQuitMessage(0);
		return 0;
	}
	//return the message for windows to handle it
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}

LPCTSTR WndClassName = "firstwindow";    //Define our window class name
const int Width = 800;    //window width
const int Height = 600;    //window height

DWORD dwUserIndex = 0;
XINPUT_STATE* pState = new XINPUT_STATE();

int main()
{
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
	// var devices = Manager.GetDevices(DeviceClass.GameControl, DeviceEnumerationFlags.AllDevices).ToList();
	

	// win api
	WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = WndClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);


	if (!RegisterClassEx(&wc))    //Register our windows class
	{
		//if registration failed, display error
		MessageBox(NULL, "Error registering class",
			"Error", MB_OK | MB_ICONERROR);
		return false;
	}


	hwnd = CreateWindowEx(    //Create our Extended Window
		NULL,    //Extended style
		WndClassName,    //Name of our windows class
		"Window Title",    //Name in the title bar of our window
		WS_OVERLAPPEDWINDOW,    //style of our window
		CW_USEDEFAULT, CW_USEDEFAULT,    //Top left corner of window
		Width,    //Width of our window
		Height,    //Height of our window
		NULL,    //Handle to parent window
		NULL,    //Handle to a Menu
		GetModuleHandle(NULL),    //Specifies instance of current program
		NULL    //used for an MDI client window
	);

	if (!hwnd)    //Make sure our window has been created
	{
		DWORD dwrd = GetLastError();

		std::ostringstream s;
		s << "Error create window>> " << dwrd;

		MessageBoxA(NULL, std::string(s.str()).c_str(),
			"Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	ShowWindow(hwnd, 1);    //Shows our window
	UpdateWindow(hwnd);

	messageloop();

	return 0;
}

int messageloop() {    //The message loop

	MSG msg;    //Create a new message structure
	ZeroMemory(&msg, sizeof(MSG));    //clear message structure to NULL

	while (true)    //while there is a message
	{
		//if there was a windows message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)    //if the message was WM_QUIT
				break;    //Exit the message loop

			TranslateMessage(&msg);    //Translate the message

									   //Send the message to default windows procedure
			DispatchMessage(&msg);
		}

		else {    //Otherewise, keep the flow going
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

	}

	return (int)msg.wParam;        //return the message

}