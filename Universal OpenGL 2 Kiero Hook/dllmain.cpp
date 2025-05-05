#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <nlohmann/json.hpp>
#include "include/imgui_hook.h"
#include "include/imgui/imgui.h"
#include <iostream>
#include <fstream>
#include <string>
#include <Shlobj.h>
#include <filesystem>
using json = nlohmann::json;
namespace fs = std::filesystem;
#pragma comment(lib, "ws2_32.lib")

struct EspBlock {
    std::string name;
    ImVec4 color;

    // Constructor to initialize a block with a name and a color
    EspBlock(const std::string& blockName, const ImVec4& blockColor)
        : name(blockName), color(blockColor) {
    }
};


ImVec4 blockColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

bool endloop = false;
bool shouldUnload = false;
bool showMenu = false;
bool showAll = false;

bool displayPlayers = false;
bool advEsp = false;
bool drawBlocks = false;
bool drawBlockTracer = false;
char blockName[256] = "diamond_ore";
std::vector<EspBlock> espBlockList;
int espRadius = 128;
int espBatchSize = 200;
int espSearchTime = 10;
bool checkPlayerAirSafety = false;
std::string isPlayerAirSafe = "Unknown";
bool forwardTunnel = false;

std::vector<std::string> nearbyPlayers;

std::string tunnelBlockStatus = "Unknown";

bool autoCrystal = false;
int crystalAttackTime = 0;
int crystalPlaceTime = 0;
bool cancelInteraction = false;
bool showMenuInitialized = false;



