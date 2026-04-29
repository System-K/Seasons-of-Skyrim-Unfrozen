#include "Hooks.h"

#include "Settings.h"
#include "ClibUtil/editorID.hpp"
#include "ClibUtil/string.hpp"

namespace Hooks {

    RE::NiAVObject* RefLoadHook::Load3D(RE::TESObjectREFR* a_this, bool a_backgroundLoading) {
        auto start = std::chrono::high_resolution_clock::now();
        if (auto base = a_this->GetBaseObject()) {
            if (auto stat = base->As<RE::TESObjectSTAT>()) {

                auto world = a_this->GetWorldspace();
                if (!Config::GetSingleton()->IsWalidWorldspace(world)) {
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> elapsed = end - start;
                    //logger::debug("Load3D not Walid Worldspace in {} ms", elapsed.count());
                    return Load3D_(a_this, a_backgroundLoading);
                }
                auto month = RE::Calendar::GetSingleton()->GetMonth();
                if (!Config::GetSingleton()->IsSummerMonth(month)) {
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> elapsed = end - start;
                    //logger::debug("Load3D not Summer Month in {} ms", elapsed.count());
                    return Load3D_(a_this, a_backgroundLoading);
                }

                if (stat->data.materialObj) {
                    auto editorID = clib_util::editorID::get_editorID(stat->data.materialObj);
                    editorID = clib_util::string::tolower(editorID);
                    if (editorID.contains("snow") || editorID.contains("ice") || editorID.contains("frost")) {
                        stat->data.materialObj = nullptr;
                        auto end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> elapsed = end - start;
                        //logger::debug("Load3D Removed in {} ms", elapsed.count());
                    }
                }
            }
        }

        return Load3D_(a_this, a_backgroundLoading);
    }
}