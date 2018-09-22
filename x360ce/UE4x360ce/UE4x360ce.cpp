#include "UE4x360ce.h"

#include "Utils.h"
#include "XInputModuleManager.h"
#include "ControllerManager.h"

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

int UE4x360ce::GetControllerUserIndexByGUID(const GUID* m_productid)
{
	auto controllers = ControllerManager::Get().GetControllers();

	for (auto cntr: controllers)
	{
		auto controller = static_pointer_cast<Controller>(cntr);
		if (controller.get() != nullptr)
		{
			int result = memcmp(m_productid, &controller->m_productid, sizeof(GUID));
			if (result == 0)
			{
				return controller->m_user;
			}
		}
	}

	return -1;
}