#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;




struct TileSheet {
    int firstgid;
    int columns;
    int tilecount;
    Texture2D texture;
};

void loadMap(
    const json &map,
    const std::string& tileName,
    int mapWidth,
    std::vector<TileSheet> &tilesheets
);