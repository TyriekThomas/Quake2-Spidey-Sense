/*
Spidey MOD
Spidey Sense Script
Tyriem Thomas
Spidey Sense
*/
/*
#include "g_spideysense.h"

void spideySr(edict_t* ent)
{
	char	string[1024];
	char* sk;

	if (skill->value == 0)
		sk = "easy";
	else if (skill->value == 1)
		sk = "medium";
	else if (skill->value == 2)
		sk = "hard";
	else
		sk = "hard+";

	// send the layout
	/*
	* "xv 202 yv 12 string2 \"%s\" "		// skill
		"xv 0 yv 24 cstring2 \"%s\" "		// level name
		"xv 0 yv 54 cstring2 \"%s\" "		// help 1
		"xv 0 yv 110 cstring2 \"%s\" "		// help 2
		"xv 50 yv 164 string2 \" kills     goals    secrets\" "
		"xv 50 yv 172 string2 \"%3i/%3i     %i/%i       %i/%i\" "
	*/

/*
	Com_sprintf(string, sizeof(string),
		"xv 32 yv 8 picn spidey "			// background
		,
		sk,
		level.level_name,
		game.helpmessage1,
		game.helpmessage2,
		level.killed_monsters, level.total_monsters,
		level.found_goals, level.total_goals,
		level.found_secrets, level.total_secrets);

	gi.WriteByte(svc_layout);
	gi.WriteString(string);
	gi.unicast(ent, true);
}


/*
==================
Cmd_SpideyS

Display the current help message
==================
*/

/*
void Cmd_SpideyS(edict_t* ent)
{
	// this is for backwards compatability
	if (deathmatch->value)
	{
		Cmd_Score_f(ent);
		return;
	}

	ent->client->showinventory = false;
	ent->client->showscores = false;

	if (ent->client->showhelp && (ent->client->pers.game_helpchanged == game.helpchanged))
	{
		ent->client->showhelp = false;
		return;
	}

	ent->client->showhelp = true;
	ent->client->pers.helpchanged = 0;
	spideySr(ent);
}
/*

/*include "g_spideysense.h"
#include "g_web.c"
#include "p_client.c"






void locateScenes(edict_t ent) {
	edict_t *loc1; edict_t *loc2; edict_t *loc3;
	loc1 = SelectCoopSpawnPoint(ent);
	loc2 = SelectCoopSpawnPoint(ent);
	loc3 = SelectCoopSpawnPoint(ent);
	SpawnEntities("base1", "monster_tank", game.spawnpoint[0]);
	Com_DPrintf("Origin");
	Com_DPrintf( ent->s.origin);
	Com_DPrintf("SPOT");
	Com_DPrintf(game.spawnpoint[0]);
	Com_DPrintf(game.spawnpoint[1]);
	Com_DPrintf(game.spawnpoint[2]);
}

*/
/*

void locateScenes() {
	SpawnEntities("base1", "monster_tank", game.spawnpoint[0]);
	Com_DPrintf("Origin");
	//Com_DPrintf(ent->s.origin);
	Com_DPrintf("SPOT");
	Com_DPrintf(game.spawnpoint[0]);
	Com_DPrintf(game.spawnpoint[1]);
	Com_DPrintf(game.spawnpoint[2]);
}
*/