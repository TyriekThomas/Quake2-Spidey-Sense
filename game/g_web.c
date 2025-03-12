/*
Spidey MOD
Web Script
Tyriem Thomas
Three spidey weapons
*/
#include "g_local.h"
#include "g_web.h"


/*------------------------------------------------------------------------*/
// Grapple to WEB
/* GRAPPLE	*/


/*

*/

/*

*/

/*

*/
/*------------------------------------------------------------------------*/

// ent is player

void PlayerResetWEB(edict_t* ent)
{
	if (ent->client && ent->client->WEB)
		ResetWEB(ent->client->WEB);
}

// self is grapple, not player
void ResetWEB(edict_t* self)
{
	if (self->owner->client->WEB) {
		float volume = 1.0;
		gclient_t* cl;

		if (self->owner->client->silencer_shots)
			volume = 0.2;

		gi.sound(self->owner, CHAN_RELIABLE + CHAN_WEAPON, gi.soundindex("weapons/web/grreset.wav"), volume, ATTN_NORM, 0);
		cl = self->owner->client;
		cl->WEB = NULL;
		cl->WEBreleasetime = level.time;
		cl->WEBstate = WEB_STATE_FLY; // we're firing, not on hook
		cl->ps.pmove.pm_flags &= ~PMF_NO_PREDICTION;
		G_FreeEdict(self);
	}
}
// draw beam between grapple and self
void WEBDrawString(edict_t* self)
{
	vec3_t	offset, start, end, f, r;
	vec3_t	dir;
	float	distance;

	AngleVectors(self->owner->client->v_angle, f, r, NULL);
	VectorSet(offset, 16, 16, self->owner->viewheight - 8);
	P_ProjectSource(self->owner->client, self->owner->s.origin, offset, f, r, start);

	VectorSubtract(start, self->owner->s.origin, offset);

	VectorSubtract(start, self->s.origin, dir);
	distance = VectorLength(dir);
	// don't draw cable if close
	if (distance < 64)
		return;

#if 0
	if (distance > 256)
		return;

	// check for min/max pitch
	vectoangles(dir, angles);
	if (angles[0] < -180)
		angles[0] += 360;
	if (fabs(angles[0]) > 45)
		return;

	trace_t	tr; //!!

	tr = gi.trace(start, NULL, NULL, self->s.origin, self, MASK_SHOT);
	if (tr.ent != self) {
		ResetWEB(self);
		return;
	}
#endif

	// adjust start for beam origin being in middle of a segment
//	VectorMA (start, 8, f, start);

	VectorCopy(self->s.origin, end);
	// adjust end z for end spot since the monster is currently dead
//	end[2] = self->absmin[2] + self->size[2] / 2;

	gi.WriteByte(svc_temp_entity);
#if 1 //def USE_GRAPPLE_CABLE
	gi.WriteByte(TE_GRAPPLE_CABLE);
	gi.WriteShort(self->owner - g_edicts);
	gi.WritePosition(self->owner->s.origin);
	gi.WritePosition(end);
	gi.WritePosition(offset);
#else
	gi.WriteByte(TE_MEDIC_CABLE_ATTACK);
	gi.WriteShort(self - g_edicts);
	gi.WritePosition(end);
	gi.WritePosition(start);
#endif
	gi.multicast(self->s.origin, MULTICAST_PVS);
}

void SV_AddGravity(edict_t* ent);

