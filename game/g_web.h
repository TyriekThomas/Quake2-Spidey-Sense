/*
Spidey MOD
Web Script
Tyriem Thomas
Three spidey weaponsThree spidey weapons
*/



#define WEB_SPEED					1050 // speed of grapple in flight
#define WEB_PULL_SPEED				1500	// speed player is pulled at
//spidey_enemycount = 0;
typedef enum {
	WEB_STATE_FLY,
	WEB_STATE_PULL,
	WEB_STATE_HANG
} WEBstate_t;

static void P_ProjectSource(gclient_t* client, vec3_t point, vec3_t distance, vec3_t forward, vec3_t right, vec3_t result)
{
	vec3_t	_distance;

	VectorCopy(distance, _distance);
	if (client->pers.hand == LEFT_HANDED)
		_distance[1] *= -1;
	else if (client->pers.hand == CENTER_HANDED)
		_distance[1] = 0;
	G_ProjectSource(point, _distance, forward, right, result);
}
qboolean ApplyHaste(edict_t* ent);

// WEB
void Weapon_WEB (edict_t *ent);
void PlayerResetWEB(edict_t *ent);
void WEBPull(edict_t *self);
void ResetWEB(edict_t *self);

//WEBPULLER
void Weapon_WEBPULLER(edict_t* ent);
//void PlayerResetWEB(edict_t* ent);
void WEBPuller(edict_t* self);
//void ResetWEB(edict_t* self);

//WEBSHOOTER
void Weapon_WEBSHOOTER(edict_t* ent);
//void PlayerResetWEB(edict_t* ent);
void WEBSHOOTER(edict_t* self);
//void ResetWEB(edict_t* self);

