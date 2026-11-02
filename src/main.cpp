#include <raylib.h>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp> 
using json = nlohmann::json;
#include <string>

struct TileSheet
{
    int firstgid;
    int columns;
    Texture2D texture;
};



// Obstacle Collision
void obstacleCollision(Vector2 &pos, int playerSize, Rectangle obstacle, Vector2 currentPlayerPos){
    if(CheckCollisionCircleRec(pos, playerSize, obstacle)) {
        pos = currentPlayerPos;
    }
}



// void loadMap(Vector2 mapPosition){

    
// }


int main(){
    const int screenWidth = 800;
    const int screenHeight = 800;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);    // Window configuration flags
    InitWindow(screenWidth, screenHeight, "Pixel Game");

    // Loading Map
    std::ifstream file("./assets/beta-map.tmj");

    if (!file.is_open()) {
        TraceLog(LOG_INFO, "File not found");
        return 1;
    }

    json map = json::parse(file, nullptr, false);
    if(map.is_discarded()){
        TraceLog(LOG_INFO, "Invalod JSON/.tmj file");
        return 1;
    }

    std::vector<TileSheet> tilesheets;

    for (const auto& mapRen : map["tilesets"]) {
        int firstgid = mapRen.value("firstgid", -1);
        int columns = mapRen.value("columns", -1);
        std::string imagePath = "./assets/"; 
        std::string image = imagePath + mapRen.value("image", "");
        TraceLog(LOG_INFO, "firstgid %d, columns %d, image %s", firstgid, columns, image.c_str());
        Texture2D texture = LoadTexture(image.c_str());
        
        if(texture.id == 0){
            TraceLog(LOG_ERROR, "Could not load picture: %s", image.c_str());
            return 1;
        }

        TileSheet tileSheetCard;
        tileSheetCard.firstgid = firstgid;
        tileSheetCard.columns = columns;
        tileSheetCard.texture = texture;
        tilesheets.push_back(tileSheetCard);

    }

    TraceLog(LOG_INFO, "size: %d, firstgid: %d, columns: %d, texture: %d", (int) tilesheets.size(), tilesheets[7].firstgid, tilesheets[0].columns, tilesheets[0].texture.width);

    // Find the Ground
    for(const auto &layer : map["layers"]){
        std::string layerName = layer.value("name", "Unknown");
        if(layerName == "Ground"){
            TraceLog(LOG_INFO, ("Found The Ground"));
        }

    }



    
    SetTargetFPS(60);
    Color BackgroundCharcoal = { 40, 40, 40, 255 };

    // Map
    Vector2 mapPosition = {0, 0};

    // Player Variables
    // Start on the path below the house (tile column 20, row 33 -> pixel center 328, 536)
    Vector2 playerPosition = { 328, 536 };
    Color playerColor = MAROON;
    int playerSize = 10;            // radius of the collision circle (tile = 16, tune this)
    float playerMovement = 250.0;   // pixels per second
    Vector2 currentPlayerPos;       // position saved before moving (to undo a bad move)


    while(!WindowShouldClose()){

        float deltaTime = GetFrameTime();

        currentPlayerPos = playerPosition;

        // Keyboard Controls (left/right also remember which way he faces)
        if(IsKeyDown(KEY_RIGHT)){
            playerPosition.x += playerMovement * deltaTime;
        }
        if(IsKeyDown(KEY_LEFT)){
            playerPosition.x -= playerMovement * deltaTime;
        }
        if(IsKeyDown(KEY_UP)) playerPosition.y -= playerMovement * deltaTime;
        if(IsKeyDown(KEY_DOWN)) playerPosition.y += playerMovement * deltaTime;

   
        BeginDrawing();

        ClearBackground(BackgroundCharcoal);
        EndDrawing();
    }

    for(const TileSheet& sheet: tilesheets){
        UnloadTexture(sheet.texture);
    }
    CloseWindow();
    return 0;
}