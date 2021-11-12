#include "script.h"
#include "keyboard.h"


#include <string>
#include <ctime>
#include "Pattern.h"
#include <Psapi.h>
using namespace std;

#pragma warning(disable : 4244 4305) // double <-> float conversions

using namespace std;
#include "MenuFunctions.h"

char *model;
HANDLE mainFiber;
DWORD wakeAt;



void BypassOnlineVehicleKick(Vehicle vehicle)
{
	Player player = PLAYER::PLAYER_ID();
	int var;
	DECORATOR::DECOR_REGISTER("Player_Vehicle", 3);
	DECORATOR::DECOR_REGISTER("Veh_Modded_By_Player", 3);
	DECORATOR::DECOR_SET_INT(vehicle, "Player_Vehicle", NETWORK::_0xBC1D768F2F5D6C05(player));
	DECORATOR::DECOR_SET_INT(vehicle, "Veh_Modded_By_Player", GAMEPLAY::GET_HASH_KEY(PLAYER::GET_PLAYER_NAME(player)));
	DECORATOR::DECOR_SET_INT(vehicle, "Not_Allow_As_Saved_Veh", 0);
	if (DECORATOR::DECOR_EXIST_ON(vehicle, "MPBitset"))
	{
		var = DECORATOR::DECOR_GET_INT(vehicle, "MPBitset");
	}
	GAMEPLAY::SET_BIT(&var, 3);
	DECORATOR::DECOR_SET_INT(vehicle, "MPBitset", var);
	VEHICLE::SET_VEHICLE_IS_STOLEN(vehicle, false);
}

Vehicle veh;

int CREATE_VEHICLE(char* hash, float x, float y, float z)
{
	DWORD model = GAMEPLAY::GET_HASH_KEY(hash);
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model))
	{
		Player playerPed = PLAYER::PLAYER_PED_ID();
		STREAMING::REQUEST_MODEL(model);
		STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
		while (!STREAMING::HAS_MODEL_LOADED(model) && !STREAMING::HAS_PTFX_ASSET_LOADED()) WAIT(0);
		{
			Vehicle veh = VEHICLE::CREATE_VEHICLE(model, x, y, z, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);
			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(veh);
			BypassOnlineVehicleKick(veh);
			VEHICLE::SET_VEHICLE_ENGINE_ON(veh, TRUE, TRUE);
		}
	}
	return veh;
}


void addVehOption(char* option, char* model11, char *notification)
{
	addOption(option);
	if (currentOption == optionCount && optionPress)
	{
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		CREATE_VEHICLE(model11, coords.x, coords.y, coords.z);
		drawNotification(notification);
	}
}

char* VehicleString;
bool SpawningVehicle;

void SpawnCar(char *carName)
{
	VehicleString = carName;
	SpawningVehicle = true;
}

bool PLAYER_GODMODE = false;
bool PLAYER_NEVER_WANTED = false;
bool PLAYER_GHOST = false;
bool PLAYER_NO_RAGDOLL = false;
bool PLAYER_NO_NOISE = false;
bool PLAYER_FAST_RUN = false;
bool PLAYER_FAST_SWIM = false;
bool PLAYER_HIGH_JUMP = false;
bool PLAYER_UNLIMITED_SPECIAL_ABILITY = false;
bool ONLINE_MONEY_DROP = false;
bool MISC_HIDE_HUD = false;
bool VEHICLE_GODMODE = false;
bool VEHICLE_GHOST = false;
bool VEHICLE_MOBILE_RADIO = false;
bool WEAPON_UNLIMITED_AMMO = false;
bool RECOVERY_RP_INCREASER = false;

void MAIN()
{
	addTitle("TRAINER BASICS");
	addSubmenuOption("PLAYER MENU ~y~>>", 1);
	addSubmenuOption("VEHICLE MENU ~y~>>", 2);
	addSubmenuOption("WEAPON MENU ~y~>>", 3);
	addSubmenuOption("WEATHER MENU ~y~>>", 4);
	addSubmenuOption("TIME MENU ~y~>>", 5);
	normalMenuActions();
}

