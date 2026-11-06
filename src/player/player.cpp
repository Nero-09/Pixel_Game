#include "player.hpp"
#include <math.h>
// Load player textures and initialize player variables.
void loadPlayerTexture(Player& player)
{


     player.idleTexture = LoadTexture(
        "assets/custom/idle.png"
    );
 
    player.walkTexture = LoadTexture(
        "assets/custom/walk.png"
    );

    player.walkUpTexture = LoadTexture(
        "assets/custom/walk_up.png"
    );

    player.walkDownTexture = LoadTexture(
        "assets/custom/walk_down.png"
    );



    player.position = {400, 225};
    player.movementSpeed = 50.0f;

    player.currentIdleFrame = 0;
    player.framesCounter = 0;
    player.framesSpeed = 5;

    // Walk
    player.currentWalkFrame = 0;

    player.isFacingLeft = false;
    player.isPlayerWalking = false;
    player.isFacingUp = false;
    player.isFacingDown = false;
    
    player.currentWalkDownFrame = 0;
    player.currentWalkUpFrame = 0;
 
    player.idleFrameRec = {
        0.0f,
        0.0f,
        (float)player.idleTexture.width / 6,
        (float)player.idleTexture.height
    };

    player.walkFrameRec = {
        0.0f,
        0.0f,
        (float)player.walkTexture.width / 6,
        (float)player.walkTexture.height
    };

    player.walkUpFrameRec = {
        0.0f,
        0.0f,
        (float)player.walkUpTexture.width / 6,
        (float)player.walkUpTexture.height 
    };

    player.walkDownFrameRec = {
        0.0f,
        0.0f,
        (float)player.walkDownTexture.width / 6,
        (float)player.walkDownTexture.height 
    };
    
}

void movePlayerWithKeyboard(Player& player, float deltaTime)
{
    player.isPlayerWalking = false;    
    // player.isFacingUp = false;
    

    if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){
        player.position.x += player.movementSpeed * deltaTime;
        player.isPlayerWalking = true;
        player.isFacingLeft = false;
        player.isFacingUp = false;
        player.isFacingDown = false;
    }

    if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)){
        player.position.x -= player.movementSpeed * deltaTime;
        player.isPlayerWalking = true;
        player.isFacingLeft = true;
        player.isFacingUp = false;
        player.isFacingDown = false;
    }

    if(IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)){
        player.position.y -= player.movementSpeed * deltaTime;
        player.isPlayerWalking = false;
        player.isFacingLeft = false;
        player.isFacingDown = false;
        player.isFacingUp = true;
    }

    if(IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
        player.position.y += player.movementSpeed * deltaTime;
        player.isPlayerWalking = false;
        player.isFacingUp = false;
        player.isFacingDown = true;


    }

    player.framesCounter++;

    // Checks if Player is Walking
    if(player.isPlayerWalking){
        
        if(player.framesCounter >= (60 / player.framesSpeed)){

            player.framesCounter = 0;
            player.currentWalkFrame++;

            if(player.currentWalkFrame > 5)
                player.currentWalkFrame = 0;

            player.walkFrameRec.x = (float)player.currentWalkFrame * player.walkFrameRec.width;
        }
    }

    if(player.isFacingUp){
        
        if(player.framesCounter >= (60 / player.framesSpeed)){

            player.framesCounter = 0;
            player.currentWalkUpFrame++;

            if(player.currentWalkUpFrame > 5)
                player.currentWalkUpFrame = 0;

            player.walkUpFrameRec.x = (float)player.currentWalkUpFrame * player.walkUpFrameRec.width;
        }
    }

       if(player.isFacingDown){
        
        if(player.framesCounter >= (60 / player.framesSpeed)){

            player.framesCounter = 0;
            player.currentWalkDownFrame++;

            if(player.currentWalkDownFrame > 5)
                player.currentWalkDownFrame = 0;

            player.walkDownFrameRec.x = (float)player.currentWalkDownFrame * player.walkDownFrameRec.width;
        }
    }

    

    // Flip the sprite frames horizontally when the player faces left so both walking and idle animations match the direction.
    if (player.isFacingLeft) {
        player.walkFrameRec.width = -fabsf(player.walkFrameRec.width);
        player.idleFrameRec.width = -fabsf(player.idleFrameRec.width);
    }
    else {
        player.walkFrameRec.width = fabsf(player.walkFrameRec.width);
        player.idleFrameRec.width = fabsf(player.idleFrameRec.width);
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