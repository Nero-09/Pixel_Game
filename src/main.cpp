#include <raylib.h>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp> 
using json = nlohmann::json;
#include <string>

// Represents one tilesheet from Tiled.

// A map can use multiple tilesheets. Each tilesheet has its own
// range of global tile IDs (GIDs).

// firstgid  = first global ID belonging to this tilesheet
// columns   = number of tiles horizontally in the tilesheet
// tilecount = total number of tiles in the tilesheet
// texture   = the actual loaded image
struct TileSheet
{
    int firstgid;
    int columns;
    int tilecount;
    Texture2D texture;
};


// Obstacle Collision
void obstacleCollision(Vector2 &pos, int playerSize, Rectangle obstacle, Vector2 currentPlayerPos){
    if(CheckCollisionCircleRec(pos, playerSize, obstacle)) {
        // If collision happens, restore the player's previous position.
        pos = currentPlayerPos;
    }
}

void loadMap(const json &map, const std::string& tileName, int mapWidth, std::vector<TileSheet> &tilesheets){

     for(const auto &layer : map["layers"]){
            // Read the current layer's name.
            std::string layerName = layer.value("name", "Unknown");

            // Gets idividual layer name
            if(layerName == tileName){

                // Get the array containing all tile IDs in this layer.
                const auto &cells = layer["data"];

                // TraceLog(LOG_INFO, "Ground cells: %d", (int)cells.size());
                // TraceLog(LOG_INFO, ("Found The Ground"));

                // ==========================================
                // 9. READ EACH CELL
                // ==========================================

                // i is the flattened array index.
                // Each index represents one tile position in the map.
                for (int i = 0; i < cells.size(); ++i) {

                    // Read the global tile ID (GID).
                    int id = cells[i].get<int>();

                    // ID 0 means this cell is empty.
                    if(id > 0){

                        // Convert the flattened array index into map coordinates.
                        //
                        // Example:
                        // mapWidth = 10, i = 23
                        // row = 23 / 10 = 2
                        // column = 23 % 10 = 3
                        //
                        // So this tile is at column 3, row 2.
                        int row = i / mapWidth;
                        int column = i % mapWidth;

                        // TraceLog(LOG_INFO,"Row: %d, Column: %d", row, column);

                        // ==========================================
                        // 10. FIND THE CORRECT TILESET
                        // ==========================================

                        // Check every loaded tilesheet.
                        for (const TileSheet& sheet : tilesheets) {

                            // Check whether this GID belongs to this tilesheet.
                            //
                            // Valid IDs:
                            // firstgid <= id < firstgid + tilecount
                            //
                            // This prevents selecting the wrong tilesheet.
                            if(id >=sheet.firstgid && id < sheet.firstgid + sheet.tilecount){

                                // ==========================================
                                // 11. CONVERT GLOBAL ID TO LOCAL ID
                                // ==========================================

                                // GID is global across the entire map.
                                // localId is the tile's index inside this tilesheet.
                                int localId = id - sheet.firstgid;

                                // Convert localId into tilesheet coordinates.
                                int tileRow = localId / sheet.columns;
                                int tileColumn = localId % sheet.columns;

                                // ==========================================
                                // 12. SOURCE COORDINATES
                                // ==========================================

                                // Find the pixel position of the tile
                                // INSIDE the tilesheet image.
                                // Each tile is 16x16 pixels.
                                int sourceX = tileColumn * 16;
                                int sourceY = tileRow * 16;

                                // ==========================================
                                // 13. DESTINATION COORDINATES
                                // ==========================================

                                // Find where the tile should appear
                                // INSIDE the game window.
                                //
                                // Map coordinates are converted into pixels.
                                int destX = column * 16;
                                int destY = row * 16;

                                Vector2 position = { (float)destX, (float)destY };

                                // Rectangle selects the tile's pixels
                                // from the original tilesheet.
                                Rectangle groundRec = {(float)sourceX, (float)sourceY, 16.0f, 16.0f};

                                // Draw the selected part of the tilesheet
                                // at the destination position.
                                DrawTextureRec(sheet.texture, groundRec, position, WHITE);
                            }
                        }
                    }
                }
            }
        }



}

int main(){
    const int screenWidth = 800;
    const int screenHeight = 800;

    // Make the window resizable before initializing it.
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Pixel Game");

    // Open the exported Tiled JSON map.
    std::ifstream file("./assets/beta-map.tmj");

    if (!file.is_open()) {
        TraceLog(LOG_INFO, "File not found");
        return 1;
    }

    // Parse the JSON file.
    // The false argument prevents parsing errors from immediately throwing.
    json map = json::parse(file, nullptr, false);

    if(map.is_discarded()){
        TraceLog(LOG_INFO, "Invalod JSON/.tmj file");
        return 1;
    }

    // ==========================================
    // 2. LOAD ALL TILESETS
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

        // TraceLog(LOG_INFO, "firstgid %d, columns %d, image %s", firstgid, columns, image.c_str());

        // Load the tilesheet image into GPU memory.
        Texture2D texture = LoadTexture(image.c_str());

        if(texture.id == 0){
            TraceLog(LOG_ERROR, "Could not load picture: %s", image.c_str());
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
    // 5. GAME SETTINGS
    // ==========================================

    SetTargetFPS(60);
    Color BackgroundCharcoal = { 40, 40, 40, 255 };


    // Player Variables
    Vector2 playerPosition = {0,0};
    // Vector2 playerPosition = { GetScreenWidth() / 2, GetScreenHeight() / 2 };
    Color playerColor = MAROON;
    int playerSize = 10;
    float playerMovement = 250.0;
    Vector2 currentPlayerPos;

    // ==========================================
    // 6. MAIN GAME LOOP
    // ==========================================

    while(!WindowShouldClose()){

        float deltaTime = GetFrameTime();

        // Save the player's position before movement.
        currentPlayerPos = playerPosition;

        // Keyboard Controls
        if(IsKeyDown(KEY_RIGHT)){
            playerPosition.x += playerMovement * deltaTime;
        }
        if(IsKeyDown(KEY_LEFT)){
            playerPosition.x -= playerMovement * deltaTime;
        }
        if(IsKeyDown(KEY_UP)) playerPosition.y -= playerMovement * deltaTime;
        if(IsKeyDown(KEY_DOWN)) playerPosition.y += playerMovement * deltaTime;

        // ==========================================
        // 7. DRAWING
        // ==========================================

        BeginDrawing();

        // Clear the previous frame.
        // This prevents old frame contents from remaining visible.
        ClearBackground(BackgroundCharcoal);

 
        // Loads all the tile sets for the map
        loadMap(map, "Grass", mapWidth, tilesheets);
        loadMap(map, "Ground", mapWidth, tilesheets);
        loadMap(map, "Roof", mapWidth, tilesheets);
        loadMap(map, "Bushes", mapWidth, tilesheets);
        loadMap(map, "Props", mapWidth, tilesheets);
        DrawFPS(10, 10);

        // Finish drawing this frame.
        EndDrawing();
    }

    // ==========================================
    // 14. CLEANUP
    // ==========================================

    // Release every loaded texture from GPU memory.
    for(const TileSheet& sheet: tilesheets){
        UnloadTexture(sheet.texture);
    }

    CloseWindow();
    return 0;
}