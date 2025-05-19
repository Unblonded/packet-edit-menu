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

static ImVec4 defaultColor() {
    return ImVec4(1.f, 1.f, 1.f, 1.f);
}

struct EspBlock {
    std::string name;
    ImVec4 color;
    bool enabled;

    // Constructor to initialize a block with a name and a color
    EspBlock(const std::string& blockName, const ImVec4& blockColor, const bool blockEnabled)
        : name(blockName), color(blockColor), enabled(blockEnabled) {
    }
};

struct PlayerInfo {
    std::string name;
    float distance;
    std::array<std::string, 4> armor; // helmet, chest, legs, boots
    std::string mainhand;
    std::string offhand;
    float health;
    int armorTuffness;
    bool isSneaking;
    bool isSprinting;
};



// Configuration class with inline static members
class cfg {
public:
    // Delete default constructor to prevent instantiation
    cfg() = delete;

    // inline static configuration variables
    inline static bool showMenu;
    inline static bool showAll;
    inline static bool displayPlayers = false;
    inline static bool advEsp = false;
    inline static bool drawBlocks = true;
    inline static bool drawBlockTracer = true;
    inline static char blockName[256];
    inline static ImVec4 blockColor = defaultColor();
    inline static std::vector<EspBlock> espBlockList;
    inline static int espRadius = 24;
    inline static int espBatchSize = 512;
    inline static int espSearchTime = 5;
    inline static bool checkPlayerAirSafety = false;
    inline static std::string isPlayerAirSafe;
    inline static bool isPlayerAirSafeShowStatus;
    inline static bool forwardTunnel;
    inline static std::vector<PlayerInfo> nearbyPlayers;
    inline static bool freezePlayers;
    inline static std::string tunnelBlockStatus;
    inline static bool autoCrystal;
    inline static int crystalAttackTime = 96;
    inline static int crystalPlaceTime = 105;
    inline static bool cancelInteraction = false;
    inline static bool showMenuInitialized;
    inline static bool autoAnchor = true;
    inline static long long oreSimSeed = 6608149111735331168LL;
    inline static bool oreSim = false;
    inline static int oreSimDistance = 5;
    inline static ImVec4 oreSimColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    inline static bool autoTotem = true;
    inline static int autoTotemDelay = 84;
    inline static int autoTotemHumanity = 29;
    inline static bool triggerAutoSell;
    inline static bool autoSell = false;
    inline static int autoSellDelay = 227;
    inline static char autoSellPrice[256] = "";
    inline static int autoSellEndpoints[2] = { 7, 9 };
    inline static bool autoDc = false;
    inline static bool autoDcPrimed;
    inline static float autoDcCondition = 5.0f;
    inline static bool backgroundFx = true;
    inline static bool chatFilter = true;
    inline static const char* chatFilterItems[] = { "Mute", "Contains", "Not Contain" };
    inline static int filterMode = 2;
    inline static char blockMsg[MAX_PATH] = "";
    inline static bool fontSizeOverride = false;
    inline static float fontSize = 2.592f;
    inline static bool aimAssistToggle = false;
    inline static bool storageScan = true;
    inline static ImVec4 storageScanColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    inline static char storageScanSearch[256] = "";
    inline static bool storageScanShowInGui = true;
    inline static bool storageScanShow;
    inline static float aimAssistrange = 6.800000190734863f;
    inline static float aimAssistfov = 128.1999969482422f;
    inline static float aimAssistsmoothness = 1.0f;
    inline static float aimAssistminSpeed = 143.0f;
    inline static float aimAssistmaxSpeed = 146.0f;
    inline static bool aimAssistvisibilityCheck = true;
    inline static int aimAssistupdateRate = 1;
    inline static bool crystalSpam = false;
    inline static int crystalSpamSearchRadius = 2;
    inline static int crystalSpamBreakDelay = 118;
    inline static bool nightFx = false;
    inline static float nightFxSize = 20.f;
    inline static bool nightFxDraw;
	inline static bool nightFxCrosshairLines = true;


