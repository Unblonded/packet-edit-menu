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
#include "IconsFontAwesome5.h"

using json = nlohmann::json;
namespace fs = std::filesystem;
#pragma comment(lib, "ws2_32.lib")


bool endloop = false;
bool shouldUnload = false;
bool singleInit = false;


void RenderMain()
{
    if (!cfg::showAll) return;

    static float pulse_speed = 3.5f;
    float pulse = 0.5f + 0.5f * sinf(static_cast<float>(ImGui::GetTime()) * pulse_speed);
    float pulse_alt = 0.5f + 0.5f * cosf(static_cast<float>(ImGui::GetTime()) * pulse_speed * 0.8f);
    ImVec4 neon_pink = ImVec4(1.0f, 0.1f, 0.6f, 1.0f);
    ImVec4 neon_blue = ImVec4(0.1f, 0.9f, 1.0f, 1.0f);
    ImVec4 neon_purple = ImVec4(0.7f, 0.3f, 1.0f, 1.0f);

    if (!cfg::fontSizeOverride) {
        ImVec2 windowSize = ImGui::GetIO().DisplaySize;
        ImGui::GetIO().FontGlobalScale = max(1.f, ((windowSize.x / 1920.0f) * 0.85f));
	}
	else ImGui::GetIO().FontGlobalScale = cfg::fontSize;

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

        if (ImGui::BeginTabBar("MainTabBar", ImGuiTabBarFlags_None))
        {
            // Combat & PvP Tab
            if (ImGui::BeginTabItem(ICON_FA_CROSSHAIRS " Combat"))
            {
                ImGui::Checkbox(ICON_FA_GEM " Auto Crystal", &cfg::autoCrystal);
                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_COGS "##crystal")) cfg::autoCrystalcfg = !cfg::autoCrystalcfg;

                ImGui::Checkbox(ICON_FA_SHIELD_ALT " Auto Totem", &cfg::autoTotem);
                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_COGS "##totem")) cfg::autoTotemcfg = !cfg::autoTotemcfg;

                ImGui::Checkbox(ICON_FA_BOMB " Auto Anchor", &cfg::autoAnchor);

                ImGui::Checkbox(ICON_FA_CROSSHAIRS " Aim Assist", &cfg::aimAssistToggle);
                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_COGS "##aimAssist")) cfg::aimAssistcfg = !cfg::aimAssistcfg;

                ImGui::Checkbox(ICON_FA_BOMB " Crystal Spam", &cfg::crystalSpam);
                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_COGS "##crystalspam")) cfg::crystalSpamcfg = !cfg::crystalSpamcfg;

                ImGui::EndTabItem();
            }

            // ESP & Visual Tab
            if (ImGui::BeginTabItem(ICON_FA_EYE " Visuals"))
            {
                ImGui::Checkbox(ICON_FA_TEXT_HEIGHT " Font Size Override", &cfg::fontSizeOverride);
                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_COGS "##fontsize")) cfg::fontSizecfg = !cfg::fontSizecfg;

                ImGui::Checkbox(ICON_FA_USERS " Show Player List", &cfg::displayPlayers);

                ImGui::Checkbox(ICON_FA_EYE " Advanced ESP", &cfg::advEsp);
                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_COGS "##advesp")) cfg::advEspcfg = !cfg::advEspcfg;

                ImGui::Checkbox(ICON_FA_IMAGE " Show Background Effects", &cfg::backgroundFx);

				ImGui::Checkbox(ICON_FA_STAR " Show Cosmic Crosshair", &cfg::nightFx);
                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_COGS "##crosshair")) cfg::nightFxcfg = !cfg::nightFxcfg;

                ImGui::EndTabItem();
            }

            // Utility Tab
            if (ImGui::BeginTabItem(ICON_FA_TOOLBOX " Utility"))
            {
            	ImGui::Checkbox(ICON_FA_HAND_PAPER " Interaction Canceler", &cfg::cancelInteraction);

                ImGui::Checkbox(ICON_FA_PLUG " Auto Disconnect", &cfg::autoDc);
                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_COGS "##autodc")) cfg::autoDccfg = !cfg::autoDccfg;

                ImGui::Checkbox(ICON_FA_DOLLAR_SIGN " Auto Sell", &cfg::autoSell);
                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_COGS "##autosell")) cfg::autoSellcfg = !cfg::autoSellcfg;

                ImGui::Checkbox(ICON_FA_COMMENT_SLASH " Chat Filter", &cfg::chatFilter);
                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_COGS "##chatfilter")) cfg::chatFiltercfg = !cfg::chatFiltercfg;

                ImGui::Checkbox(ICON_FA_BOX_OPEN " Storage Scan", &cfg::storageScan);
                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_COGS "##storagescan")) cfg::storageScancfg = !cfg::storageScancfg;

                ImGui::EndTabItem();
            }

            // Mining & Economy Tab
            if (ImGui::BeginTabItem(ICON_FA_GEM " Mining"))
            {
                ImGui::Checkbox(ICON_FA_HARD_HAT " Player Dig Safety", &cfg::checkPlayerAirSafety);
                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_COGS "##digsafety")) cfg::checkPlayerAirSafetycfg = !cfg::checkPlayerAirSafetycfg;

            	ImGui::Checkbox(ICON_FA_ROUTE " Straight Tunnel", &cfg::forwardTunnel);

                ImGui::Checkbox(ICON_FA_SEEDLING " Seed-Ray", &cfg::oreSim);
                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_COGS "##oresim")) cfg::oreSimcfg = !cfg::oreSimcfg;

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }


		ImGui::PopStyleColor(3);

        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        if (cfg::nightFxcfg) {
			ImGui::Begin("Cosmic Crosshair", &cfg::nightFxcfg);
			ImGui::Text("Cosmic Crosshair is %s", cfg::nightFx ? "enabled" : "disabled");
			ImGui::SliderFloat("Size", &cfg::nightFxSize, 1.0f, 100.0f);
			ImGui::Checkbox("Show Crosshair Lines", &cfg::nightFxCrosshairLines);
			ImGui::End();
        }

        if (cfg::crystalSpamcfg) {
			ImGui::Begin("Crystal Spam", &cfg::crystalSpamcfg);
			ImGui::Text("Crystal Spam is %s", cfg::crystalSpam ? "enabled" : "disabled");
			ImGui::SliderInt("Search Radius", &cfg::crystalSpamSearchRadius, 1, 6);
			ImGui::SliderInt("Break Delay (ms)", &cfg::crystalSpamBreakDelay, 1, 1000);
			ImGui::End();
        }

        if (cfg::aimAssistcfg) {
            ImGui::Begin("Aim Assist", &cfg::aimAssistcfg);
            ImGui::Text("Aim Assist is %s", cfg::aimAssistToggle ? "enabled" : "disabled");

            ImGui::SliderFloat("Range", &cfg::aimAssistrange, 1.0f, 20.0f, "%.1f");
            ImGui::SliderFloat("Field of View", &cfg::aimAssistfov, 1.0f, 180.0f, "%.1f");
            ImGui::SliderFloat("Smoothness", &cfg::aimAssistsmoothness, 0.1f, 10.0f, "%.1f");
            ImGui::SliderFloat("Min Speed", &cfg::aimAssistminSpeed, 1.0f, 360.0f, "%.1f/s");
            ImGui::SliderFloat("Max Speed", &cfg::aimAssistmaxSpeed, 1.0f, 360.0f, "%.1f/s");
            ImGui::Checkbox("Visibility Check", &cfg::aimAssistvisibilityCheck);
            ImGui::SliderInt("Update Rate (ms)", &cfg::aimAssistupdateRate, 1, 1000);

            ImGui::End();
        }

        if (cfg::fontSizecfg) {
			ImGui::Begin("Font Size Override", &cfg::fontSizecfg);
			ImGui::Text("Use a custom font size.");
			ImGui::SliderFloat("Font Size", &cfg::fontSize, 0.25f, 4.0f);
			ImGui::End();
        }

        if (cfg::chatFiltercfg) {
			ImGui::Begin("Chat Filter", &cfg::chatFiltercfg);
			if (cfg::filterMode != 0) ImGui::Text("Block Chat If: ");
            if (ImGui::BeginCombo("Filter Mode", cfg::chatFilterItems[cfg::filterMode])) {
                for (int n = 0; n < IM_ARRAYSIZE(cfg::chatFilterItems); n++) {
                    bool isSelected = (cfg::filterMode == n);
                    if (ImGui::Selectable(cfg::chatFilterItems[n], isSelected)) cfg::filterMode = n;
                    if (isSelected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

			if (cfg::filterMode != 0)
				ImGui::InputText("Message", cfg::blockMsg, IM_ARRAYSIZE(cfg::blockMsg));
            ImGui::End();
        }

        if (cfg::autoDccfg) {
            ImGui::Begin("Auto Disconnect", &cfg::autoDccfg);
			ImGui::Text("Player Proximity Condition");
            ImGui::InputFloat("##prox", &cfg::autoDcCondition, 0, 0, "%.1f");
			if (ImGui::Button(cfg::autoDcPrimed ? "Primed, Disconnect Ready" : "Not Primed!")) cfg::autoDcPrimed = !cfg::autoDcPrimed;
			ImGui::End();
        }

        if (cfg::autoSellcfg) {
            ImGui::Begin("Auto Sell", &cfg::autoSellcfg);
			ImGui::SliderInt("Delay (ms)", &cfg::autoSellDelay, 5, 500);
            ImGui::InputText("Price", cfg::autoSellPrice, IM_ARRAYSIZE(cfg::autoSellPrice));
			ImGui::Text("Endpoints:");
			ImGui::InputInt("Start Slot", &cfg::autoSellEndpoints[0]);
			ImGui::InputInt("Stop Slot", &cfg::autoSellEndpoints[1]);
            if (ImGui::Button("Trigger Sell")) cfg::triggerAutoSell = true;
			ImGui::End();

			if (cfg::autoSellEndpoints[0] < 1) cfg::autoSellEndpoints[0] = 1;
			if (cfg::autoSellEndpoints[0] > 9) cfg::autoSellEndpoints[0] = 9;
			if (cfg::autoSellEndpoints[1] > 9) cfg::autoSellEndpoints[1] = 9;
			if (cfg::autoSellEndpoints[1] < 1) cfg::autoSellEndpoints[1] = 1;
        }

        if (cfg::autoTotemcfg) {
			ImGui::Begin("Auto Totem", &cfg::autoTotemcfg);
			ImGui::Text("Auto Totem is %s", cfg::autoTotem ? "enabled" : "disabled");
			ImGui::SliderInt("Delay (ms)", &cfg::autoTotemDelay, 5, 500);
			ImGui::SliderInt("Humanity", &cfg::autoTotemHumanity, 0, 200);
			ImGui::End();
        }

        if (cfg::oreSimcfg) {
            // Seed-Ray window with scanline effect
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.03f, 0.08f, 0.95f));
            ImGui::Begin("Seed-Ray Config", &cfg::oreSimcfg);

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

            ImGui::TextColored(neon_blue, "Seed-Ray is %s", cfg::oreSim ? "enabled" : "disabled");
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

        if (cfg::autoCrystalcfg) {
			ImGui::Begin("Auto Crystal", &cfg::autoCrystalcfg);
            ImGui::Text("Auto Crystal is %s", cfg::autoCrystal ? "enabled" : "disabled");
            ImGui::SliderInt("Attack Time (ms)", &cfg::crystalAttackTime, 5, 500);
			ImGui::SliderInt("Place Time (ms)", &cfg::crystalPlaceTime, 5, 500);
			ImGui::End();
        }

        if (cfg::checkPlayerAirSafetycfg) {
			ImGui::Begin("Player Dig Safety", &cfg::checkPlayerAirSafetycfg);
			ImGui::Text("Player Dig Safety is %s", cfg::checkPlayerAirSafety ? "enabled" : "disabled");
            ImGui::Checkbox("Show Status In-Game", &cfg::isPlayerAirSafeShowStatus);
			ImGui::End();
        }

        if (cfg::advEspcfg) {
            ImGui::Begin("Advanced ESP", &cfg::advEspcfg);

            ImGui::Text("ESP Settings:");
            ImGui::SliderInt("Esp Radius", &cfg::espRadius, 16, 128);
            ImGui::SliderInt("Batch Size x1K", &cfg::espBatchSize, 50, 1000);
            ImGui::SliderInt("Search Time (sec)", &cfg::espSearchTime, 0, 20);
            ImGui::Checkbox("Draw Blocks", &cfg::drawBlocks);
            if (cfg::drawBlocks) ImGui::Checkbox("Draw Tracers", &cfg::drawBlockTracer);

            // Add new block input
            ImGui::InputText("Block Name", cfg::blockName, sizeof(cfg::blockName));
            ImGui::ColorEdit4("Block Color", (float*)&cfg::blockColor);

            if (ImGui::Button("Add Block to ESP")) {
                // Check if the block already exists in the list
                bool exists = false;
                for (const auto& blockColorItem : cfg::espBlockList) {
                    if (blockColorItem.name == cfg::blockName) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    cfg::espBlockList.push_back(EspBlock(cfg::blockName, cfg::blockColor, true)); // Add the new block with color
                }
            }


            ImGui::Separator();
            ImGui::Text("Current ESP Blocks:");
            ImGui::BeginChild("BlockListChild", ImVec2(0, 150), true);

            for (int i = 0; i < cfg::espBlockList.size(); ++i) {
                std::string label = "##" + cfg::espBlockList[i].name;
                ImGui::PushID(i);
                ImGui::Checkbox(label.c_str(), &cfg::espBlockList[i].enabled);
                ImGui::SameLine();
                ImGui::ColorEdit4("##color", (float*)&cfg::espBlockList[i].color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                ImGui::SameLine();
                ImGui::TextUnformatted(cfg::espBlockList[i].name.c_str());
                ImGui::SameLine(ImGui::GetWindowWidth() - 120);
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

    if (cfg::storageScancfg || (cfg::storageScanShow && cfg::storageScanShowInGui)) {
        if (!cfg::showMenu) cfg::storageScancfg = false;
        ImGui::Begin("Storage Scan", &cfg::storageScancfg);
        ImGui::Text("Storage Scan is %s", cfg::storageScan ? "enabled" : "disabled");
        ImGui::ColorEdit4("Highlight Color", (float*)&cfg::storageScanColor, ImGuiColorEditFlags_NoInputs);
        ImGui::InputText("Search For", cfg::storageScanSearch, IM_ARRAYSIZE(cfg::storageScanSearch));
        ImGui::Checkbox("Show Config In Gui", &cfg::storageScanShowInGui);
        ImGui::End();
    }

    if (cfg::displayPlayers) {
        ImGui::Begin("Nearby Players");

        // Top-level freeze toggle
        ImGui::Checkbox(ICON_FA_SNOWFLAKE " Freeze List", &cfg::freezePlayers);
        if (cfg::freezePlayers) {
            ImGui::TextColored(ImVec4(0.8f, 0.5f, 0.2f, 1.0f), "List is frozen.");
        }
        ImGui::Separator();

        for (const PlayerInfo& entry : cfg::nearbyPlayers) {
            const std::string& name = entry.name;
            float distance = entry.distance;
            const std::array<std::string, 4>& armor = entry.armor;
            const std::string& mainhand = entry.mainhand;
            const std::string& offhand = entry.offhand;
            float health = entry.health;
            int armorTuffness = entry.armorTuffness;
            bool isSneaking = entry.isSneaking;
            bool isSprinting = entry.isSprinting;

            std::string treeID = name + "##" + name;
            std::string headerLabel = name + " - " + std::format("{:.1f}m", distance);

            if (ImGui::TreeNode(treeID.c_str(), "%s", headerLabel.c_str())) {
                if (ImGui::TreeNode("Stats")) {
                    ImGui::Text("Health -> %.1f", health);
                    ImGui::Text("Armor Rating -> %d", armorTuffness);
                    ImGui::Text("Sprinting -> %s", isSprinting ? "yes" : "no");
                    ImGui::Text("Sneaking -> %s", isSneaking ? "yes" : "no");
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Armor")) {
                    ImGui::Text("Helm -> %s", armor[0].c_str());
                    ImGui::Text("Chest -> %s", armor[1].c_str());
                    ImGui::Text("Leg -> %s", armor[2].c_str());
                    ImGui::Text("Boot -> %s", armor[3].c_str());
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Hands")) {
                    ImGui::Text("Main -> %s", mainhand.c_str());
                    ImGui::Text("Off -> %s", offhand.c_str());
                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }
        }

        ImGui::End();
    }

    if (cfg::checkPlayerAirSafety && cfg::isPlayerAirSafeShowStatus) {
		ImGui::Begin("Dig Safety");
        ImGui::Text(cfg::isPlayerAirSafe.c_str());
		ImGui::End();
    }

    if (cfg::forwardTunnel) {
        ImGui::Begin("Tunneling Status");
        ImGui::Text(cfg::tunnelBlockStatus.c_str());
        ImGui::End();
    }

    if (!singleInit) {
        // use if u need it
        singleInit = true;
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
    serverAddr.sin_port = htons(getPort());
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
				config["oreSimColor"] = vec4Arr(cfg::oreSimColor);
				config["autoTotem"] = cfg::autoTotem;
				config["autoTotemDelay"] = cfg::autoTotemDelay;
				config["autoTotemHumanity"] = cfg::autoTotemHumanity;
				config["autoSellDelay"] = cfg::autoSellDelay;
				config["autoSellPrice"] = cfg::autoSellPrice;
				config["triggerAutoSell"] = cfg::triggerAutoSell;
				config["autoSellEndpointStart"] = cfg::autoSellEndpoints[0] -1;
				config["autoSellEndpointStop"] = cfg::autoSellEndpoints[1] -1;
				config["autoDcPrimed"] = cfg::autoDcPrimed;
				config["autoDcCondition"] = cfg::autoDcCondition;
                config["filterMode"] = cfg::filterMode;
				config["chatFilter"] = cfg::chatFilter;
				config["blockMsg"] = cfg::blockMsg;
                config["storageScan"] = cfg::storageScan;
				config["storageScanSearch"] = cfg::storageScanSearch;
				config["storageScanColor"] = vec4Arr(cfg::storageScanColor);
                config["storageScanShowInGui"] = cfg::storageScanShowInGui;
                config["aimAssistToggle"] = cfg::aimAssistToggle;
				config["aimAssistRange"] = cfg::aimAssistrange;
				config["aimAssistFov"] = cfg::aimAssistfov;
				config["aimAssistSmoothness"] = cfg::aimAssistsmoothness;
				config["aimAssistMinSpeed"] = cfg::aimAssistminSpeed;
				config["aimAssistMaxSpeed"] = cfg::aimAssistmaxSpeed;
				config["aimAssistUpdateRate"] = cfg::aimAssistupdateRate;
                config["aimAssistVisibility"] = cfg::aimAssistvisibilityCheck;
                config["crystalSpam"] = cfg::crystalSpam;
				config["crystalSpamSearchRadius"] = cfg::crystalSpamSearchRadius;
				config["crystalSpamBreakDelay"] = cfg::crystalSpamBreakDelay;
				config["nightFx"] = cfg::nightFx;

                cfg::triggerAutoSell = false;
                json espBlocksJson = json::array();
                for (const auto& block : cfg::espBlockList) {
                        espBlocksJson.push_back({
                            {"name", block.name},
                            {"color", vec4Arr(block.color)},
							{"enabled", block.enabled}
                            });
                }
                config["espBlockList"] = espBlocksJson;

                std::string response = config.dump() + "\n";
                int remaining = static_cast<int>(response.length());
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
            else if (message.find("STATUS") != std::string::npos && !cfg::freezePlayers) {
                try {
                    size_t jsonStart = message.find_first_of('{');

                    std::string jsonStr = message.substr(jsonStart);
                    json data = json::parse(jsonStr);

                    // Render flags
                    cfg::showMenu = data.value("shouldRender", false);
                    cfg::showAll = data.value("worldReady", false);

                    // Player safety
                    cfg::isPlayerAirSafe = data.value("playerAirSafety", "");

                    // Tunnel block status
                    cfg::tunnelBlockStatus = data.value("tunnelBlockStatus", "");

                    // GUI Scanner
                    cfg::storageScanShow = data.value("sendGuiStorageScanner", false);

                    // Crosshair
                    cfg::nightFxDraw = data.value("sendCrosshairDraw", false);

                    // Player list
                    if (data.contains("PLAYERS") && data["PLAYERS"].is_array()) {
                        cfg::nearbyPlayers.clear();

                        for (const auto& playerJson : data["PLAYERS"]) {
                            if (!playerJson.is_object()) continue;

                            PlayerInfo pInfo;

                            if (playerJson.contains("name") && playerJson["name"].is_string())
                                pInfo.name = playerJson["name"].get<std::string>();

                            if (playerJson.contains("distance") && playerJson["distance"].is_number_float())
                                pInfo.distance = playerJson["distance"].get<float>();

                            if (playerJson.contains("armor") && playerJson["armor"].is_array()) {
                                int idx = 0;
                                for (const auto& armorPiece : playerJson["armor"]) {
                                    if (idx >= 4) break; // Just 4 armor pieces expected
                                    if (armorPiece.is_string())
                                        pInfo.armor[idx++] = armorPiece.get<std::string>();
                                }
                                // If less than 4, remaining stay empty strings
                                for (; idx < 4; idx++) {
                                    pInfo.armor[idx] = "";
                                }
                            }
                            else {
                                // default empty armor
                                pInfo.armor.fill("");
                            }

                            if (playerJson.contains("mainhand") && playerJson["mainhand"].is_string())
                                pInfo.mainhand = playerJson["mainhand"].get<std::string>();
                            else pInfo.mainhand = "";

                            if (playerJson.contains("offhand") && playerJson["offhand"].is_string())
                                pInfo.offhand = playerJson["offhand"].get<std::string>();
                            else pInfo.offhand = "";


                            if (playerJson.contains("health") && playerJson["health"].is_number_float())
                                pInfo.health = playerJson["health"].get<float>();
                            else pInfo.health = 0.f;

                            if (playerJson.contains("armorTuffness") && playerJson["armorTuffness"].is_number_integer())
                                pInfo.armorTuffness = playerJson["armorTuffness"].get<int>();
                            else pInfo.armorTuffness = 0;

                            if (playerJson.contains("isSneaking") && playerJson["isSneaking"].is_boolean())
                                pInfo.isSneaking = playerJson["isSneaking"].get<bool>();
                            else pInfo.isSneaking = false;

                            if (playerJson.contains("isSprinting") && playerJson["isSprinting"].is_boolean())
                                pInfo.isSprinting = playerJson["isSprinting"].get<bool>();
                            else pInfo.isSprinting = false;



                            cfg::nearbyPlayers.push_back(std::move(pInfo));
                        }
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "[TCP] Failed to parse STATUS message: " << e.what() << "\n";
                }
            }
            else if (message.find("autoDcPrimedDisable") != std::string::npos) {
                try {
                    size_t jsonStart = message.find_first_of('{');
                    if (jsonStart != std::string::npos) {
                        json renderJson = json::parse(message.substr(jsonStart));
                        cfg::autoDcPrimed = renderJson.value("autoDcPrimedDisable", false);
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "[TCP] Failed to parse render flag: " << e.what() << "\n";
                }
			}
        }

        Sleep(5);
    }

    closesocket(clientSock);
    closesocket(listenSock);
    WSACleanup();
    FreeLibraryAndExitThread((HMODULE)lpParam, 0);
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
	config["oreSimColor"] = vec4Arr(cfg::oreSimColor);
	config["autoTotem"] = cfg::autoTotem;
	config["autoTotemDelay"] = cfg::autoTotemDelay;
	config["autoTotemHumanity"] = cfg::autoTotemHumanity;
	config["autoSellDelay"] = cfg::autoSellDelay;
	config["autoSellPrice"] = cfg::autoSellPrice;
	config["autoSell"] = cfg::autoSell;
	config["autoSellEndpointStart"] = cfg::autoSellEndpoints[0];
	config["autoSellEndpointStop"] = cfg::autoSellEndpoints[1];
	config["autoDc"] = cfg::autoDc;
	config["autoDcCondition"] = cfg::autoDcCondition;
	config["backgroundFx"] = cfg::backgroundFx;
	config["filterMode"] = cfg::filterMode;
	config["chatFilter"] = cfg::chatFilter;
	config["blockMsg"] = cfg::blockMsg;
	config["fontSizeOverride"] = cfg::fontSizeOverride;
	config["fontSize"] = cfg::fontSize;
	config["aimAssistToggle"] = cfg::aimAssistToggle;
    config["storageScan"] = cfg::storageScan;
    config["storageScanSearch"] = cfg::storageScanSearch;
    config["storageScanColor"] = vec4Arr(cfg::storageScanColor);
    config["storageScanShowInGui"] = cfg::storageScanShowInGui;
    config["aimAssistToggle"] = cfg::aimAssistToggle;
    config["aimAssistRange"] = cfg::aimAssistrange;
    config["aimAssistFov"] = cfg::aimAssistfov;
    config["aimAssistSmoothness"] = cfg::aimAssistsmoothness;
    config["aimAssistMinSpeed"] = cfg::aimAssistminSpeed;
    config["aimAssistMaxSpeed"] = cfg::aimAssistmaxSpeed;
    config["aimAssistUpdateRate"] = cfg::aimAssistupdateRate;
    config["aimAssistVisibility"] = cfg::aimAssistvisibilityCheck;
    config["crystalSpam"] = cfg::crystalSpam;
    config["crystalSpamSearchRadius"] = cfg::crystalSpamSearchRadius;
    config["crystalSpamBreakDelay"] = cfg::crystalSpamBreakDelay;
	config["nightFx"] = cfg::nightFx;
	config["nightFxSize"] = cfg::nightFxSize;
	config["nightFxCrosshairLines"] = cfg::nightFxCrosshairLines;

    json blocksJson = json::array();
    for (const auto& block : cfg::espBlockList) {
        blocksJson.push_back({
            {"name", block.name},
            {"color", vec4Arr(block.color)},
			{"enabled", block.enabled}
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
		if (config.contains("autoTotem")) cfg::autoTotem = config["autoTotem"].get<bool>();
		if (config.contains("autoTotemDelay")) cfg::autoTotemDelay = config["autoTotemDelay"].get<int>();
		if (config.contains("autoTotemHumanity")) cfg::autoTotemHumanity = config["autoTotemHumanity"].get<int>();
		if (config.contains("autoSellDelay")) cfg::autoSellDelay = config["autoSellDelay"].get<int>();
		if (config.contains("autoSellPrice")) {
			std::string price = config["autoSellPrice"].get<std::string>();
			strncpy_s(cfg::autoSellPrice, price.c_str(), sizeof(cfg::autoSellPrice));
		}
		if (config.contains("autoSell")) cfg::autoSell = config["autoSell"].get<bool>();
		if (config.contains("autoSellEndpointStart")) cfg::autoSellEndpoints[0] = config["autoSellEndpointStart"].get<int>();
		if (config.contains("autoSellEndpointStop")) cfg::autoSellEndpoints[1] = config["autoSellEndpointStop"].get<int>();
		if (config.contains("autoDc")) cfg::autoDc = config["autoDc"].get<bool>();
		if (config.contains("autoDcCondition")) cfg::autoDcCondition = config["autoDcCondition"].get<float>();
		if (config.contains("backgroundFx")) cfg::backgroundFx = config["backgroundFx"].get<bool>();
		if (config.contains("filterMode")) cfg::filterMode = config["filterMode"].get<int>();
		if (config.contains("chatFilter")) cfg::chatFilter = config["chatFilter"].get<bool>();
        if (config.contains("blockMsg")) {
            std::string msg = config["blockMsg"].get<std::string>();
            strncpy_s(cfg::blockMsg, MAX_PATH, msg.c_str(), _TRUNCATE);
        }
		if (config.contains("fontSizeOverride")) cfg::fontSizeOverride = config["fontSizeOverride"].get<bool>();
		if (config.contains("fontSize")) cfg::fontSize = config["fontSize"].get<float>();
		if (config.contains("storageScan")) cfg::storageScan = config["storageScan"].get<bool>();
		if (config.contains("storageScanSearch")) {
			std::string search = config["storageScanSearch"].get<std::string>();
			strncpy_s(cfg::storageScanSearch, MAX_PATH, search.c_str(), _TRUNCATE);
		}
		if (config.contains("storageScanColor") && config["storageScanColor"].is_array() && config["storageScanColor"].size() == 4)
			cfg::storageScanColor = ImVec4(config["storageScanColor"][0], config["storageScanColor"][1], config["storageScanColor"][2], config["storageScanColor"][3]);
        if (config.contains("storageScanShowInGui")) cfg::storageScanShowInGui = config["storageScanShowInGui"].get<bool>();
        if (config.contains("aimAssistToggle")) cfg::aimAssistToggle = config["aimAssistToggle"].get<bool>();
    	if (config.contains("aimAssistRange")) cfg::aimAssistrange = config["aimAssistRange"].get<float>();
		if (config.contains("aimAssistFov")) cfg::aimAssistfov = config["aimAssistFov"].get<float>();
		if (config.contains("aimAssistSmoothness")) cfg::aimAssistsmoothness = config["aimAssistSmoothness"].get<float>();
		if (config.contains("aimAssistMinSpeed")) cfg::aimAssistminSpeed = config["aimAssistMinSpeed"].get<float>();
		if (config.contains("aimAssistMaxSpeed")) cfg::aimAssistmaxSpeed = config["aimAssistMaxSpeed"].get<float>();
		if (config.contains("aimAssistUpdateRate")) cfg::aimAssistupdateRate = config["aimAssistUpdateRate"].get<int>();
		if (config.contains("aimAssistVisibility")) cfg::aimAssistvisibilityCheck = config["aimAssistVisibility"].get<bool>();
		if (config.contains("crystalSpam")) cfg::crystalSpam = config["crystalSpam"].get<bool>();
		if (config.contains("crystalSpamSearchRadius")) cfg::crystalSpamSearchRadius = config["crystalSpamSearchRadius"].get<int>();
		if (config.contains("crystalSpamBreakDelay")) cfg::crystalSpamBreakDelay = config["crystalSpamBreakDelay"].get<int>();
		if (config.contains("nightFx")) cfg::nightFx = config["nightFx"].get<bool>();
		if (config.contains("nightFxSize")) cfg::nightFxSize = config["nightFxSize"].get<float>();
		if (config.contains("nightFxCrosshairLines")) cfg::nightFxCrosshairLines = config["nightFxCrosshairLines"].get<bool>();


        // Load ESP blocks
        cfg::espBlockList.clear();
        if (config.contains("espBlockList") && config["espBlockList"].is_array()) {
            for (const auto& item : config["espBlockList"]) {
                if (item.contains("name") && item.contains("color") && item["color"].size() == 4) {
                    cfg::espBlockList.push_back({
                        item["name"],
                        ImVec4(item["color"][0], item["color"][1], item["color"][2], item["color"][3]),
						item.value("enabled", true)
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