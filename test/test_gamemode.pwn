#include <a_samp>
#include <a_spdlog>

#define MoneyLog "money"
#define LoginLog "login"
#define RandomLog "random"
#define ChatLog "chatlog"
#define Console "stdout"

main() {
	LogCritical(Console, "Loaded script '%s'", "test");
}

public OnGameModeInit()
{
	// Enable async logging
    LoggerSetAsyncMode(4096);

  	// Initialize log sinks
    BasicLogger(MoneyLog, "scriptfiles/MoneyLog.txt");  // To file
	BasicLogger(LoginLog, "scriptfiles/LoginLog.txt");  // To file
	BasicLogger(ChatLog, "scriptfiles/ChatLog.txt");  	// To file
	BasicLogger(RandomLog, "scriptfiles/Random.txt");  	// To file
	ConsoleLogger(Console);                          	// To console (no log)

	// Info (This text doesnt show up in server_log.txt)
	LogInfo(Console, "Gamemode initializing...");

	// Don't use these lines if it's a filterscript
	SetGameModeText("SPDLog Test");
	AddPlayerClass(0, 1958.3783, 1343.1572, 15.3746, 269.1425, 0, 0, 0, 0, 0, 0);

	// Info (This text doesnt show up in server_log.txt)
	LogInfo(Console, "Gamemode initialized...");

	// Use param ids
	LogInfo(
		RandomLog,
		"The last param: %s and then the rest %d %s", "BLUB", 0, "50", 5991992, 10, 20, 30
	);
	return 1;
}

public OnGameModeExit()
{
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	SetPlayerPos(playerid, 1958.3783, 1343.1572, 15.3746);
	SetPlayerCameraPos(playerid, 1958.3783, 1343.1572, 15.3746);
	SetPlayerCameraLookAt(playerid, 1958.3783, 1343.1572, 15.3746);
	return 1;
}

public OnPlayerConnect(playerid)
{
	LogInfo(LoginLog, "New player joined pid(%d)", playerid);
	LogCritical(Console, "OMG a new USER!!!");
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	LogInfo(LoginLog, "Player left pid(%d)", playerid);
	LogCritical(Console, "We are losing users!!!!!!!");
	return 1;
}

public OnPlayerSpawn(playerid)
{
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	LogInfo(ChatLog, "Chat message pid(%d): %s", playerid, text);
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	// Commands
	if (strcmp("/mycommand", cmdtext, true, 10) == 0)
	{
		return 1;
	}

		// Commands
	if (strcmp("/writelog", cmdtext, true, 9) == 0)
	{
	    LogInfo(RandomLog, "Some things");
		return 1;
	}
	
	// Set log level to debug
	if (strcmp("/debug", cmdtext, true, 6) == 0)
	{
		LoggerSetLevel(SPDLOG_LEVEL:DEBUG);
		return 1;
	}

	// Yeah
	LogWarn(Console, "Unknown command by pid(%d): %s", playerid, cmdtext);
	return 0;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	LogInfo(Console, "RCon Command: %s", cmd);
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}
