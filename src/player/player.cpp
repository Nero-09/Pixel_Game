
#include "player.hpp"

// Load player textures and initialize player variables.
void loadPlayer(Player& player)
{
    player.idleTexture = LoadTexture(
        "assets/Pixel Crawler/Entities/Npc's/Citizen_F/Peasant_A/Idle/Idle-Sheet.png"
    );
 

    player.walkTexture = LoadTexture(
        "assets/Pixel Crawler/Entities/Npc's/Citizen_F/Peasant_A/Walk/Walk-Sheet.png"
    );


    player.position = {400, 225};
    player.movementSpeed = 70.0f;

    player.currentIdleFrame = 0;
    player.framesCounter = 0;
    player.framesSpeed = 4;

    player.idleFrameRec = {
        0.0f,
        0.0f,
        (float)player.idleTexture.width / 4,
        (float)player.idleTexture.height
    };

    player.walkFrameRec = {
        0.0f,
        0.0f,
        (float)player.walkTexture.width / 6,
        (float)player.walkTexture.height
    };
}


void movePlayerWithKeyboard(Player& player, float deltaTime)
{
    player.isPlayerWalking = false;
    // player.isFacingLeft = false;

    if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){
        player.position.x += player.movementSpeed * deltaTime;
        player.isPlayerWalking = true;
        player.isFacingLeft = false;
        player.framesCounter++;
    }

    if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)){
        player.position.x -= player.movementSpeed * deltaTime;
        player.isPlayerWalking = true;
        player.isFacingLeft = true;
        player.framesCounter++;
    }

    if(IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)){
        player.position.y -= player.movementSpeed * deltaTime;
        player.isPlayerWalking = true;
        player.framesCounter++;

    }

    if(IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
        player.position.y += player.movementSpeed * deltaTime;
        player.isPlayerWalking = true;
        player.framesCounter++;
    }

    player.framesCounter++;

    if(player.isPlayerWalking){
        
        if(player.framesCounter >= (60 / player.framesSpeed)){

            player.framesCounter = 0;
            player.currentWalkFrame++;

            if(player.currentWalkFrame > 5)
                player.currentWalkFrame = 0;

            player.walkFrameRec.x = (float)player.currentWalkFrame * player.walkFrameRec.width;

        }
    }

    // Walks left
    if(player.isFacingLeft){
        if(player.walkFrameRec.width > 0.0f)
            player.walkFrameRec.width *= -1.0f;
    }
    else if(player.walkFrameRec.width < 0.0f){
        player.walkFrameRec.width *= -1.0f;
    }

    // Idle animation
    if(player.framesCounter >= (60 / player.framesSpeed)){
        player.framesCounter = 0;
        player.currentIdleFrame++;

        if(player.currentIdleFrame > 3)
            player.currentIdleFrame = 0;

        player.idleFrameRec.x = (float)player.currentIdleFrame * player.idleFrameRec.width;
    }

}


// Release player textures.
void unloadPlayer(Player& player)
{
    UnloadTexture(player.idleTexture);
    UnloadTexture(player.walkTexture);
}