// pull the player toward the grapple
void WEBPull(edict_t* self)
{
	vec3_t hookdir, v;
	float vlen;

	if (strcmp(self->owner->client->pers.weapon->classname, "weapon_web") == 0 &&
		!self->owner->client->newweapon &&
		self->owner->client->weaponstate != WEAPON_FIRING &&
		self->owner->client->weaponstate != WEAPON_ACTIVATING) {
		ResetWEB(self);
		return;
	}

	if (self->enemy) {
		if (self->enemy->solid == SOLID_NOT) {
			ResetWEB(self);
			return;
		}
		if (self->enemy->solid == SOLID_BBOX) {
			T_Damage(self->enemy, self, self->owner, self->velocity, self->s.origin, vec3_origin, 100, 1, 0, MOD_WEB);
			//gi.sound(self, CHAN_WEAPON, gi.soundindex("weapons/web/grhurt.wav"), volume, ATTN_NORM, 0);
			VectorScale(self->enemy->size, 0.5, v);
			VectorAdd(v, self->enemy->s.origin, v);
			VectorAdd(v, self->enemy->mins, self->s.origin);
			gi.linkentity(self);
		}
		else
			VectorCopy(self->enemy->velocity, self->velocity);
		if (self->enemy->takedamage &&
			!CheckTeamDamage(self->enemy, self->owner)) {
			float volume = 1.0;

			if (self->owner->client->silencer_shots)
				volume = 0.2;

			T_Damage(self->enemy, self, self->owner, self->velocity, self->s.origin, vec3_origin, 1, 1, 0, MOD_WEB);
			gi.sound(self, CHAN_WEAPON, gi.soundindex("weapons/web/grhurt.wav"), volume, ATTN_NORM, 0);
		}
		if (self->enemy->deadflag) { // he died
			spidey_enemycount++;
			ResetWEB(self);
			return;
		}
	}

	WEBDrawString(self);

	if (self->owner->client->WEBstate > WEB_STATE_FLY) {
		// pull player toward grapple
		// this causes icky stuff with prediction, we need to extend
		// the prediction layer to include two new fields in the player
		// move stuff: a point and a velocity.  The client should add
		// that velociy in the direction of the point
		vec3_t forward, up;

		AngleVectors(self->owner->client->v_angle, forward, NULL, up);
		VectorCopy(self->owner->s.origin, v);
		v[2] += self->owner->viewheight;
		VectorSubtract(self->s.origin, v, hookdir);

		vlen = VectorLength(hookdir);

		if (self->owner->client->WEBstate == WEB_STATE_PULL &&
			vlen < 64) {
			float volume = 1.0;

			if (self->owner->client->silencer_shots)
				volume = 0.2;

			self->owner->client->ps.pmove.pm_flags |= PMF_NO_PREDICTION;
			gi.sound(self->owner, CHAN_RELIABLE + CHAN_WEAPON, gi.soundindex("weapons/web/grhang.wav"), volume, ATTN_NORM, 0);
			self->owner->client->WEBstate = WEB_STATE_HANG;
		}

		VectorNormalize(hookdir);
		VectorScale(hookdir, WEB_PULL_SPEED*1.5, hookdir);
		VectorCopy(hookdir, self->owner->velocity);
		SV_AddGravity(self->owner);
	}
}

void WEBTouch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	float volume = 1.0;

	if (other == self->owner)
		return;

	if (self->owner->client->WEBstate != WEB_STATE_FLY)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		ResetWEB(self);
		return;
	}

	VectorCopy(vec3_origin, self->velocity);

	PlayerNoise(self->owner, self->s.origin, PNOISE_IMPACT);

	if (other->takedamage) {
		WEBPull(self);
		T_Damage(other, self, self->owner, self->velocity, self->s.origin, plane->normal, self->dmg, 1000000, 100, MOD_WEB);
		ResetWEB(self);
		return;
	}

	self->owner->client->WEBstate = WEB_STATE_PULL; // we're on hook
	self->enemy = other;

	self->solid = SOLID_NOT;

	if (self->owner->client->silencer_shots)
		volume = 0.2;

	gi.sound(self->owner, CHAN_RELIABLE + CHAN_WEAPON, gi.soundindex("weapons/web/grpull.wav"), volume, ATTN_NORM, 0);
	gi.sound(self, CHAN_WEAPON, gi.soundindex("weapons/web/grhit.wav"), volume, ATTN_NORM, 0);

	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_SPARKS);
	gi.WritePosition(self->s.origin);
	if (!plane)
		gi.WriteDir(vec3_origin);
	else
		gi.WriteDir(plane->normal);
	gi.multicast(self->s.origin, MULTICAST_PVS);
}




void FireWEB(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int effect)
{
	edict_t* web;
	trace_t	tr;

	VectorNormalize(dir);

	web = G_Spawn();
	VectorCopy(start, web->s.origin);
	VectorCopy(start, web->s.old_origin);
	vectoangles(dir, web->s.angles);
	VectorScale(dir, speed*2, web->velocity);
	web->movetype = MOVETYPE_FLYMISSILE;
	web->clipmask = MASK_SHOT;
	web->solid = SOLID_BBOX;
	web->s.effects |= effect;
	VectorClear(web->mins);
	VectorClear(web->maxs);
	web->s.modelindex = gi.modelindex("models/weapons/grapple/hook/tris.md2");
	//	grapple->s.sound = gi.soundindex ("misc/lasfly.wav");
	web->owner = self;
	web->touch = WEBTouch;
	//	grapple->nextthink = level.time + FRAMETIME;
	//	grapple->think = CTFGrappleThink;
	web->dmg = damage;
	self->client->WEB = web;
	self->client->WEBstate = WEB_STATE_FLY; // we're firing, not on hook
	gi.linkentity(web);

	tr = gi.trace(self->s.origin, NULL, NULL, web->s.origin, web, MASK_SHOT);
	if (tr.fraction < 1.0)
	{
		VectorMA(web->s.origin, -10, dir, web->s.origin);
		web->touch(web, tr.ent, NULL, NULL);
	}
}
//////    Port to Single Player Game


