#include "loadMap.hpp"

// Represents one tilesheet from Tiled.



// A map can use multiple tilesheets. Each tilesheet has its own
// range of global tile IDs (GIDs).

// firstgid  = first global ID belonging to this tilesheet
// columns   = number of tiles horizontally in the tilesheet
// tilecount = total number of tiles in the tilesheet
// texture   = the actual loaded image

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

