
#include <raylib.h>
#include <raymath.h>

#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <string>

#include "map/loadMap.hpp"
#include "player/player.hpp"


int main(){
    const int screenWidth = 800;
    // const int screenHeight = 450;
    const int screenHeight = 800;

    // Make the window resizable before initializing it.
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Pixel Game");

    // Open the exported Tiled JSON map.
    std::ifstream file("./assets/beta-map.tmj");

    if (!file.is_open()) {
        TraceLog(LOG_INFO, "File not found");
        CloseWindow();
        return 1;
    }

    // Parse the JSON file.
    // The false argument prevents parsing errors from immediately throwing.
    json map = json::parse(file, nullptr, false);

    if(map.is_discarded()){
        TraceLog(LOG_INFO, "Invalid JSON/.tmj file");
        CloseWindow();
        return 1;
    }

    // ==========================================
    //  LOAD ALL TILESETS
    // ==========================================

    // Each tileset is stored inside this vector.
    // We need all of them because different map tiles
    // can belong to different tilesheets.
    std::vector<TileSheet> tilesheets;

    for (const auto& mapRender : map["tilesets"]) {

        // Read tileset information from the JSON.
        int firstgid = mapRender.value("firstgid", -1);
        int columns = mapRender.value("columns", -1);
        int tilecount = mapRender.value("tilecount", -1);

        // Build the image path.
        // Example: ./assets/tiles.png
        std::string imagePath = "./assets/";
        std::string image = imagePath + mapRender.value("image", "");

        TraceLog(LOG_INFO, "firstgid %d, columns %d, image %s", firstgid, columns, image.c_str());

        // Load the tilesheet image into GPU memory.
        Texture2D texture = LoadTexture(image.c_str());

        if(texture.id == 0){
            TraceLog(LOG_ERROR, "Could not load picture: %s", image.c_str());

            for(const TileSheet& sheet : tilesheets){
                UnloadTexture(sheet.texture);
            }

            CloseWindow();
            return 1;
        }

        // Create a TileSheet object and store its information.
        TileSheet tileSheetCard;
        tileSheetCard.firstgid = firstgid;
        tileSheetCard.columns = columns;
        tileSheetCard.tilecount = tilecount;
        tileSheetCard.texture = texture;

        // Add this tilesheet to our vector.
        tilesheets.push_back(tileSheetCard);
    }

    // Debug information about the loaded tilesheets.
    // TraceLog(LOG_INFO, "size: %d, firstgid: %d, columns: %d, texture: %d", (int) tilesheets.size(), tilesheets[7].firstgid, tilesheets[0].columns, tilesheets[0].texture.width);

    // ==========================================
    // 3. GET MAP DIMENSIONS
    // ==========================================

    // These are the number of tiles, NOT pixel dimensions.
    int mapWidth = map.value("width", -1);
    int mapHeight = map.value("height", -1);

    // TraceLog(LOG_INFO, "width: %d, height: %d",mapWidth, mapHeight);

    // ==========================================
    // Names of the components of the map
    // ==========================================
    std::vector<std::string> tileSets = {"Grass", "Ground", "Roof", "Trees", "Bushes", "Props", "PlayerCharacter"};


    // ==========================================
    // GAME SETTINGS
    // ==========================================

    Color BackgroundCharcoal = { 40, 40, 40, 255 };

    // ==========================================
    // PLAYER
    // ==========================================

    Player player;
    loadPlayerTexture(player);


    // =======================================
    // Camera
    // =======================================
    Camera2D camera = { 0 };
    camera.target = (Vector2){ player.position.x + 20.0f, player.position.y + 20.0f };
    camera.offset = (Vector2){ GetRenderWidth()/2.0f, GetRenderHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;

    // Sets the FPS
    SetTargetFPS(60);

    // ==========================================
    // 6. MAIN GAME LOOP
    // ==========================================

    while(!WindowShouldClose()){

        float deltaTime = GetFrameTime();

        // Save the player's position before movement.
        movePlayerWithKeyboard(player, deltaTime);

        // ==========================================
        // Camera angle that follows the player
        // ==========================================
        camera.target = (Vector2){ player.position.x + 20, player.position.y + 20 };

        // ==========================================
        // 7. DRAWING
        // ==========================================

        BeginDrawing();

        // Clear the previous frame.
        // This prevents old frame contents from remaining visible.
        ClearBackground(BackgroundCharcoal);

        BeginMode2D(camera);

        for(const auto &tileSetsName : tileSets){
            loadMap(map, tileSetsName, mapWidth, tilesheets);
        }

        // Draws the Player
        if(player.isPlayerWalking){
            DrawTextureRec(player.walkTexture, player.walkFrameRec, player.position, WHITE);
        } 

        else if(player.isFacingUp){
            DrawTextureRec(player.walkUpTexture, player.walkUpFrameRec, player.position, WHITE);
        }
        
        else if(player.isFacingDown){
            DrawTextureRec(player.walkDownTexture, player.walkDownFrameRec, player.position, WHITE);
        }

        else{
            DrawTextureRec(player.idleTexture, player.idleFrameRec,player.position,WHITE);
        }

        EndMode2D();

        DrawFPS(10, 10);

        // Finish drawing this frame.
        EndDrawing();
    }

    // ==========================================
    // 14. CLEANUP
    // ==========================================

    unloadPlayer(player);

    // Release every loaded texture from GPU memory.
    for(const TileSheet& sheet: tilesheets){
        UnloadTexture(sheet.texture);
    }

    CloseWindow();
    return 0;
}