void WEBFire(edict_t* ent, vec3_t g_offset, int damage, int effect)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	offset;
	float volume = 1.0;

	if (ent->client->WEBstate > WEB_STATE_FLY)
		return; // it's already out

	AngleVectors(ent->client->v_angle, forward, right, NULL);
	//	VectorSet(offset, 24, 16, ent->viewheight-8+2);
	VectorSet(offset, 24, 8, ent->viewheight - 8 + 2);
	VectorAdd(offset, g_offset, offset);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	if (ent->client->silencer_shots)
		volume = 0.2;

	gi.sound(ent, CHAN_RELIABLE + CHAN_WEAPON, gi.soundindex("weapons/web/grfire.wav"), volume, ATTN_NORM, 0);
	FireWEB(ent, start, forward, damage, WEB_SPEED*2, effect);

#if 0
	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_BLASTER);
	gi.multicast(ent->s.origin, MULTICAST_PVS);
#endif

	PlayerNoise(ent, start, PNOISE_WEAPON);
}


void Weapon_WEB_Fire(edict_t* ent)
{
	int		damage;

	damage = 25;
	WEBFire(ent, vec3_origin, damage, 40000);
	ent->client->ps.gunframe++;
}


void Weapon_WEB(edict_t* ent)
{
	static int	pause_frames[] = { 10, 18, 27, 0 };
	static int	fire_frames[] = { 6, 0 };
	int prevstate;

	// if the the attack button is still down, stay in the firing frame
	if ((ent->client->buttons & BUTTON_ATTACK) &&
		ent->client->weaponstate == WEAPON_FIRING &&
		ent->client->WEB)
		ent->client->ps.gunframe = 9;

	if (!(ent->client->buttons & BUTTON_ATTACK) &&
		ent->client->WEB) {
		ResetWEB(ent->client->WEB);
		if (ent->client->weaponstate == WEAPON_FIRING)
			ent->client->weaponstate = WEAPON_READY;
	}


	if (ent->client->newweapon &&
		ent->client->WEBstate > WEB_STATE_FLY &&
		ent->client->weaponstate == WEAPON_FIRING) {
		// he wants to change weapons while grappled
		ent->client->weaponstate = WEAPON_DROPPING;
		ent->client->ps.gunframe = 32;
	}

	prevstate = ent->client->weaponstate;
	Weapon_Generic(ent, 5, 9, 31, 36, pause_frames, fire_frames,
		Weapon_WEB_Fire);

	// if we just switched back to grapple, immediately go to fire frame
	if (prevstate == WEAPON_ACTIVATING &&
		ent->client->weaponstate == WEAPON_READY &&
		ent->client->WEBstate > WEB_STATE_FLY) {
		if (!(ent->client->buttons & BUTTON_ATTACK))
			ent->client->ps.gunframe = 9;
		else
			ent->client->ps.gunframe = 5;
		ent->client->weaponstate = WEAPON_FIRING;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//WEB PULLER
//

void WEBpullerFire(edict_t* ent, vec3_t g_offset, int damage, int effect)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	offset;
	float volume = 1.0;

	if (ent->client->WEBstate > WEB_STATE_FLY)
		return; // it's already out

	AngleVectors(ent->client->v_angle, forward, right, NULL);
	//	VectorSet(offset, 24, 16, ent->viewheight-8+2);
	VectorSet(offset, 24, 8, ent->viewheight - 8 + 2);
	VectorAdd(offset, g_offset, offset);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	if (ent->client->silencer_shots)
		volume = 0.2;

	gi.sound(ent, CHAN_RELIABLE + CHAN_WEAPON, gi.soundindex("weapons/web/grfire.wav"), volume, ATTN_NORM, 0);
	FireWEB(ent, start, forward, damage, WEB_SPEED *1, effect);

#if 0
	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_BLASTER);
	gi.multicast(ent->s.origin, MULTICAST_PVS);
#endif

	PlayerNoise(ent, start, PNOISE_WEAPON);
}
void Weapon_WEBpuller_Fire(edict_t* ent)
{
	int		damage;

	damage = 0;
	WEBpullerFire(ent, vec3_origin, damage, 1);
	ent->client->ps.gunframe++;
}
void Weapon_WEBPULLER(edict_t* ent)
{
	static int	pause_frames[] = { 10, 18, 27, 0 };
	static int	fire_frames[] = { 6, 0 };
	int prevstate;

	// if the the attack button is still down, stay in the firing frame
	if ((ent->client->buttons & BUTTON_ATTACK) &&
		ent->client->weaponstate == WEAPON_FIRING &&
		ent->client->WEB)
		ent->client->ps.gunframe = 9;

	if (!(ent->client->buttons & BUTTON_ATTACK) &&
		ent->client->WEB) {
		//sleep(4)
		ResetWEB(ent->client->WEB);
		if (ent->client->weaponstate == WEAPON_FIRING)
			ent->client->weaponstate = WEAPON_READY;
	}


	if (ent->client->newweapon &&
		ent->client->WEBstate > WEB_STATE_FLY &&
		ent->client->weaponstate == WEAPON_FIRING) {
		// he wants to change weapons while grappled
		ent->client->weaponstate = WEAPON_DROPPING;
		ent->client->ps.gunframe = 32;
	}

	prevstate = ent->client->weaponstate;
	Weapon_Generic(ent, 5, 9, 31, 36, pause_frames, fire_frames,
		Weapon_WEBpuller_Fire);

	// if we just switched back to grapple, immediately go to fire frame
	if (prevstate == WEAPON_ACTIVATING &&
		ent->client->weaponstate == WEAPON_READY &&
		ent->client->WEBstate > WEB_STATE_FLY) {
		if (!(ent->client->buttons & BUTTON_ATTACK))
			ent->client->ps.gunframe = 9;
		else
			ent->client->ps.gunframe = 5;
		ent->client->weaponstate = WEAPON_FIRING;
	}
}





