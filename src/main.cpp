#include <raylib.h>
#include <iostream>

void screenLimitCollision(Vector2 &pos, int size){

    if(pos.x >= GetScreenWidth() - size)      
    pos.x = GetScreenWidth() - size;
            
    // Left
    if(pos.x <= size)
    pos.x = 0 + size;

    // Down
    if(pos.y >= GetScreenHeight() - size) 
        pos.y = GetScreenHeight() - size;
        // fallOneSpeed *= -0.8f;

    // Up
    if( pos.y <= size)
    pos.y = 0 + size; 
}

// Obstacle Collision
void obstacleCollision(Vector2 &pos, int playerSize, Rectangle obstacle, Vector2 currentPlayerPos){
    if(CheckCollisionCircleRec(pos, playerSize, obstacle)) {
        pos = currentPlayerPos;
    }
}

int main(){
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Pixel Game");
    SetTargetFPS(60);


    // Player Variables
    Vector2 playerPosition = { GetScreenWidth() / 2, GetScreenHeight() / 2};
    Color playerColor = MAROON;
    int playerSize = 20;
    float playerMovement = 400.0;
    Vector2 currentPlayerPos;

    // Npc 1
    Vector2 npcPosition = {25, 573};
    int npcSize = 20;
    Color npcColor = YELLOW;


    // Obstacles variables in Rectangle shape
    // Obstacle 1
    Vector2 obstacle1Position = {50, 50};
    Color obstacleColor = BLUE;
    Vector2 obstacleSize = {100, 500};
    Rectangle obstacle1 = {obstacle1Position.x, obstacle1Position.y,obstacleSize.x, obstacleSize.y};

    // Obstacle 2
    Vector2 obstacle2Position = {580, 58};
    Color obstacle2Color = BLUE;
    Vector2 obstacle2Size = {60, 180};
    Rectangle obstacle2 = {obstacle2Position.x, obstacle2Position.y,obstacle2Size.x, obstacle2Size.y};

    // Obstacle 3
    Vector2 obstacle3Position = {317, 143};
    Color obstacle3Color = BLUE;
    Vector2 obstacle3Size = {290, 40};
    Rectangle obstacle3 = {obstacle3Position.x, obstacle3Position.y,obstacle3Size.x, obstacle3Size.y};


    // Obstacle 4
    Vector2 obstacle4Position = {240, 420};
    Color obstacle4Color = BLUE;
    Vector2 obstacle4Size = {190, 50};
    Rectangle obstacle4 = {obstacle4Position.x, obstacle4Position.y,obstacle4Size.x, obstacle4Size.y};

    // Obstacle 5
    Vector2 obstacle5Position = {650, 370};
    Color obstacle5Color = BLUE;
    Vector2 obstacle5Size = {90, 300};
    Rectangle obstacle5 = {obstacle5Position.x, obstacle5Position.y,obstacle5Size.x, obstacle5Size.y};


    while(!WindowShouldClose()){

    float deltaTime = GetFrameTime();

        currentPlayerPos = playerPosition;

        // Keyboard Controls
        if(IsKeyDown(KEY_RIGHT)) playerPosition.x += playerMovement * deltaTime;
        if(IsKeyDown(KEY_LEFT)) playerPosition.x -= playerMovement * deltaTime;
        if(IsKeyDown(KEY_UP)) playerPosition.y -= playerMovement * deltaTime;
        if(IsKeyDown(KEY_DOWN)) playerPosition.y += playerMovement * deltaTime;
         
        screenLimitCollision(playerPosition, playerSize);

        // Finding Coordinates for obstacles
        std::cout << "X: " << GetMouseX() <<::std::endl;
        std::cout << "Y: " << GetMouseY() <<::std::endl;

        
        // Obsctacle Collision
        obstacleCollision(playerPosition, playerSize, obstacle1, currentPlayerPos);
        obstacleCollision(playerPosition, playerSize, obstacle2, currentPlayerPos);
        obstacleCollision(playerPosition, playerSize, obstacle3, currentPlayerPos);
        obstacleCollision(playerPosition, playerSize, obstacle4, currentPlayerPos);
        obstacleCollision(playerPosition, playerSize, obstacle5, currentPlayerPos);

        
        BeginDrawing();

        ClearBackground({ 40, 40, 40, 255 });

        // Player
        DrawCircleV(playerPosition, playerSize,playerColor);
        DrawText("Find the Yellow Ball", 300, 50, 20, MAROON);

        // Npc
        DrawCircleV(npcPosition, npcSize, npcColor);


        // Npc Interaction
        if (CheckCollisionCircles(playerPosition, playerSize, npcPosition, npcSize)){
            DrawText("HI, Congratulation You Found Me.", 250, 295, 20, YELLOW);
        }

        // Obstacles
        DrawRectangleV(obstacle1Position, obstacleSize, obstacleColor);
        DrawRectangleV(obstacle2Position, obstacle2Size, obstacle2Color);
        DrawRectangleV(obstacle3Position, obstacle3Size, obstacle3Color);
        DrawRectangleV(obstacle4Position, obstacle4Size, obstacle4Color);
        DrawRectangleV(obstacle5Position, obstacle5Size, obstacle5Color);
   
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
