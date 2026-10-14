#ifndef multiline_h
#define multiline_h


#include "engine/logic/bbFlag.h"

//typedef bbFlag bbParseFunction(void* spawner, char* line);


bbFlag bbPF_NewEntity(void* spawner, char* line);
bbFlag bbPF_AddServerHandle(void* spawner, char* line);
bbFlag bbPF_AddMovement(void* spawner, char* line);
bbFlag bbPF_AddGraphics(void* spawner, char* line);
bbFlag bbPF_AddProjectile(void* spawner, char* line);
bbFlag bbPF_AddAI(void* spawner, char* line);
bbFlag bbPF_AddTeam(void* spawner, char* line);
bbFlag bbPF_AddHitpoints(void* spawner, char* line);
bbFlag bbPF_SetPlayer(void* spawner, char* line);


#endif //multiline_h