//////////////////////////////////////////////////////////////////////////////////////////
//WEB SHOOTER


void WEBshooterFire(edict_t* ent, vec3_t g_offset, int damage, int effect)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	offset;
	float volume = 1.0;

	if (ent->client->WEBstate > WEB_STATE_FLY)
		return; // it's already out

	AngleVectors(ent->client->v_angle, forward, right, NULL);
	//	VectorSet(offset, 24, 16, ent->viewheight-8+2);
	VectorSet(offset, 24, 8, ent->viewheight - 8 + 2);
	VectorAdd(offset, g_offset, offset);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	if (ent->client->silencer_shots)
		volume = 0.2;

	gi.sound(ent, CHAN_RELIABLE + CHAN_WEAPON, gi.soundindex("weapons/web/grfire.wav"), volume, ATTN_NORM, 0);
	FireWEB(ent, start, forward, damage, WEB_SPEED * 2, effect);

#if 0
	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_BLASTER);
	gi.multicast(ent->s.origin, MULTICAST_PVS);
#endif

	PlayerNoise(ent, start, PNOISE_WEAPON);
}
void Weapon_WEBshooter_Fire(edict_t* ent)
{
	int		damage;

	damage = 500;
	WEBshooterFire(ent, vec3_origin, damage, 1);
	ent->client->ps.gunframe++;
}
void Weapon_WEBSHOOTER(edict_t* ent)
{
	static int	pause_frames[] = { 10, 18, 27, 0 };
	static int	fire_frames[] = { 6, 0 };
	int prevstate;

	// if the the attack button is still down, stay in the firing frame
	if ((ent->client->buttons & BUTTON_ATTACK) &&
		ent->client->weaponstate == WEAPON_FIRING &&
		ent->client->WEB)
		ent->client->ps.gunframe = 9;

	if (!(ent->client->buttons & BUTTON_ATTACK) &&
		ent->client->WEB) {
		ResetWEB(ent->client->WEB);
		if (ent->client->weaponstate == WEAPON_FIRING)
			ent->client->weaponstate = WEAPON_READY;
	}


	if (ent->client->newweapon &&
		ent->client->WEBstate > WEB_STATE_FLY &&
		ent->client->weaponstate == WEAPON_FIRING) {
		// he wants to change weapons while grappled
		ent->client->weaponstate = WEAPON_DROPPING;
		ent->client->ps.gunframe = 32;
	}

	prevstate = ent->client->weaponstate;
	Weapon_Generic(ent, 5, 9, 31, 36, pause_frames, fire_frames,
		Weapon_WEBshooter_Fire);

	// if we just switched back to grapple, immediately go to fire frame
	if (prevstate == WEAPON_ACTIVATING &&
		ent->client->weaponstate == WEAPON_READY &&
		ent->client->WEBstate > WEB_STATE_FLY) {
		if (!(ent->client->buttons & BUTTON_ATTACK))
			ent->client->ps.gunframe = 9;
		else
			ent->client->ps.gunframe = 5;
		ent->client->weaponstate = WEAPON_FIRING;
	}
}