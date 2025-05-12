#pragma once
#include <string>
#include <mutex>
#include <vector>
#include "include/imgui/imgui.h"
#include <nlohmann/json.hpp>
#include <Windows.h>
#include <Shlobj.h>
#include <fstream>
#include <curl/curl.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "advapi32.lib")

#define IM_PI 3.14159265358979323846f

using json = nlohmann::json;
namespace fs = std::filesystem;

// Forward declaration for EspBlock structure
struct EspBlock {
    std::string name;
    ImVec4 color;
    bool enabled;

    // Constructor to initialize a block with a name and a color
    EspBlock(const std::string& blockName, const ImVec4& blockColor, const bool blockEnabled)
        : name(blockName), color(blockColor), enabled(blockEnabled) {
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
    inline static ImVec4 blockColor = ImVec4(1.f, 1.f, 1.f, 1.f);
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
    inline static bool autoTotem;
    inline static int autoTotemDelay;
    inline static int autoTotemHumanity;
    inline static bool triggerAutoSell;
	inline static bool autoSell;
    inline static int autoSellDelay;
	inline static char autoSellPrice[256] = "";
    inline static int autoSellEndpoints[2] = { 1, 9 };
    inline static bool autoDc;
	inline static bool autoDcPrimed;
	inline static float autoDcCondition;
	inline static bool backgroundFx;
    inline static bool chatFilter;
    inline static const char* chatFilterItems[] = { "Mute", "Contains", "Not Contain" };
    inline static int filterMode = 0;
    inline static char blockMsg[MAX_PATH];

    inline static bool autoCrystalcfg;
	inline static bool advEspcfg;
	inline static bool autoSellcfg;
	inline static bool autoDccfg;
	inline static bool oreSimcfg;
    inline static bool autoTotemcfg;
	inline static bool chatFiltercfg;
};

static std::string getMinecraftFolder() {
    char appdata[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appdata);
    std::filesystem::path minecraftPath = std::filesystem::path(appdata) / ".minecraft";
    return minecraftPath.string();
}

using json = nlohmann::json;

static int getPort() {
    std::string path = getMinecraftFolder() + "\\packetedit-port.json";
    std::ifstream file(path.c_str());

    if (!file.is_open()) return 1337;

    try {
        json config;
        file >> config;
        file.close();

        if (config.contains("port")) {
            return config["port"].get<int>();
        }
    }
    catch (...) {}

    return 1337;
}

// cURL callback for writing response
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    try {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    catch (...) {
        return 0; // Signal error to cURL
    }
}

static std::string escapeJson(const std::string& input) {
    std::string output;
    for (char c : input) {
        switch (c) {
        case '"': output += "\\\""; break;
        case '\\': output += "\\\\"; break;
        case '\b': output += "\\b"; break;
        case '\f': output += "\\f"; break;
        case '\n': output += "\\n"; break;
        case '\r': output += "\\r"; break;
        case '\t': output += "\\t"; break;
        default: output += c;
        }
    }
    return output;
}

static void RenderCustomShaders() {
    float time = ImGui::GetTime();
    float pulse = 0.5f + 0.5f * sinf(time * 3.5f);
    float pulse_fast = 0.5f + 0.5f * sinf(time * 8.0f);
    float flicker = 0.3f + 0.7f * fabsf(sinf(time * 20.0f));

    // Alpha multiplier to control overall visibility (0.0f - 1.0f)
    const float alphaMultiplier = 0.3f; // Reduced from original full intensity

    // === Cyberpunk Color Palette ===
    ImVec4 neon_pink = ImVec4(1.0f, 0.1f, 0.6f, 1.0f * alphaMultiplier);
    ImVec4 neon_blue = ImVec4(0.1f, 0.9f, 1.0f, 1.0f * alphaMultiplier);
    ImVec4 neon_green = ImVec4(0.1f, 1.0f, 0.4f, 1.0f * alphaMultiplier);
    ImVec4 neon_purple = ImVec4(0.7f, 0.3f, 1.0f, 1.0f * alphaMultiplier);
    ImVec4 yellow_glow = ImVec4(1.0f, 1.0f, 0.1f, 1.0f * alphaMultiplier);

    ImDrawList* bg = ImGui::GetBackgroundDrawList();
    ImVec2 size = ImGui::GetIO().DisplaySize;
    ImVec2 center = ImVec2(size.x / 2.0f, size.y / 2.0f);

    // === Plasma Glow Background ===
    bg->AddRectFilledMultiColor(
        ImVec2(0, 0), size,
        ImGui::ColorConvertFloat4ToU32(ImVec4(neon_pink.x, neon_pink.y, neon_pink.z, (0.05f + 0.03f * pulse) * alphaMultiplier)),
        ImGui::ColorConvertFloat4ToU32(ImVec4(neon_green.x, neon_green.y, neon_green.z, (0.05f + 0.03f * pulse_fast) * alphaMultiplier)),
        ImGui::ColorConvertFloat4ToU32(ImVec4(neon_purple.x, neon_purple.y, neon_purple.z, (0.07f + 0.04f * pulse) * alphaMultiplier)),
        ImGui::ColorConvertFloat4ToU32(ImVec4(neon_blue.x, neon_blue.y, neon_blue.z, (0.05f + 0.03f * pulse_fast) * alphaMultiplier))
    );

    // === Color Bleed Frame Bloom ===
    float edge_thickness = 20.0f;
    bg->AddRectFilled(ImVec2(0, 0), ImVec2(size.x, edge_thickness), IM_COL32(100, 255, 180, (int)(25 * alphaMultiplier)), 0); // Top
    bg->AddRectFilled(ImVec2(0, size.y - edge_thickness), ImVec2(size.x, size.y), IM_COL32(255, 100, 200, (int)(25 * alphaMultiplier)), 0); // Bottom
    bg->AddRectFilled(ImVec2(0, 0), ImVec2(edge_thickness, size.y), IM_COL32(150, 255, 255, (int)(25 * alphaMultiplier)), 0); // Left
    bg->AddRectFilled(ImVec2(size.x - edge_thickness, 0), ImVec2(size.x, size.y), IM_COL32(255, 255, 150, (int)(25 * alphaMultiplier)), 0); // Right

    // === Grid Distortion Offset ===
    float grid_spacing = 100.0f;
    for (float x = fmodf(time * 10, grid_spacing); x < size.x; x += grid_spacing) {
        float offset = sinf(time * 2.0f + x * 0.1f) * 5.0f;
        bg->AddLine(ImVec2(x + offset, 0), ImVec2(x + offset, size.y), IM_COL32(50, 255, 120, (int)((20 + 20 * pulse) * alphaMultiplier)), 1.0f);
    }
    for (float y = fmodf(time * 10, grid_spacing); y < size.y; y += grid_spacing) {
        float offset = cosf(time * 2.0f + y * 0.1f) * 5.0f;
        bg->AddLine(ImVec2(0, y + offset), ImVec2(size.x, y + offset), IM_COL32(50, 255, 120, (int)((20 + 20 * pulse) * alphaMultiplier)), 1.0f);
    }

    // === Spinning Holographic Radials ===
    for (int i = 0; i < 36; ++i) {
        float angle = time + i * (IM_PI / 18);
        ImVec2 dir = ImVec2(cosf(angle), sinf(angle));
        bg->AddLine(center, ImVec2(center.x + dir.x * 500.0f, center.y + dir.y * 500.0f),
            IM_COL32(50, 200, 255, (int)(30 * alphaMultiplier)), 1.0f);
    }

    // === Concentric Pulse Rings ===
    for (int i = 1; i < 4; ++i) {
        float radius = 50.0f + i * 60.0f + (pulse * 10.0f);
        ImU32 ring_col = IM_COL32((int)(pulse * 255), 100 + i * 40, 255, (int)((30 + i * 5) * alphaMultiplier));
        bg->AddCircle(center, radius, ring_col, 64, 1.5f);
    }

    // === Scanlines ===
    float scan_spacing = 5.0f;
    float scroll_offset = fmodf(time * 30.0f, scan_spacing);
    for (float y = -scan_spacing; y < size.y; y += scan_spacing) {
        bg->AddLine(ImVec2(0, y + scroll_offset), ImVec2(size.x, y + scroll_offset),
            IM_COL32(0, 255, 200, (int)((25 + 25 * pulse) * alphaMultiplier)), 2.0f);
    }

    // === Chromatic Aberration Glitch Lines ===
    for (int y = 0; y < size.y; y += 120) {
        bg->AddLine(ImVec2(0, y), ImVec2(size.x, y), IM_COL32(255, 50, 200, (int)(30 * alphaMultiplier)), 1.0f);
        bg->AddLine(ImVec2(2, y + 1), ImVec2(size.x + 2, y + 1), IM_COL32(0, 255, 255, (int)(30 * alphaMultiplier)), 1.0f);
        bg->AddLine(ImVec2(-2, y + 2), ImVec2(size.x - 2, y + 2), IM_COL32(0, 255, 100, (int)(20 * alphaMultiplier)), 1.0f);
    }

    // === Noise Dots ===
    for (int i = 0; i < 100; ++i) {
        float rx = fmodf(sinf(time * i) * 10000.0f, size.x);
        float ry = fmodf(cosf(time * i * 1.5f) * 8000.0f, size.y);
        int r = 100 + rand() % 155;
        int g = 200 + rand() % 55;
        int b = 200 + rand() % 55;
        bg->AddCircleFilled(ImVec2(rx, ry), 1.5f,
            IM_COL32(r, g, b, (int)((80 * flicker) * alphaMultiplier)));
    }
}
