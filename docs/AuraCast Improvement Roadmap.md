# AuraCast Improvement Roadmap

This roadmap turns the current product goals into a staged plan aligned with the existing Flycast-based architecture in `core/rend`, `core/ui`, `core/network`, and the platform shells under `shell`.

## Product goals

- **Visual quality:** improve default presentation, renderer consistency, and high-resolution stability across supported graphics APIs.
- **Usability:** reduce setup friction, modernize the UI flow, and improve controller-first navigation.
- **Online reliability:** make multiplayer easier to start, easier to diagnose, and more stable in real-world network conditions.
- **Emulation quality:** improve compatibility, frame pacing, input latency, save-state reliability, and cross-platform consistency.

## Success criteria

Before implementation work starts, each milestone should define measurable outcomes:

- Lower crash and renderer reset rates on supported APIs.
- Higher completion rates for first-run setup and common settings tasks.
- Better online session success rates, fewer disconnects, and clearer desync reporting.
- Improved compatibility and stability across the most-played Dreamcast and Naomi titles.

## Phase 1: Graphics quality upgrades

**Primary areas:** `core/rend`, `core/ui/settings_video.cpp`

- Improve graphics defaults so new users get strong image quality without manual tuning.
- Prioritize renderer-specific quality work for Vulkan, OpenGL, and DirectX paths.
- Tighten transparency handling, framebuffer accuracy, and upscaling behavior.
- Review shader behavior and API-specific rendering regressions for consistency across platforms.
- Clarify in-UI guidance for costly options such as framebuffer emulation and custom textures.
- Build and maintain a graphics compatibility matrix for high-visibility Dreamcast and Naomi games.

**Exit criteria**

- Stable high-resolution rendering for the main renderer paths.
- Fewer known graphics regressions in popular titles.
- Clearer recommended presets for low-end, balanced, and high-quality usage.

## Phase 2: UI modernization

**Primary areas:** `core/ui`, platform-specific presentation under `shell`

- Redesign the settings flow around clearer categories and fewer dense screens.
- Improve onboarding for video, controls, saves, and network configuration.
- Refresh spacing, typography, icon usage, and controller navigation patterns.
- Add stronger inline help, better status/error messaging, and more discoverable options.
- Introduce settings search or filtering to reduce time spent hunting through menus.
- Standardize the core UI behavior first, then apply shell-specific polish where needed.

**Exit criteria**

- Faster first-run setup and fewer abandoned settings flows.
- Better usability with mouse, keyboard, touch, and controller input.
- More consistent messaging for renderer and network problems.

## Phase 3: Better online multiplayer

**Primary areas:** `core/network`, `core/ui/settings_network.cpp`

- Build on existing GGPO, Naomi networking, ICE, UPnP, and DCNet support.
- Simplify connection setup with clearer host/client language and friendlier session flow.
- Improve matchmaking and peer/session creation where feasible within the current architecture.
- Focus on NAT traversal, reconnect handling, latency reporting, desync detection, and diagnostics.
- Expand session features such as chat improvements, presence, spectator support, and recovery tools where practical.
- Add compatibility rules so unsupported or unstable game/network combinations are not surfaced as ready-to-use options.

**Exit criteria**

- Higher successful connection rates for supported games.
- Better visibility into latency, peer state, and failure causes.
- Lower support burden caused by unclear setup or unsupported combinations.

## Phase 4: Better overall emulator

**Primary areas:** core emulation, renderer integration, audio, input, save states, and platform shells

- Prioritize improvements users feel immediately: stability, frame pacing, latency, save-state reliability, and compatibility.
- Strengthen regression coverage for graphics, audio, networking, save states, and popular game paths before deeper feature work.
- Improve defaults so common hardware and game setups work well out of the box.
- Reduce platform drift by aligning core behavior before platform-specific polish.
- Use the compatibility matrix to focus work on the highest-impact games and hardware paths first.

**Exit criteria**

- Fewer regressions in core gameplay scenarios.
- Better consistency across desktop, Android, and console targets.
- Improved default experience for new users without extra configuration.

## Phase 5: Release and feedback loop

- Ship in small milestones instead of one large release.
- Track milestone outcomes with metrics for crash rate, frame-time stability, game compatibility, online session success, and settings completion.
- Use community feedback from the most-played games to reprioritize between milestones.
- Keep a visible backlog of top renderer issues, UI friction points, and online reliability gaps.

## Suggested delivery order

1. UI and network onboarding improvements plus better graphics defaults.
2. Online reliability, NAT/desync diagnostics, and session quality work.
3. Renderer accuracy, shader consistency, and performance stabilization.
4. Broader emulator compatibility, defaults tuning, and platform polish.

## Milestone planning guidance

Each milestone should include:

- A short scope with in-scope and out-of-scope items.
- A measurable success target.
- A validation plan covering popular Dreamcast and Naomi titles.
- A rollback plan for high-risk graphics and networking changes.
