
#pragma once

#include <raylib.h>

struct Player
{
    Texture2D idleTexture;
    Texture2D walkTexture;
    Texture2D walkUpTexture;
    Texture2D walkDownTexture;

    Rectangle idleFrameRec;
    Rectangle walkFrameRec;
    Rectangle walkUpFrameRec;
    Rectangle walkDownFrameRec;

    bool isPlayerWalking;
    bool isFacingLeft;
    bool isFacingUp;
    bool isFacingDown;
    
    int currentIdleFrame;
    int currentWalkFrame;
    int currentWalkUpFrame;
    int currentWalkDownFrame;
    int framesCounter;
    int framesSpeed;    

    Vector2 position;
    float movementSpeed;
    
};

void loadPlayerTexture(Player& player); //Idle animation of the Player
void movePlayerWithKeyboard(Player& player, float deltaTime);
void unloadPlayer(Player& player);