    inline static bool autoCrystalcfg;
	inline static bool advEspcfg;
    inline static bool checkPlayerAirSafetycfg;
	inline static bool autoSellcfg;
	inline static bool autoDccfg;
	inline static bool oreSimcfg;
    inline static bool autoTotemcfg;
	inline static bool chatFiltercfg;
    inline static bool fontSizecfg;
	inline static bool aimAssistcfg;
	inline static bool storageScancfg;
    inline static bool crystalSpamcfg;
    inline static bool nightFxcfg;
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

static std::array<float, 4> vec4Arr(const ImVec4& color) {
    return { color.x, color.y, color.z, color.w };
}


static void RenderCustomShaders() {
    float time = static_cast<float>(ImGui::GetTime());
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
    for (float y = 0; y < size.y; y += 120) {
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


static void CosmicCrosshair() {
    float time = static_cast<float>(ImGui::GetTime());
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    ImVec2 size = ImGui::GetIO().DisplaySize;
    ImVec2 center = ImVec2(size.x / 2.0f, size.y / 2.0f);

    // === Cosmic Portal/Galaxy Crosshair ===
    // We'll use the portal code but position it at screen center
    float portal_phase = 1.0f; // Fully active (no fade in/out)
    float radius = cfg::nightFxSize;//20.0f;
    float rotation = time * 1.2f; // Rotation speed

    // Draw portal rings
    for (int ring = 0; ring < 5; ring++) {
        float ring_radius = radius * (0.4f + ring * 0.15f);
        float ring_alpha = 0.8f - ring * 0.15f; // Make rings more visible for crosshair purpose

        // Pulse effect for rings
        float ring_pulse = 0.7f + 0.3f * sinf(time * 3.0f + ring);

        // Blend between green and purple for cosmic effect
        float hue = 0.28f + 0.3f * (sinf(time + ring * 0.5f) * 0.5f + 0.5f);
        float r, g, b;
        ImGui::ColorConvertHSVtoRGB(hue, 0.9f, 1.0f, r, g, b);

        // Number of segments in each ring for a more complex portal
        const int segments = 36;
        for (int i = 0; i < segments; i++) {
            float angle1 = rotation + 2.0f * IM_PI * i / segments;
            float angle2 = rotation + 2.0f * IM_PI * (i + 1) / segments;

            // Wave effect in rings
            float wave = 1.0f + 0.15f * sinf(angle1 * 6.0f + time * 4.0f);

            ImVec2 p1(
                center.x + cosf(angle1) * ring_radius * wave,
                center.y + sinf(angle1) * ring_radius * wave
            );
            ImVec2 p2(
                center.x + cosf(angle2) * ring_radius * wave,
                center.y + sinf(angle2) * ring_radius * wave
            );

            // Draw segment
            ImU32 segment_color = IM_COL32(
                (int)(r * 255),
                (int)(g * 255),
                (int)(b * 255),
                (int)(180 * ring_alpha * ring_pulse)
            );
            drawList->AddLine(p1, p2, segment_color, 1.0f);
        }
    }

    // Draw energy rays from portal for crosshair effect
    const int rays = 8;
    for (int i = 0; i < rays; i++) {
        float ray_angle = rotation * 0.5f + 2.0f * IM_PI * i / rays;
        float ray_length = radius * 1.5f * (0.5f + 0.5f * sinf(time * 2.0f + i));

        ImVec2 ray_end(
            center.x + cosf(ray_angle) * ray_length,
            center.y + sinf(ray_angle) * ray_length
        );

        // Ray color with pulsating effect
        float ray_hue = 0.3f + 0.1f * sinf(time * 2.0f + i);
        float r, g, b;
        ImGui::ColorConvertHSVtoRGB(ray_hue, 0.9f, 1.0f, r, g, b);

        float ray_alpha = 0.7f + 0.3f * sinf(time * 5.0f + i);
        ImU32 ray_color = IM_COL32(
            (int)(r * 255),
            (int)(g * 255),
            (int)(b * 255),
            (int)(180 * ray_alpha)
        );

        drawList->AddLine(center, ray_end, ray_color, 1.0f);
    }

    // Portal center glow - this is the center of the crosshair
    //drawList->AddCircleFilled(center, radius * 0.3f,
    //    IM_COL32(120, 255, 180, 120));
	//drawList->AddCircleFilled(center, radius * 0.15f,
    //    IM_COL32(220, 255, 220, 180));

    // Optional: Add cardinal crosshair lines for better targeting
    // These lines help with precise aiming while keeping the cosmic effect
    if (cfg::nightFxCrosshairLines) {
        const float crossLength = radius * 0.7f;
        ImU32 crossColor = IM_COL32(255, 255, 255, 150);

        // Horizontal line (pulsing)
        float horizontalPulse = 0.8f + 0.2f * sinf(time * 4.0f);
        drawList->AddLine(
            ImVec2(center.x - crossLength * horizontalPulse, center.y),
            ImVec2(center.x + crossLength * horizontalPulse, center.y),
            crossColor, 1.0f
        );

        // Vertical line (pulsing slightly out of phase)
        float verticalPulse = 0.8f + 0.2f * sinf(time * 4.0f + 0.5f);
        drawList->AddLine(
            ImVec2(center.x, center.y - crossLength * verticalPulse),
            ImVec2(center.x, center.y + crossLength * verticalPulse),
            crossColor, 1.0f
        );
    }
}