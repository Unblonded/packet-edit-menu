#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include <vector>
#include "include/imgui_hook.h"
#include "include/imgui/imgui.h"

// Forward declaration for EspBlock structure
struct EspBlock {
    std::string name;
    ImVec4 color;

    // Constructor to initialize a block with a name and a color
    EspBlock(const std::string& blockName, const ImVec4& blockColor)
        : name(blockName), color(blockColor) {
    }
};

// Configuration class with inline static members
class cfg {
public:
    // Delete default constructor to prevent instantiation
    cfg() = delete;

    // inline static configuration variables
    inline static bool showMenu;
    inline static bool showAll;
    inline static bool displayPlayers;
    inline static bool advEsp;
    inline static bool drawBlocks;
    inline static bool drawBlockTracer;
    inline static char blockName[256];
    inline static ImVec4 blockColor;
    inline static std::vector<EspBlock> espBlockList;
    inline static int espRadius;
    inline static int espBatchSize;
    inline static int espSearchTime;
    inline static bool checkPlayerAirSafety;
    inline static std::string isPlayerAirSafe;
    inline static bool forwardTunnel;
    inline static std::vector<std::string> nearbyPlayers;
    inline static std::string tunnelBlockStatus;
    inline static bool autoCrystal;
    inline static int crystalAttackTime;
    inline static int crystalPlaceTime;
    inline static bool cancelInteraction;
    inline static bool showMenuInitialized;
    inline static bool autoAnchor;
    inline static long long oreSimSeed;
    inline static bool oreSim;
    inline static int oreSimDistance;
    inline static ImVec4 oreSimColor;
};