#pragma once

namespace Hooks {

    struct RefLoadHook {
        static RE::NiAVObject* Load3D(RE::TESObjectREFR* a_this, bool a_backgroundLoading);
        static inline REL::Relocation<decltype(Load3D)> Load3D_;
    };

    inline void InstallHooks() {

        RefLoadHook::Load3D_ =
            REL::Relocation<std::uintptr_t>(RE::VTABLE_TESObjectREFR[0]).write_vfunc(0x6A, RefLoadHook::Load3D);
    }
}