void PLAYER_MENU()
{
	Player PLAYER_ID = PLAYER::PLAYER_ID();
	Ped PLAYER_PED_ID = PLAYER::PLAYER_PED_ID();
	BOOL PLAYER_EXIST = ENTITY::DOES_ENTITY_EXIST(PLAYER_PED_ID);
	Vehicle VEHICLE_ID = PED::GET_VEHICLE_PED_IS_USING(PLAYER_PED_ID);
	Vector3 ONLINE_PLAYER_COORDS = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1);

	addTitle("PLAYER MENU");
	addBoolOption("GODMODE", PLAYER_GODMODE);
	addBoolOption("NEVER WANTED", PLAYER_NEVER_WANTED);
	addOption("+ WANTED");
	addOption("- WANTED");
	addBoolOption("GHOST", PLAYER_GHOST);
	addBoolOption("NO RAGDOLL", PLAYER_NO_RAGDOLL);
	addOption("CASH");
	addBoolOption("NO NOISE", PLAYER_NO_NOISE);
	addBoolOption("FAST RUN", PLAYER_FAST_RUN);
	addBoolOption("FAST SWIM", PLAYER_FAST_SWIM);
	addBoolOption("HIGH JUMP", PLAYER_HIGH_JUMP);
	addBoolOption("INF. ABILITY", PLAYER_UNLIMITED_SPECIAL_ABILITY);
	if (getOption() == 1)
	{
		if (!PLAYER_GODMODE)
		{
			PLAYER_GODMODE = true;
		}
		else
		{
			PLAYER_GODMODE = false;
		}
	}
	if (getOption() == 2)
	{
		if (!PLAYER_NEVER_WANTED)
		{
			PLAYER_NEVER_WANTED = true;
		}
		else
		{
			PLAYER_NEVER_WANTED = false;
		}
	}
	if (getOption() == 3)
	{
		PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER_ID, PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER_ID) + 1, 0);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER_ID, 0);
		drawNotification("~HUD_COLOUR_YELLOW~+ WANTED");
	}
	if (getOption() == 4)
	{
		PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER_ID, PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER_ID) - 1, 0);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER_ID, 0);
		drawNotification("~HUD_COLOUR_YELLOW~- WANTED");
	}
	if (getOption() == 5)
	{
		if (!PLAYER_GHOST)
		{
			PLAYER_GHOST = true;
		}
		else
		{
			PLAYER_GHOST = false;
		}
	}
	if (getOption() == 6)
	{
		if (!PLAYER_NO_RAGDOLL)
		{
			PLAYER_NO_RAGDOLL = true;
			PED::SET_PED_CAN_RAGDOLL(PLAYER_PED_ID, false);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER_PED_ID, false);
			PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER_PED_ID, false);
			PLAYER::GIVE_PLAYER_RAGDOLL_CONTROL(PLAYER_ID, true);
			PED::SET_PED_RAGDOLL_ON_COLLISION(PLAYER_PED_ID, false);
		}
		else
		{
			PLAYER_NO_RAGDOLL = false;
			PED::SET_PED_CAN_RAGDOLL(PLAYER_PED_ID, true);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER_PED_ID, true);
			PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER_PED_ID, true);
			PLAYER::GIVE_PLAYER_RAGDOLL_CONTROL(PLAYER_ID, false);
			PED::SET_PED_RAGDOLL_ON_COLLISION(PLAYER_PED_ID, true);
		}
	}
	if (getOption() == 7)
	{
		for (int i = 0; i < 3; i++)
		{
			char statNameFull[32];
			sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
			Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
			int val;
			STATS::STAT_GET_INT(hash, &val, -1);
			val += 100000000;
			STATS::STAT_SET_INT(hash, val, 1);
		}
		drawNotification("~HUD_COLOUR_YELLOW~CASH");
	}
	if (getOption() == 8)
	{
		if (!PLAYER_NO_NOISE)
		{
			PLAYER_NO_NOISE = true;
		}
		else
		{
			PLAYER_NO_NOISE = false;
		}
	}
	if (getOption() == 9)
	{
		if (!PLAYER_FAST_RUN)
		{
			PLAYER_FAST_RUN = true;
		}
		else
		{
			PLAYER_FAST_RUN = false;
		}
	}
	if (getOption() == 10)
	{
		if (!PLAYER_FAST_SWIM)
		{
			PLAYER_FAST_SWIM = true;
		}
		else
		{
			PLAYER_FAST_SWIM = false;
		}
	}
	if (getOption() == 11)
	{
		if (!PLAYER_HIGH_JUMP)
		{
			PLAYER_HIGH_JUMP = true;
		}
		else
		{
			PLAYER_HIGH_JUMP = false;
		}
	}
	if (getOption() == 12)
	{
		if (!PLAYER_UNLIMITED_SPECIAL_ABILITY)
		{
			PLAYER_UNLIMITED_SPECIAL_ABILITY = true;
		}
		else
		{
			PLAYER_UNLIMITED_SPECIAL_ABILITY = false;
		}
	}
	normalMenuActions();
}

