#ifndef PLAYER_H
#define PLAYER_H

#include "include.h"

typedef struct Player {
    //camera
    Camera camera;

    //state
    bool isRunning;
    bool isOnGround;
    bool hasTargetBlock;
    Vector3 targetBlockWorld;
    Vector3 prevTargetBlockPos;

    //physics
    Vector3 velocity;
    float gravity;
    float moveSpeed;
    float sprintMultiplier;
    float jumpForce;
    float mouseSensitivity;

    //collision box
    Vector3 collisionSize;

}Player;

void InitPlayer(Player* player);

void UpdatePlayerInput(Player* player, ChunkTable* chunkTable);

void UpdatePlayerCamera(Player* player, float dt);

void UpdatePlayerMovement(Player* player, float dt);

void UpdatePlayer(Player* player, float dt);

#endif
