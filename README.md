[![Build](https://github.com/Rex109/CoD4QOL/actions/workflows/msbuild.yml/badge.svg)](https://github.com/Rex109/CoD4QOL/actions/workflows/msbuild.yml)
[![License](https://img.shields.io/badge/license-CC_BY--NC_4.0-blue)](https://creativecommons.org/licenses/by-nc/4.0/)
[![Personal Discord](https://img.shields.io/discord/953653773962739793?color=%237289DA&label=Personal%20Discord&logo=discord&logoColor=%23FFFFFF)](https://discord.gg/QDYk75vBBk)
[![ManyAsset](https://img.shields.io/discord/585171589750849538?color=%23FF8711&label=ManyAsset&logo=discord&logoColor=%23FFFFFF)](https://discord.gg/v2TWkeR)


# CoD4QOL

> [!WARNING]
> Even if this tool is not directly a cheat, lots of players have been banned by servers while playing with this tool installed.
>
> Use it at your own risk!

![211422660-af8afa37-87a0-45c0-b1ff-a2fb60b77228 copy](https://github.com/Rex109/CoD4QOL/assets/8615649/b907fe74-cef3-4690-b28b-b5889f6dd9cd)
*<p align="center"><sub>Play the way YOU want</sub></p>*
<br>
A Call of Duty 4 X plugin that adds new settings, unlocks essential dvars and adds security options

## Features
- Using native menus, no flying imgui windows
- Easy to install
- Works on any server regardless of the mod
- Speeds up the game's boot time
- Adds an in-game demo browser/manager
- Fixes red reflections on older maps
- Customizable crosshair
- Increase LOD distance for better visuals
- Unlock all button + console command

### Complete patches list
- Adds "vm_anim" command
- Adds "LoadZone" command
- Adds "LoadIWD" command
- Adds "ReadProtectedConfig"/"WriteProtectedConfig" commands
- Unlocks cg_fovscale, r_fullbright, r_zfar, r_filmusetweaks, developer_script, cg_gun_*, cg_draw2d, cg_drawgun, bg_bobmax
- Adds "Additional graphics" menu:
  - Fov slider
  - Fovscale slider
  - Fullbright switch
  - Fullscreen switch
  - Maxfps selector
  - Drawfps selector
  - Draw distance selector
  - Lod scale skinned
  - FSR1 Upscaling
  - Film use tweaks toggle
  - Rigid model detail distance selector
  - Skinned model detail distance selector
  - Gun X, Y, Z sliders
  - Gun handedness switch
- Adds "Privacy options" menu:
  - Screenshot protection selector
  - Block vstr switch
  - Disable steam auth
- Adds "CoD4QOL settings" menu:
  - Check for updates on startup
  - Show external console
  - Display loading info
  - Play ambient music
  - Always load stock menu
  - Display main menu branding
  - Auto Bunnyhop
  - Disable slash commands
  - Show red reflection probes
  - Invert flashbangs
  - Disable flashbang sound
  - Load custom localized resources
  - Chat font size slider
  - Customize crosshair
  - Unlock All

## Download and installation
### Automated Install/Uninstall
Download the automatic [CoD4QOL Installer](https://github.com/Rex109/CoD4QOLInstaller)
### Manual Install
To download the pre-compiled binaries you can grab the latest version in the [Releases Page](https://github.com/Rex109/Cod4QOL/releases)

To install it simply download and extract the files from the .zip, then drag n drop the "miles" folder in your Call of Duty 4 root folder and the "qol.ff" file inside zone/\<your game language\>, and simply run the game, no extra tools needed!

The final folder structure should look like this:
```
.
├── ...
├── 📁 Call of Duty 4
│   ├── 📁 miles
|   |  └── 📜 cod4qol.asi
│   └── 📁 zone
|      └── 📁 <your game language>
|          └── 📜 qol.ff
└── ...
```
### Manual Uninstall
To uninstall CoD4QOL completely remove the following files from your game directory:
- miles/cod4qol.asi
- zone/\<your game language\>/qol.ff
- main/xcommon_cod4qol.iwd
- main/protected.cfg

The folder structure looks like this:
```
.
├── ...
├── 📁 Call of Duty 4
│   ├── 📁 main
|   |  ├── 📜 protected.cfg ❌
|   |  └── 📜 xcommon_cod4qol.iwd ❌
│   ├── 📁 miles
|   |  └── 📜 cod4qol.asi ❌
│   └── 📁 zone
|      └── 📁 <your game language>
|          └── 📜 qol.ff ❌
└── ...
```

Part of the user settings are stored inside config_mp.cfg, if you wish to delete them remove every line that starts with "qol_"

# FSR 1 4K Benchmark video
[![Call of Duty 4 - CoD4QOL FSR 1 4K Benchmark](https://img.youtube.com/vi/q7UkncTijTY/0.jpg)](https://www.youtube.com/watch?v=q7UkncTijTY)

## Limitations
- Does not work in single player
- Does not work with cod4 stock 1.7

## Pull requests and issues
If you have any suggestions i encourage you to open a pull request, same goes for issues: do not hesitate to open one if you encounter a problem.
For further support you can contact me directly in my personal discord server linked on top of the page or join ManyAsset, a community made entirely for CoD4 Modders.
