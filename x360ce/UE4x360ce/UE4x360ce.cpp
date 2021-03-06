#include "UE4x360ce.h"

#include "Utils.h"
#include "XInputModuleManager.h"
#include "ControllerManager.h"
#include "Config.h"

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

void UE4x360ce::XInputEnable(bool enable)
{
	// If any controller is native XInput then use state too.
	for (auto it = ControllerManager::Get().GetControllers().begin(); it != ControllerManager::Get().GetControllers().end(); ++it)
	{
		if ((*it)->m_passthrough)
			XInputModuleManager::Get().XInputEnable(enable);
	}

	ControllerManager::Get().XInputEnable(enable);
}

int UE4x360ce::GetControllerUserIndexByGUIDInstance(const GUID* guidInstance)
{
	auto controllers = ControllerManager::Get().GetControllers();

	for (auto cntr: controllers)
	{
		auto controller = static_pointer_cast<Controller>(cntr);
		if (controller.get() != nullptr)
		{
			int result = memcmp(guidInstance, &controller->m_instanceid, sizeof(GUID));
			if (result == 0)
			{
				return controller->m_user;
			}
		}
	}

	return -1;
}

void UE4x360ce::Reset()
{
	ControllerManager::Get().GetControllers().clear();
	ControllerManager::Get().GetConfig().ReadConfig();
}

int16_t UE4x360ce::GetDeadZone(DWORD dwUserIndex, int DeadZoneIndex, bool & bIsFound)
{
	auto controllers = ControllerManager::Get().GetControllers();
	
	if (dwUserIndex >= controllers.size() || dwUserIndex < 0)
	{
		bIsFound = false;
		return -1;
	}

	auto controller = controllers[dwUserIndex];
	if (controller == nullptr)
	{
		bIsFound = false;
		return -1;
	}

	auto m_mapping = controller->m_mapping;
	if (DeadZoneIndex >= 4 || DeadZoneIndex < 0)
	{
		bIsFound = false;
		return -1;
	}

	bIsFound = true;
	return m_mapping.Axis[DeadZoneIndex].axisdeadzone;
}

