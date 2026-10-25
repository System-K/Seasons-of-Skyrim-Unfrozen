# Seasons Unfrozen NG11 rebuild

Build using sh work/seasons-unfrozen/build-linux.sh with the workbench toolchain. Default builds CommonLib from pinned source; optional COMMONLIB_WORKBENCH_CACHE reuses our own SHA/compiler/CRT-checked source-built library. This session used that cache (not MSVC prebuilt binaries).

All upstream src/ and include/ bytes are unchanged. CMake and dependency manifest alone changed. The DLL name and upstream metadata version0.1.0.0 are retained; author metadata is RavenKZP rather than the template's literal AUTHOR_NAME token. Removed automatic game-folder deployment and the template's independent rc command; generated SKSE metadata still present.

Only referenced dependencies retained: workbench NG11 dependencies plus CLibUtil from the workbench's existing overlay at pinned commit e1eabe6. The old template's unused clib-utils-qtr, skse-mcp and fast-cpp-csv-parser dependencies were not substituted with fabricated APIs; the only non-CommonLib helper headers actually included are ClibUtil/editorID.hpp and string.hpp. Build tested with fmt/spdlog versions from the workbench baseline rather than the obsolete fmt9 override.

One vtable hook, TESObjectREFR::Load3D slot0x6A, matches the pinned header signature. No in-function patch offsets or trampoline allocations. Actual target-runtime vtable/interface interactions still require testing.

Permissions/provenance are documented separately. No standard license is invented for the author's code. No real game paths were written.