void RenderMain()
{
    if (!showMenu) showMenuInitialized = false;
    if (!showAll) return;

    ImVec2 windowSize = ImGui::GetIO().DisplaySize;
    ImGui::GetIO().FontGlobalScale = (windowSize.x / 1920.0f) * 0.85f;

    if (showMenu) {
        ImGui::Begin("Packet Edit - Unblonded");
        ImGui::Checkbox("Show Player List", &displayPlayers);
		ImGui::Checkbox("Advanced ESP", &advEsp);
		ImGui::Checkbox("Player Dig Safety", &checkPlayerAirSafety);
        ImGui::Checkbox("Straight Tunnel", &forwardTunnel);
		ImGui::Checkbox("Auto Crystal", &autoCrystal);
        ImGui::Checkbox("Interaction Canceler", &cancelInteraction);
        ImGui::End();

        if (autoCrystal) {
			ImGui::Begin("Auto Crystal");
			ImGui::Text("Auto Crystal is enabled.");
            ImGui::SliderInt("Attack Time (ms)", &crystalAttackTime, 5, 500);
			ImGui::SliderInt("Place Time (ms)", &crystalPlaceTime, 5, 500);
			ImGui::End();
        }

        if (advEsp) {
            ImGui::Begin("Advanced ESP");

            ImGui::Text("ESP Settings:");
            ImGui::SliderInt("Esp Radius", &espRadius, 16, 128);
            ImGui::SliderInt("Batch Size x1K", &espBatchSize, 50, 200);
            ImGui::SliderInt("Search Time (sec)", &espSearchTime, 0, 20);
            ImGui::Checkbox("Draw Blocks", &drawBlocks);
            if (drawBlocks) ImGui::Checkbox("Draw Tracers", &drawBlockTracer);

            // Add new block input
            ImGui::InputText("Block Name", blockName, sizeof(blockName));
            ImGui::ColorEdit4("Block Color", (float*)&blockColor); // Color picker for the block

            if (ImGui::Button("Add Block to ESP")) {
                // Check if the block already exists in the list
                bool exists = false;
                for (const auto& blockColorItem : espBlockList) {
                    if (blockColorItem.name == blockName) {
                        exists = true;
                        break;
                    }
                }

                // If the block does not exist, add it to the list
                if (!exists) {
                    espBlockList.push_back(EspBlock(blockName, blockColor)); // Add the new block with color
                }
            }


            ImGui::Separator();
            ImGui::Text("Current ESP Blocks:");
            ImGui::BeginChild("BlockListChild", ImVec2(0, 150), true);

            for (int i = 0; i < espBlockList.size(); ++i) {
                ImGui::PushID(i);
                ImGui::ColorEdit4("##color", (float*)&espBlockList[i].color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                ImGui::SameLine();
                ImGui::TextUnformatted(espBlockList[i].name.c_str());
                ImGui::SameLine(ImGui::GetWindowWidth() - 100);
                if (ImGui::SmallButton("Remove")) {
                    espBlockList.erase(espBlockList.begin() + i);
                    --i;
                }
                ImGui::PopID();
            }

            ImGui::EndChild();
            ImGui::End();
        }

    }

    if (displayPlayers) {
        ImGui::Begin("Nearby Players");
        for (const auto& name : nearbyPlayers) {
            ImGui::TextUnformatted(name.c_str());
        }
        ImGui::End();
    }

    if (checkPlayerAirSafety) {
		ImGui::Begin("Dig Safety");
        ImGui::Text(isPlayerAirSafe.c_str());
		ImGui::End();
    }

    if (forwardTunnel) {
        ImGui::Begin("Tunneling Status");
        ImGui::Text(tunnelBlockStatus.c_str());
        ImGui::End();
    }

    if (showMenu && !showMenuInitialized) {
        ImGui::SetWindowFocus(nullptr); // Remove focus from any window
        showMenuInitialized = true;
    }
}


DWORD WINAPI TCPThread(LPVOID lpParam) {
    WSADATA wsaData;
    SOCKET listenSock = INVALID_SOCKET;
    SOCKET clientSock = INVALID_SOCKET;
    sockaddr_in serverAddr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[TCP] WSAStartup failed.\n";
        return 1;
    }

    listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock == INVALID_SOCKET) {
        std::cerr << "[TCP] Failed to create socket.\n";
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1337);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[TCP] Bind failed.\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    if (listen(listenSock, 1) == SOCKET_ERROR) {
        std::cerr << "[TCP] Listen failed.\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    std::cout << "[TCP] Waiting for Java client connection...\n";

    clientSock = accept(listenSock, NULL, NULL);
    if (clientSock == INVALID_SOCKET) {
        std::cerr << "[TCP] Accept failed.\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    std::cout << "[TCP] Java client connected.\n";

    char buffer[4096];
    std::string leftover;

    while (!endloop) {
        int bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            if (bytesReceived == 0)
                std::cout << "[TCP] Client disconnected.\n";
            else
                std::cerr << "[TCP] Recv error: " << WSAGetLastError() << "\n";
            break;
        }

        buffer[bytesReceived] = '\0';
        leftover += buffer;

        size_t newlinePos;
        while ((newlinePos = leftover.find('\n')) != std::string::npos) {
            std::string message = leftover.substr(0, newlinePos);
            leftover.erase(0, newlinePos + 1);

            // Now handle each line separately:
            if (message.find("GET_CONFIG") != std::string::npos) {
                json config;
                config["displayPlayers"] = displayPlayers;
				config["drawBlocks"] = drawBlocks;
				config["advEsp"] = advEsp;
				config["espRadius"] = espRadius;
				config["espBatchSize"] = espBatchSize;
				config["espSearchTime"] = espSearchTime;
                config["checkPlayerAirSafety"] = checkPlayerAirSafety;
                config["drawBlockTracer"] = drawBlockTracer;
                config["forwardTunnel"] = forwardTunnel;
				config["autoCrystal"] = autoCrystal;
				config["crystalAttackTime"] = crystalAttackTime;
				config["crystalPlaceTime"] = crystalPlaceTime;
				config["cancelInteraction"] = cancelInteraction;

                json espBlocksJson = json::array();
                for (const auto& block : espBlockList) {
                    espBlocksJson.push_back({
                        {"name", block.name},
                        {"color", {block.color.x, block.color.y, block.color.z, block.color.w}}
                        });
                }
                config["espBlockList"] = espBlocksJson;

                std::string response = config.dump() + "\n";
                int remaining = response.length();
                const char* ptr = response.c_str();
                while (remaining > 0) {
                    int sent = send(clientSock, ptr, remaining, 0);
                    if (sent == SOCKET_ERROR) {
                        std::cerr << "[TCP] Send failed: " << WSAGetLastError() << std::endl;
                        break;
                    }
                    remaining -= sent;
                    ptr += sent;
                }
            }
            else if (message.find("RENDER_FLAG") != std::string::npos) {
                try {
                    size_t jsonStart = message.find_first_of('{');
                    if (jsonStart != std::string::npos) {
                        std::string jsonStr = message.substr(jsonStart);
                        json renderJson = json::parse(jsonStr);
                        showMenu = renderJson.value("shouldRender", false);
                        showAll = renderJson.value("worldReady", false);
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "[TCP] Failed to parse render flag: " << e.what() << "\n";
                }
            }
            else if (message.find("PLAYERS") != std::string::npos) {
                try {
                    size_t jsonStart = message.find_first_of('{');
                    if (jsonStart != std::string::npos) {
                        std::string jsonStr = message.substr(jsonStart);
                        json playerJson = json::parse(jsonStr);

                        if (playerJson.contains("PLAYERS") && playerJson["PLAYERS"].is_array()) {
                            nearbyPlayers.clear();
                            for (const auto& name : playerJson["PLAYERS"]) {
                                if (name.is_string())
                                    nearbyPlayers.push_back(name.get<std::string>());
                            }

                            for (const auto& name : nearbyPlayers) {
                                std::cout << " - " << name << '\n';
                            }
                        }
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "[TCP] Failed to parse player list: " << e.what() << "\n";
                }
            }
            else if (message.find("playerAirSafety") != std::string::npos) {
                try {
                    size_t jsonStart = message.find_first_of('{');
                    if (jsonStart != std::string::npos) {
                        std::string jsonStr = message.substr(jsonStart);
                        json result = json::parse(jsonStr);
                        isPlayerAirSafe = result.value("playerAirSafety", "");
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "[TCP] Failed to parse render flag: " << e.what() << "\n";
                }
            }
            else if (message.find("tunnelBlockStatus") != std::string::npos) {
                try {
                    size_t jsonStart = message.find_first_of('{');
                    if (jsonStart != std::string::npos) {
                        std::string jsonStr = message.substr(jsonStart);
                        json result = json::parse(jsonStr);
                        tunnelBlockStatus = result.value("tunnelBlockStatus", "");
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "[TCP] Failed to parse render flag: " << e.what() << "\n";
                }
            }
        }

        Sleep(1);
    }

    closesocket(clientSock);
    closesocket(listenSock);
    WSACleanup();
    FreeLibraryAndExitThread((HMODULE)lpParam, 0);
    return 0;
}

std::string getMinecraftFolder() {
    char appdata[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appdata);
    std::filesystem::path minecraftPath = std::filesystem::path(appdata) / ".minecraft";
    return minecraftPath.string();
}

void saveSettings() {
    json config;
    config["displayPlayers"] = displayPlayers;
    config["drawBlocks"] = drawBlocks;
    config["advEsp"] = advEsp;
    config["espRadius"] = espRadius;
    config["espBatchSize"] = espBatchSize;
    config["espSearchTime"] = espSearchTime;
    config["checkPlayerAirSafety"] = checkPlayerAirSafety;
    config["drawBlockTracer"] = drawBlockTracer;
    config["crystalAttackTime"] = crystalAttackTime;
	config["crystalPlaceTime"] = crystalPlaceTime;
	config["cancelInteraction"] = cancelInteraction;

    json blocksJson = json::array();
    for (const auto& block : espBlockList) {
        blocksJson.push_back({
            {"name", block.name},
            {"color", {block.color.x, block.color.y, block.color.z, block.color.w}}
            });
    }
    config["espBlockList"] = blocksJson;

    std::string path = getMinecraftFolder() + "\\packet-edit.json";
    std::ofstream file(path);
    if (file.is_open()) {
        file << config.dump(4); // Pretty print with indent = 4
        file.close();
    }
}

void loadSettings() {
    std::string path = getMinecraftFolder() + "\\packet-edit.json";
    std::ifstream file(path);
    if (!file.is_open()) return;

    try {
        json config;
        file >> config;

        if (config.contains("displayPlayers")) displayPlayers = config["displayPlayers"].get<bool>();
        if (config.contains("drawBlocks")) drawBlocks = config["drawBlocks"].get<bool>();
        if (config.contains("advEsp")) advEsp = config["advEsp"].get<bool>();
        if (config.contains("espRadius")) espRadius = config["espRadius"].get<int>();
        if (config.contains("espBatchSize")) espBatchSize = config["espBatchSize"].get<int>();
        if (config.contains("espSearchTime")) espSearchTime = config["espSearchTime"].get<int>();
        if (config.contains("checkPlayerAirSafety")) checkPlayerAirSafety = config["checkPlayerAirSafety"].get<bool>();
        if (config.contains("drawBlockTracer")) drawBlockTracer = config["drawBlockTracer"].get<bool>();
		if (config.contains("crystalAttackTime")) crystalAttackTime = config["crystalAttackTime"].get<int>();
		if (config.contains("cancelInteraction")) cancelInteraction = config["cancelInteraction"].get<bool>();
		if (config.contains("crystalPlaceTime")) crystalPlaceTime = config["crystalPlaceTime"].get<int>();

        // Load ESP blocks
        espBlockList.clear();
        if (config.contains("espBlockList") && config["espBlockList"].is_array()) {
            for (const auto& item : config["espBlockList"]) {
                if (item.contains("name") && item.contains("color") && item["color"].size() == 4) {
                    espBlockList.push_back({
                        item["name"],
                        ImVec4(item["color"][0], item["color"][1], item["color"][2], item["color"][3])
                        });
                }
            }
        }

    }
    catch (...) { }
}


DWORD WINAPI InfoThread(LPVOID lpParam)
{
    loadSettings();
    while (!endloop) {
        saveSettings();
        Sleep(1000);
    }
    return 0;
}


BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, TCPThread, hMod, 0, nullptr);
        CreateThread(nullptr, 0, InfoThread, hMod, 0, nullptr);
		ImGuiHook::Load(RenderMain);
		break;
	case DLL_PROCESS_DETACH:
		endloop = true;
		ImGuiHook::Unload();
		break;
	}
	return TRUE;
}