void VEHICLE_MENU()
{
	Player PLAYER_ID = PLAYER::PLAYER_ID();
	Ped PLAYER_PED_ID = PLAYER::PLAYER_PED_ID();
	BOOL PLAYER_EXIST = ENTITY::DOES_ENTITY_EXIST(PLAYER_PED_ID);
	Vehicle VEHICLE_ID = PED::GET_VEHICLE_PED_IS_USING(PLAYER_PED_ID);
	Vector3 ONLINE_PLAYER_COORDS = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1);

	addTitle("VEHICLE MENU");
	addOption("SPAWN VEHICLE ~y~>>");
	addBoolOption("GODMODE", VEHICLE_GODMODE);
	addBoolOption("GHOST", VEHICLE_GHOST);
	addBoolOption("MOBILE RADIO", VEHICLE_MOBILE_RADIO);
	addOption("SKIP RADIO");

	if (getOption() == 1)
	{
		GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "", "", "", "", "", "", 100);
		while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
		CREATE_VEHICLE(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT(), 0.0, 1, 1);
	}
	if (getOption() == 2)
	{
		if (!VEHICLE_GODMODE)
		{
			VEHICLE_GODMODE = true;
		}
		else
		{
			VEHICLE_GODMODE = false;
		}
	}
	if (getOption() == 3)
	{
		if (!VEHICLE_GHOST)
		{
			VEHICLE_GHOST = true;
		}
		else
		{
			VEHICLE_GHOST = false;
		}
	}
	if (getOption() == 4)
	{
		if (!VEHICLE_MOBILE_RADIO)
		{
			VEHICLE_MOBILE_RADIO = true;
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		}
		else
		{
			VEHICLE_MOBILE_RADIO = false;
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
		}
	}
	if (getOption() == 5)
	{
		AUDIO::SKIP_RADIO_FORWARD();
		drawNotification("~HUD_COLOUR_YELLOW~RADIO SKIPED");
	}

	normalMenuActions();
}

void WEAPON_MENU()
{
	Player PLAYER_ID = PLAYER::PLAYER_ID();
	Ped PLAYER_PED_ID = PLAYER::PLAYER_PED_ID();
	BOOL PLAYER_EXIST = ENTITY::DOES_ENTITY_EXIST(PLAYER_PED_ID);
	Vehicle VEHICLE_ID = PED::GET_VEHICLE_PED_IS_USING(PLAYER_PED_ID);
	Vector3 ONLINE_PLAYER_COORDS = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1);

	addTitle("WEAPON MENU");
	addBoolOption("INF. AMMO", WEAPON_UNLIMITED_AMMO);
	if (getOption() == 1)
	{
		if (!WEAPON_UNLIMITED_AMMO)
		{
			WEAPON_UNLIMITED_AMMO = true;
		}
		else
		{
			WEAPON_UNLIMITED_AMMO = false;
		}
	}
	normalMenuActions();
}

void WEATHER_MENU()
{
	Player PLAYER_ID = PLAYER::PLAYER_ID();
	Ped PLAYER_PED_ID = PLAYER::PLAYER_PED_ID();
	BOOL PLAYER_EXIST = ENTITY::DOES_ENTITY_EXIST(PLAYER_PED_ID);
	Vehicle VEHICLE_ID = PED::GET_VEHICLE_PED_IS_USING(PLAYER_PED_ID);
	Vector3 ONLINE_PLAYER_COORDS = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1);

	addTitle("WEATHER MENU");
	addOption("CLEAR");
	addOption("EXTRASUNNY");
	addOption("CLOUDS");
	addOption("SMOG");
	addOption("FOGGY");
	addOption("OVERCAST");
	addOption("RAINTHUNDER");
	addOption("SNOW");
	addOption("BLIZZARD");
	addOption("SNOWLIGHT");
	if (getOption() == 1)
	{
		GAMEPLAY::SET_OVERRIDE_WEATHER("CLEAR");
		drawNotification("~HUD_COLOUR_YELLOW~WEATHER: CLEAR");
	}
	if (getOption() == 2)
	{
		GAMEPLAY::SET_OVERRIDE_WEATHER("EXTRASUNNY");
		drawNotification("~HUD_COLOUR_YELLOW~WEATHER: EXTRASUNNY");
	}
	if (getOption() == 3)
	{
		GAMEPLAY::SET_OVERRIDE_WEATHER("CLOUDS");
		drawNotification("~HUD_COLOUR_YELLOW~WEATHER: CLOUDS");
	}
	if (getOption() == 4)
	{
		GAMEPLAY::SET_OVERRIDE_WEATHER("SMOG");
		drawNotification("~HUD_COLOUR_YELLOW~WEATHER: SMOG");
	}
	if (getOption() == 5)
	{
		GAMEPLAY::SET_OVERRIDE_WEATHER("FOGGY");
		drawNotification("~HUD_COLOUR_YELLOW~WEATHER: FOGGY");
	}
	if (getOption() == 6)
	{
		GAMEPLAY::SET_OVERRIDE_WEATHER("OVERCAST");
		drawNotification("~HUD_COLOUR_YELLOW~WEATHER: OVERCAST");
	}
	if (getOption() == 7)
	{
		GAMEPLAY::SET_OVERRIDE_WEATHER("RAINTHUNDER");
		drawNotification("~HUD_COLOUR_YELLOW~WEATHER: RAINTHUNDER");
	}
	if (getOption() == 8)
	{
		GAMEPLAY::SET_OVERRIDE_WEATHER("SNOW");
		drawNotification("~HUD_COLOUR_YELLOW~WEATHER: SNOW");
	}
	if (getOption() == 9)
	{
		GAMEPLAY::SET_OVERRIDE_WEATHER("BLIZZARD");
		drawNotification("~HUD_COLOUR_YELLOW~WEATHER: BLIZZARD");
	}
	if (getOption() == 10)
	{
		GAMEPLAY::SET_OVERRIDE_WEATHER("SNOWLIGHT");
		drawNotification("~HUD_COLOUR_YELLOW~WEATHER: SNOWLIGHT");
	}
	normalMenuActions();
}

