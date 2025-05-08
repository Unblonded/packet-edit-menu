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
#include "cfg.h"
using json = nlohmann::json;
namespace fs = std::filesystem;
#pragma comment(lib, "ws2_32.lib")


bool endloop = false;
bool shouldUnload = false;


void RenderMain()
{
    if (!cfg::showMenu) cfg::showMenuInitialized = false;
    if (!cfg::showAll) return;

    static float pulse_speed = 3.5f;
    float pulse = 0.5f + 0.5f * sinf(ImGui::GetTime() * pulse_speed);
    float pulse_alt = 0.5f + 0.5f * cosf(ImGui::GetTime() * pulse_speed * 0.8f);
    ImVec4 neon_pink = ImVec4(1.0f, 0.1f, 0.6f, 1.0f);
    ImVec4 neon_blue = ImVec4(0.1f, 0.9f, 1.0f, 1.0f);
    ImVec4 neon_purple = ImVec4(0.7f, 0.3f, 1.0f, 1.0f);

    ImVec2 windowSize = ImGui::GetIO().DisplaySize;
    ImGui::GetIO().FontGlobalScale = (windowSize.x / 1920.0f) * 0.85f;

    if (cfg::showMenu) {
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(
            neon_purple.x * pulse,
            neon_purple.y * pulse,
            neon_purple.z * pulse,
            0.8f
        ));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f + 1.5f * pulse);

        ImGui::Begin("Modules - Unblonded");

        // Animated checkboxes
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.08f, 0.05f, 0.12f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(
            neon_blue.x * 0.3f,
            neon_blue.y * 0.3f,
            neon_blue.z * 0.3f,
            0.6f
        ));
        ImGui::PushStyleColor(ImGuiCol_CheckMark, neon_pink);

        ImGui::Checkbox("Show Player List", &cfg::displayPlayers);
        ImGui::Checkbox("Advanced ESP", &cfg::advEsp);
        ImGui::Checkbox("Player Dig Safety", &cfg::checkPlayerAirSafety);
        ImGui::Checkbox("Straight Tunnel", &cfg::forwardTunnel);
        ImGui::Checkbox("Auto Crystal", &cfg::autoCrystal);
        ImGui::Checkbox("Interaction Canceler", &cfg::cancelInteraction);
        ImGui::Checkbox("Auto Anchor", &cfg::autoAnchor);

        // Special animation for Seed-Ray checkbox
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(
            neon_purple.x * pulse,
            neon_purple.y * pulse,
            neon_purple.z * pulse,
            0.8f
        ));
        ImGui::Checkbox("Seed-Ray", &cfg::oreSim);
        ImGui::PopStyleColor(4); // Pop all checkbox styles

        ImGui::End();
        ImGui::PopStyleVar(); // WindowBorderSize
        ImGui::PopStyleColor();

        if (cfg::oreSim) {
            // Seed-Ray window with scanline effect
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.03f, 0.08f, 0.95f));
            ImGui::Begin("Seed-Ray Config");

            // Add scanline overlay
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 p_min = ImGui::GetWindowPos();
            ImVec2 p_max = ImVec2(p_min.x + ImGui::GetWindowWidth(), p_min.y + ImGui::GetWindowHeight());
            for (float y = p_min.y; y < p_max.y; y += 3.0f) {
                draw_list->AddLine(
                    ImVec2(p_min.x, y),
                    ImVec2(p_max.x, y),
                    IM_COL32(0, 255, 255, 10 + (int)(5 * pulse))
                );
            }

            ImGui::TextColored(neon_blue, "Seed-Ray is enabled");
            if (cfg::oreSimDistance > 8)
                ImGui::TextColored(neon_pink, "Warning: High render distance may use lots of CPU");

            // Animated slider
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.12f, 0.08f, 0.18f, 0.8f));
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.18f, 0.12f, 0.24f, 0.8f));
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, neon_purple);
            ImGui::SliderInt("Render Distance", &cfg::oreSimDistance, 0, 32);
            ImGui::PopStyleColor(3);

            ImGui::InputScalar("Seed", ImGuiDataType_S64, &cfg::oreSimSeed);
            if (ImGui::Button("Donut Nether")) {
                cfg::oreSimSeed = (long long)(6608149111735331168);
            }

            ImGui::ColorEdit4("ESP Color", (float*)&cfg::oreSimColor);
            ImGui::End();
            ImGui::PopStyleColor();
        }

        if (cfg::autoCrystal) {
			ImGui::Begin("Auto Crystal");
			ImGui::Text("Auto Crystal is enabled.");
            ImGui::SliderInt("Attack Time (ms)", &cfg::crystalAttackTime, 5, 500);
			ImGui::SliderInt("Place Time (ms)", &cfg::crystalPlaceTime, 5, 500);
			ImGui::End();
        }

        if (cfg::advEsp) {
            ImGui::Begin("Advanced ESP");

            ImGui::Text("ESP Settings:");
            ImGui::SliderInt("Esp Radius", &cfg::espRadius, 16, 128);
            ImGui::SliderInt("Batch Size x1K", &cfg::espBatchSize, 50, 200);
            ImGui::SliderInt("Search Time (sec)", &cfg::espSearchTime, 0, 20);
            ImGui::Checkbox("Draw Blocks", &cfg::drawBlocks);
            if (cfg::drawBlocks) ImGui::Checkbox("Draw Tracers", &cfg::drawBlockTracer);

            // Add new block input
            ImGui::InputText("Block Name", cfg::blockName, sizeof(cfg::blockName));
            ImGui::ColorEdit4("Block Color", (float*)&cfg::blockColor); // Color picker for the block

            if (ImGui::Button("Add Block to ESP")) {
                // Check if the block already exists in the list
                bool exists = false;
                for (const auto& blockColorItem : cfg::espBlockList) {
                    if (blockColorItem.name == cfg::blockName) {
                        exists = true;
                        break;
                    }
                }

                // If the block does not exist, add it to the list
                if (!exists) {
                    cfg::espBlockList.push_back(EspBlock(cfg::blockName, cfg::blockColor)); // Add the new block with color
                }
            }


            ImGui::Separator();
            ImGui::Text("Current ESP Blocks:");
            ImGui::BeginChild("BlockListChild", ImVec2(0, 150), true);

            for (int i = 0; i < cfg::espBlockList.size(); ++i) {
                ImGui::PushID(i);
                ImGui::ColorEdit4("##color", (float*)&cfg::espBlockList[i].color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                ImGui::SameLine();
                ImGui::TextUnformatted(cfg::espBlockList[i].name.c_str());
                ImGui::SameLine(ImGui::GetWindowWidth() - 110);
                if (ImGui::SmallButton("Remove")) {
                    cfg::espBlockList.erase(cfg::espBlockList.begin() + i);
                    --i;
                }
                ImGui::PopID();
            }

            ImGui::EndChild();
            ImGui::End();
        }

    }

    if (cfg::displayPlayers) {
        ImGui::Begin("Nearby Players");
        for (const auto& name : cfg::nearbyPlayers) {
            ImGui::TextUnformatted(name.c_str());
        }
        ImGui::End();
    }

    if (cfg::checkPlayerAirSafety) {
		ImGui::Begin("Dig Safety");
        ImGui::Text(cfg::isPlayerAirSafe.c_str());
		ImGui::End();
    }

    if (cfg::forwardTunnel) {
        ImGui::Begin("Tunneling Status");
        ImGui::Text(cfg::tunnelBlockStatus.c_str());
        ImGui::End();
    }

    if (cfg::showMenu && !cfg::showMenuInitialized) {
        ImGui::SetWindowFocus(nullptr); // Remove focus from any window
        cfg::showMenuInitialized = true;
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
                config["displayPlayers"] = cfg::displayPlayers;
				config["drawBlocks"] = cfg::drawBlocks;
				config["advEsp"] = cfg::advEsp;
				config["espRadius"] = cfg::espRadius;
				config["espBatchSize"] = cfg::espBatchSize;
				config["espSearchTime"] = cfg::espSearchTime;
                config["checkPlayerAirSafety"] = cfg::checkPlayerAirSafety;
                config["drawBlockTracer"] = cfg::drawBlockTracer;
                config["forwardTunnel"] = cfg::forwardTunnel;
				config["autoCrystal"] = cfg::autoCrystal;
				config["crystalAttackTime"] = cfg::crystalAttackTime;
				config["crystalPlaceTime"] = cfg::crystalPlaceTime;
				config["cancelInteraction"] = cfg::cancelInteraction;
				config["autoAnchor"] = cfg::autoAnchor;
				config["oreSim"] = cfg::oreSim;
				config["oreSimSeed"] = cfg::oreSimSeed;
				config["oreSimDistance"] = cfg::oreSimDistance;
				config["oreSimColor"] = { cfg::oreSimColor.x, cfg::oreSimColor.y, cfg::oreSimColor.z, cfg::oreSimColor.w };

                json espBlocksJson = json::array();
                for (const auto& block : cfg::espBlockList) {
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
                        cfg::showMenu = renderJson.value("shouldRender", false);
                        cfg::showAll = renderJson.value("worldReady", false);
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
                            cfg::nearbyPlayers.clear();
                            for (const auto& name : playerJson["PLAYERS"]) {
                                if (name.is_string())
                                    cfg::nearbyPlayers.push_back(name.get<std::string>());
                            }

                            for (const auto& name : cfg::nearbyPlayers) {
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
                        cfg::isPlayerAirSafe = result.value("playerAirSafety", "");
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
                        cfg::tunnelBlockStatus = result.value("tunnelBlockStatus", "");
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
    config["displayPlayers"] = cfg::displayPlayers;
    config["drawBlocks"] = cfg::drawBlocks;
    config["advEsp"] = cfg::advEsp;
    config["espRadius"] = cfg::espRadius;
    config["espBatchSize"] = cfg::espBatchSize;
    config["espSearchTime"] = cfg::espSearchTime;
    config["checkPlayerAirSafety"] = cfg::checkPlayerAirSafety;
    config["drawBlockTracer"] = cfg::drawBlockTracer;
    config["crystalAttackTime"] = cfg::crystalAttackTime;
	config["crystalPlaceTime"] = cfg::crystalPlaceTime;
	config["cancelInteraction"] = cfg::cancelInteraction;
	config["autoAnchor"] = cfg::autoAnchor;
	config["oreSim"] = cfg::oreSim;
	config["oreSimSeed"] = cfg::oreSimSeed;
	config["oreSimDistance"] = cfg::oreSimDistance;
	config["oreSimColor"] = { cfg::oreSimColor.x, cfg::oreSimColor.y, cfg::oreSimColor.z, cfg::oreSimColor.w };

    json blocksJson = json::array();
    for (const auto& block : cfg::espBlockList) {
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

        if (config.contains("displayPlayers")) cfg::displayPlayers = config["displayPlayers"].get<bool>();
        if (config.contains("drawBlocks")) cfg::drawBlocks = config["drawBlocks"].get<bool>();
        if (config.contains("advEsp")) cfg::advEsp = config["advEsp"].get<bool>();
        if (config.contains("espRadius")) cfg::espRadius = config["espRadius"].get<int>();
        if (config.contains("espBatchSize")) cfg::espBatchSize = config["espBatchSize"].get<int>();
        if (config.contains("espSearchTime")) cfg::espSearchTime = config["espSearchTime"].get<int>();
        if (config.contains("checkPlayerAirSafety")) cfg::checkPlayerAirSafety = config["checkPlayerAirSafety"].get<bool>();
        if (config.contains("drawBlockTracer")) cfg::drawBlockTracer = config["drawBlockTracer"].get<bool>();
		if (config.contains("crystalAttackTime")) cfg::crystalAttackTime = config["crystalAttackTime"].get<int>();
		if (config.contains("cancelInteraction")) cfg::cancelInteraction = config["cancelInteraction"].get<bool>();
		if (config.contains("crystalPlaceTime")) cfg::crystalPlaceTime = config["crystalPlaceTime"].get<int>();
		if (config.contains("autoAnchor")) cfg::autoAnchor = config["autoAnchor"].get<bool>();
		if (config.contains("oreSim")) cfg::oreSim = config["oreSim"].get<bool>();
		if (config.contains("oreSimSeed")) cfg::oreSimSeed = config["oreSimSeed"].get<long long>();
		if (config.contains("oreSimDistance")) cfg::oreSimDistance = config["oreSimDistance"].get<int>();
		if (config.contains("oreSimColor") && config["oreSimColor"].is_array() && config["oreSimColor"].size() == 4)
            cfg::oreSimColor = ImVec4(config["oreSimColor"][0], config["oreSimColor"][1], config["oreSimColor"][2], config["oreSimColor"][3]);

        // Load ESP blocks
        cfg::espBlockList.clear();
        if (config.contains("espBlockList") && config["espBlockList"].is_array()) {
            for (const auto& item : config["espBlockList"]) {
                if (item.contains("name") && item.contains("color") && item["color"].size() == 4) {
                    cfg::espBlockList.push_back({
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