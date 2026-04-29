#pragma once

#include "REX/REX.h"

#include <unordered_set>

class Config : public REX::Singleton<Config> {
public:
    Config() { LoadConfig();
    }

    bool IsSummerMonth(int month) const {
        if (permanentSummer) {
            return true;
        }
        if (seasonal) {
            return summerMonths.contains(month);
        }
        return false;
    }

    bool IsWalidWorldspace(RE::TESWorldSpace* world) const {
        if (!world) return false;
        auto editorID = world->GetFormEditorID();
        return validWorldspaces.contains(editorID);
    }

private:
    void LoadConfig() {
        const std::string& iniPath = "Data\\SKSE\\Plugins\\po3_SeasonsOfSkyrim.ini";
        static const std::vector<std::string> monthOrder = {
            "Morning Star", "Sun's Dawn", "First Seed", "Rain's Hand", "Second Seed", "Mid Year",
            "Sun's Height", "Last Seed",  "Hearthfire", "Frost Fall",  "Sun's Dusk",  "Evening Star"};

        std::ifstream file(iniPath);
        std::string line;
        enum class Section { None, Settings, Summer };
        Section currentSection = Section::None;

        while (std::getline(file, line)) {
            line.erase(line.begin(),
                       std::find_if(line.begin(), line.end(), [](unsigned char ch) { return !std::isspace(ch); }));
            line.erase(
                std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(),
                line.end());

            if (line.empty() || line[0] == ';') continue;

            if (line.contains("[Settings]")) {
                currentSection = Section::Settings;
                continue;
            } else if (line.contains("[Summer]")) {
                currentSection = Section::Summer;
                continue;
            } else if (line[0] == '[') {
                currentSection = Section::None;
                continue;
            }

            auto eq = line.find('=');
            if (eq == std::string::npos) continue;

            std::string key = line.substr(0, eq);
            std::string value = line.substr(eq + 1);
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));

            if (currentSection == Section::Settings) {
                if (key.contains("Season Type") && value == "3") {
                    permanentSummer = true;
                } else if (key.contains("Season Type") && value == "5") {
                    seasonal = true;
                }

                auto it = std::find(monthOrder.begin(), monthOrder.end(), key);
                if (it != monthOrder.end() && value == "3") {
                    int monthIndex = static_cast<int>(std::distance(monthOrder.begin(), it));
                    summerMonths.insert(monthIndex);
                    logger::info("Added month {} as a summer month", key);
                } else if (it != monthOrder.end()) {
                    logger::info("Month {} isn't a summer month", key);
                }
            } else if (currentSection == Section::Summer) {
                if (key.contains("Worldspaces")) {
                    std::istringstream ss(value);
                    std::string editorID;
                    while (std::getline(ss, editorID, '|')) {
                        editorID.erase(0, editorID.find_first_not_of(" \t\r\n"));
                        editorID.erase(editorID.find_last_not_of(" \t\r\n") + 1);
                        if (!editorID.empty()) {
                            validWorldspaces.insert(editorID);
                            logger::info("Added worldspace {}", editorID);
                        }
                    }
                }
            }
        }
    }

    std::unordered_set<int> summerMonths;
    std::unordered_set<std::string> validWorldspaces;
    bool permanentSummer = false;
    bool seasonal = false;
};