void TIME_MENU()
{
	Player PLAYER_ID = PLAYER::PLAYER_ID();
	Ped PLAYER_PED_ID = PLAYER::PLAYER_PED_ID();
	BOOL PLAYER_EXIST = ENTITY::DOES_ENTITY_EXIST(PLAYER_PED_ID);
	Vehicle VEHICLE_ID = PED::GET_VEHICLE_PED_IS_USING(PLAYER_PED_ID);
	Vector3 ONLINE_PLAYER_COORDS = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1);

	addTitle("TIME MENU");
	normalMenuActions();
}

Vector3 obje;
Vector3 fff;

DWORD64 GetModuleBase(HANDLE hProc, string &sModuleName)
{
	HMODULE *hModules;
	hModules = 0;
	char szBuf[50];
	DWORD cModules = 0;
	DWORD64 dwBase = 0;

	EnumProcessModules(hProc, hModules, 0, &cModules);
	hModules = new HMODULE[cModules / sizeof(HMODULE)];

	if (EnumProcessModules(hProc, hModules, cModules / sizeof(HMODULE), &cModules)) {
		for (int i = 0; i < cModules / sizeof(HMODULE); ++i) {
			if (GetModuleBaseName(hProc, hModules[i], szBuf, sizeof(szBuf))) {
				if (sModuleName.compare(szBuf) == 0) {
					dwBase = (DWORD64)hModules[i];
					break;
				}
			}
		}
	}
	return dwBase;
}

void bypass_online()
{
	__int64 Address = GetModuleBase(GetCurrentProcess(), string("GTA5.exe"));

	CHAR *MemoryBuff = new CHAR[4096];
	HANDLE hProcess = GetCurrentProcess();
	BYTE bytes[10] = { 0x48, 0x8B, 0x88, 0x10, 0x01, 0x00, 0x00, 0x48, 0x8B, 0xC1 };//BYPASS_SPAWN
	BYTE bytes2[10] = { 0x83, 0xBB, 0x34, 0x01, 0x00, 0x00, 0xFF, 0x0F, 0x95, 0xC0 };//BYPASS_EVERY_TIME_CHECK_FOR_PLAYER_MODEL
	BYTE nop2[2] = { 0x90, 0x90 };
	BYTE nop3[3] = { 0x90, 0x90, 0x90 };

	int Check = 0;
	for (;;)
	{
		ReadProcessMemory(hProcess, (LPVOID)Address, (LPVOID)MemoryBuff, 4096, NULL);
		for (INT p = 0; p < 4096; p++)
		{
			Address++;
			MemoryBuff++;
			if (memcmp(MemoryBuff, bytes, 10) == 0)
			{
				WriteProcessMemory(hProcess, (LPVOID)(Address + 0x20), nop2, 2, NULL);
				WriteProcessMemory(hProcess, (LPVOID)(Address + 0x2D), nop2, 2, NULL);
				Check += 1;
			}
			if (memcmp(MemoryBuff, bytes2, 10) == 0)
			{
				WriteProcessMemory(hProcess, (LPVOID)(Address - 3), nop3, 3, NULL);
				Check += 1;
			}
			if (Check >= 2)
			{
				goto endfunc;
			}
		}
		MemoryBuff = MemoryBuff - 4096;
	}
endfunc:;
}

void Hook()
{
	bypass_online();
	while (true)
	{

	monitorButtons();
	optionCount = 0;

	

	if (submenu == 1)
	{
		MAIN();
	}
	else if (submenu == 2)
	{
		PLAYER_MENU();
	}
	else if (submenu == 3)
	{
		VEHICLE_MENU();
	}
	else if (submenu == 4)
	{
		WEAPON_MENU();
	}
	else if (submenu == 5)
	{
		WEATHER_MENU();
	}
	else if (submenu == 6)
	{
		TIME_MENU();
	}
	

	optionPress = false;
	WAIT(0);
	}
}



void ScriptMain()
{
	srand(GetTickCount());
	Hook();
}
