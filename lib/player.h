#ifndef PLAYER_H
#define PLAYER_H

#include "include.h"

typedef struct Player {
    bool hasTargetBlock;
    Vector3 targetBlockWorld;
    Vector3 prevTargetBlockPos;
    Camera camera;

}Player;

void InitPlayer(Player* player);

void UpdatePlayerInput(Player* player, ChunkTable* chunkTable);

#endif
