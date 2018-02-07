#include <API/ARK/Ark.h>
#include <Logger/Logger.h>
#include <Permissions.h>

#pragma comment(lib, "ArkApi.lib")
#pragma comment(lib, "Permissions.lib")

void OnChatMessage(AShooterPlayerController* player_controller, FString* message, EChatSendMode::Type)
{
	TArray<FString> parsed;
	message->ParseIntoArray(parsed, L" ", true);

	if (!parsed.IsValidIndex(1))
		return;

	const uint64 steam_id = ArkApi::GetApiUtils().GetSteamIdFromController(player_controller);

	if (Permissions::IsPlayerHasPermission(steam_id, "Cheat." + parsed[1]))
	{
		if (!message->RemoveFromStart("/cheat "))
			return;

		FString result;
		player_controller->ConsoleCommand(&result, message, true);
	}
	else
	{
		ArkApi::GetApiUtils().SendChatMessage(player_controller, L"AdminsPermissions", L"You don't have permissions to use this command");
	}
}

void Load()
{
	Log::Get().Init("AdminsPermissions");

	ArkApi::GetCommands().AddChatCommand("/cheat", &OnChatMessage);
}

void Unload()
{
	ArkApi::GetCommands().RemoveChatCommand("/cheat");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Load();
		break;
	case DLL_PROCESS_DETACH:
		Unload();
		break;
	}
	return TRUE;
}
