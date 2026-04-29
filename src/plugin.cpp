#include "logger.h"

#include "Hooks.h"
#include "Settings.h"

void OnMessage(SKSE::MessagingInterface::Message* message) {
    if (message->type == SKSE::MessagingInterface::kPostPostLoad) {
        // After Seasons of Skyrim save the worldspace list, we can load it in our plugin
        Config::GetSingleton();  // init
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    SetupLog();
    logger::info("Plugin loaded");
    SKSE::Init(skse);
    Hooks::InstallHooks();
    SKSE::GetMessagingInterface()->RegisterListener(OnMessage);
    return true;
}
