//////////////////////////////////// Spidey MOD
//Give the actor 5 different spidey actions
//Action 1: Crawl
//Action 2: Leap/Hop
//Action 3: Slide
//Action 4: Push
//Action 5: Yank/PULL

/*
#include "g_spidey_moves.h"
#include "g_local.h"
//#include "g_cmds.c"

//////////////////////////////////// Spidey MOD

void Cmd_spidey_1(edict_t *ent) {
	
	
		char* msg;

		if (deathmatch->value && !sv_cheats->value)
		{
			gi.cprintf(ent, PRINT_HIGH, "You must run the server with '+set cheats 1' to enable this command.\n");
			return;
		}

		if  (ent->movetype == MOVETYPE_NOCLIP)
		{
			ent->movetype = MOVETYPE_WALK;
			msg = "noclip OFF\n";
		}
		else
		{
			ent->movetype = MOVETYPE_NOCLIP;
			msg = "noclip ON\n";
		}

		gi.cprintf(ent, PRINT_HIGH, msg);
	

}


void Cmd_spidey_2(edict_t *ent) {


	char* msg;

	if (deathmatch->value && !sv_cheats->value)
	{
		gi.cprintf(ent, PRINT_HIGH, "You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}

	if (ent->movetype == MOVETYPE_PUSH)
	{
		ent->movetype = MOVETYPE_WALK;
		msg = "noclip OFF\n";
	}
	else
	{
		ent->movetype = MOVETYPE_PUSH;
		msg = "noclip ON\n";
	}

	gi.cprintf(ent, PRINT_HIGH, msg);


}

void Cmd_spidey_3(edict_t *ent) {


	char* msg;

	if (deathmatch->value && !sv_cheats->value)
	{
		gi.cprintf(ent, PRINT_HIGH, "You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}

	if (ent->movetype == MOVETYPE_FLY)
	{
		ent->movetype = MOVETYPE_WALK;
		msg = "noclip OFF\n";
	}
	else
	{
		ent->movetype = MOVETYPE_FLY;
		msg = "noclip ON\n";
	}

	gi.cprintf(ent, PRINT_HIGH, msg);


}
void Cmd_spidey_4(edict_t *ent) {


	char* msg;

	if (deathmatch->value && !sv_cheats->value)
	{
		gi.cprintf(ent, PRINT_HIGH, "You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}

	if (ent->movetype == MOVETYPE_BOUNCE)
	{
		ent->movetype = MOVETYPE_WALK;
		msg = "noclip OFF\n";
	}
	else
	{
		ent->movetype = MOVETYPE_BOUNCE;
		msg = "noclip ON\n";
	}

	gi.cprintf(ent, PRINT_HIGH, msg);


}

void Cmd_spidey_5(edict_t *ent) {


	char* msg;

	if (deathmatch->value && !sv_cheats->value)
	{
		gi.cprintf(ent, PRINT_HIGH, "You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}

	if (ent->movetype == MOVETYPE_STEP)
	{
		ent->movetype = MOVETYPE_WALK;
		msg = "noclip OFF\n";
	}
	else
	{
		ent->movetype = MOVETYPE_STEP;
		msg = "noclip ON\n";
	}

	gi.cprintf(ent, PRINT_HIGH, msg);


}
void Cmd_spidey_6(edict_t *ent) {


	char* msg;

	if (deathmatch->value && !sv_cheats->value)
	{
		gi.cprintf(ent, PRINT_HIGH, "You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}

	if (ent->movetype == MOVETYPE_STOP)
	{
		ent->movetype = MOVETYPE_WALK;
		msg = "noclip OFF\n";
	}
	else
	{
		ent->movetype = MOVETYPE_STEP;
		msg = "noclip ON\n";
	}

	gi.cprintf(ent, PRINT_HIGH, msg);


}



*/


//////////////////////////////////// Spidey MOD
//Give the actor 5 different spidey actions
//Action 1: Crawl
//Action 2: Leap/Hop
//Action 3: Slide
//Action 4: Push
//Action 5: Yank/PULL

//////////////////////////////////// Spidey MOD
