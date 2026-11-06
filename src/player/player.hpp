
#pragma once

#include <raylib.h>

struct Player
{
    Texture2D idleTexture;
    Rectangle idleFrameRec;
    int currentIdleFrame;


    Texture2D walkTexture;
    Rectangle walkFrameRec;    
    bool isPlayerWalking;
    int currentWalkFrame;

    bool isFacingLeft;

    Vector2 position;
    float movementSpeed;


    int framesCounter;
    int framesSpeed;
};

void loadPlayer(Player& player); //Idle animation of the Player
void movePlayerWithKeyboard(Player& player, float deltaTime);
void unloadPlayer(Player& player);
