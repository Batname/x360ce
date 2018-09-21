#include "UE4x360ce.h"

#include "XInputModuleManager.h"
#include "Utils.h"

void UE4x360ce::Run()
{
	ControllerManager::Get();
	ControllerManager::Get().Init();
}

DWORD UE4x360ce::XInputGetState(DWORD dwUserIndex, XINPUT_STATE * pState)
{
	ControllerBase* pController;
	if (!pState)
		return ERROR_BAD_ARGUMENTS;
	u32 initFlag = ControllerManager::Get().DeviceInitialize(dwUserIndex, &pController);
	if (initFlag != ERROR_SUCCESS)
		return initFlag;

	return pController->GetState(pState);
}