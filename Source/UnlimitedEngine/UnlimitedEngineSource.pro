TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11
# SFML
LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system

SOURCES += \
        main.cpp \
    lua/lapi.c \
    lua/lauxlib.c \
    lua/lbaselib.c \
    lua/lbitlib.c \
    lua/lcode.c \
    lua/lcorolib.c \
    lua/lctype.c \
    lua/ldblib.c \
    lua/ldebug.c \
    lua/ldo.c \
    lua/ldump.c \
    lua/lfunc.c \
    lua/lgc.c \
    lua/linit.c \
    lua/liolib.c \
    lua/llex.c \
    lua/lmathlib.c \
    lua/lmem.c \
    lua/loadlib.c \
    lua/lobject.c \
    lua/lopcodes.c \
    lua/loslib.c \
    lua/lparser.c \
    lua/lstate.c \
    lua/lstring.c \
    lua/lstrlib.c \
    lua/ltable.c \
    lua/ltablib.c \
    lua/ltests.c \
    lua/ltm.c \
    lua/lundump.c \
    lua/lutf8lib.c \
    lua/lvm.c \
    lua/lzio.c \
    Core/Animation.cpp \
    Core/Application.cpp \
    Core/BloomEffect.cpp \
    Core/Command.cpp \
    Core/CommandQueue.cpp \
    Core/DataTables.cpp \
    Core/EmitterNode.cpp \
    Core/Entity.cpp \
    Core/MusicPlayer.cpp \
    Core/ParallelTask.cpp \
    Core/Player.cpp \
    Core/PostEffect.cpp \
    Core/ResourceManager.inl \
    Core/SceneNode.cpp \
    Core/SoundNode.cpp \
    Core/SoundPlayer.cpp \
    Core/SpriteNode.cpp \
    Core/State.cpp \
    Core/StateStack.cpp \
    Core/TextNode.cpp \
    Core/Utility.cpp \
    Core/Utility.inl \
    States/GameState.cpp \
    States/LoadingState.cpp \
    States/MenuState.cpp \
    States/PauseState.cpp \
    States/TitleState.cpp \
    Gui/Component.cpp \
    Gui/Container.cpp \
    Gui/Label.cpp \
    States/SplashScreen.cpp \
    Gui/Button.cpp \
    States/SettingsState.cpp \
    Core/ParticleNode.cpp \
    Core/VertexArrayNode.cpp \
    Objects/Actor.cpp \
    Objects/Item.cpp \
    Objects/Wall.cpp \
    Objects/Warp.cpp \
    Core/RectangleShapeNode.cpp \
    Gui/MessageBoxNode.cpp \
    Gui/ResponseSelector.cpp \
    States/MessageBoxState.cpp \
    Core/CollisionMan.cpp \
    Objects/Cowboy.cpp

HEADERS += \
    lua/lapi.h \
    lua/lauxlib.h \
    lua/lcode.h \
    lua/lctype.h \
    lua/ldebug.h \
    lua/ldo.h \
    lua/lfunc.h \
    lua/lgc.h \
    lua/llex.h \
    lua/llimits.h \
    lua/lmem.h \
    lua/lobject.h \
    lua/lopcodes.h \
    lua/lparser.h \
    lua/lprefix.h \
    lua/lstate.h \
    lua/lstring.h \
    lua/ltable.h \
    lua/ltests.h \
    lua/ltm.h \
    lua/lua.h \
    lua/luaconf.h \
    lua/lualib.h \
    lua/lundump.h \
    lua/lvm.h \
    lua/lzio.h \
    Core/Animation.hpp \
    Core/Application.hpp \
    Core/BloomEffect.hpp \
    Core/Category.hpp \
    Core/Command.hpp \
    Core/CommandQueue.hpp \
    Core/DataTables.hpp \
    Core/EmitterNode.hpp \
    Core/Entity.hpp \
    Core/Globals.hpp \
    Core/MusicPlayer.hpp \
    Core/ParallelTask.hpp \
    Core/Particle.hpp \
    Core/Player.hpp \
    Core/PostEffect.hpp \
    Core/ResourceIdentifiers.hpp \
    Core/ResourceManager.hpp \
    Core/SceneNode.hpp \
    Core/SoundNode.hpp \
    Core/SoundPlayer.hpp \
    Core/SpriteNode.hpp \
    Core/State.hpp \
    Core/StateIdentifiers.hpp \
    Core/StateStack.hpp \
    Core/TextNode.hpp \
    Core/Utility.hpp \
    States/GameState.hpp \
    States/LoadingState.hpp \
    States/MenuState.hpp \
    States/PauseState.hpp \
    States/TitleState.hpp \
    Gui/Component.hpp \
    Gui/Container.hpp \
    Gui/Label.hpp \
    States/SplashScreen.hpp \
    Gui/Button.hpp \
    States/SettingsState.hpp \
    Core/Particle.hpp \
    Core/ParticleNode.hpp \
    Tiled/TiledManager.hpp \
    Core/VertexArrayNode.hpp \
    Objects/Actor.hpp \
    Objects/Item.hpp \
    Objects/Wall.hpp \
    Objects/Warp.hpp \
    Core/RectangleShapeNode.hpp \
    Gui/MessageBoxNode.hpp \
    Core/DialogNode.hpp \
    Gui/ResponseSelector.hpp \
    States/MessageBoxState.hpp \
    Core/CollisionMan.hpp \
    Objects/Cowboy.hpp

DISTFILES += \
    Media/Sansation.ttf \
    Media/Shaders/Add.frag \
    Media/Shaders/Brightness.frag \
    Media/Shaders/DownSample.frag \
    Media/Shaders/GuassianBlur.frag \
    Media/Shaders/Fullpass.vert \
    Media/Sound/109340__cheesepuff__a-soldier-song-slow0.mp3 \
    Media/Sound/AlliedGunfire.wav \
    Media/Sound/Button.wav \
    Media/Sound/CollectAlly.wav \
    Media/Sound/CollectPickup.wav \
    Media/Sound/EnemyGunfire.wav \
    Media/Sound/Explosion1.wav \
    Media/Sound/FuelLow.wav \
    Media/Sound/LaunchMissile.wav \
    Media/Sound/LetsGo.wav \
    Media/Sound/PlayerDeath.wav \
    Media/Sound/ReturnToBase.wav \
    Media/Sound/TroopCountFull.wav \
    Media/Sound/SoundEffectCredits.txt \
    Media/Sound/Untitled Document \
    Media/Textures/soldier.jpg \
    Media/Textures/TitleScreen.jpg \
    Media/Textures/Buttons.png \
    Media/Textures/Explosion.png \
    Media/Textures/fueltank.png \
    Media/Textures/grass.png \
    Media/Textures/GreenTank.png \
    Media/Textures/GreenTurret.png \
    Media/Textures/Helicopter.png \
    Media/Textures/HelicopterBlades.png \
    Media/Textures/HUDbg.png \
    Media/Textures/landingpad.png \
    Media/Textures/MainMenu.png \
    Media/Textures/Particle.png \
    Media/Textures/soldier.png \
    Media/Textures/trees.png \
    Media/Textures/Wall.png \
    Game/Resources/Animations.lua \
    Game/Resources/Assets.lua \
    Game/Resources/Particles.lua \
    Game/Resources/SourceFileMap.lua \
    Game/Application.lua \
    Game/Game.lua \
    Media/Textures/DivittoUnlimitedLogo1024X768.png \
    Notes.txt \
    Game/Levels/Greenville.lua \
    Game/Resources/Actors.lua \
    Game/Resources/Items.lua \
    Game/Resources/Warps.lua \
    Game/Resources/Conversations.lua \
    Media/Textures/trainers.png \
    Media/Textures/tiles.png \
    Game/Levels/SevenSunsetDemo.lua \
    Media/Textures/wagonWheel.png \
    Media/Textures/woodTexture.png \
    Media/Textures/cowboy.png \
    Media/Textures/SevenSunSetDemoTexture.png \
    Game/Resources/ScriptedAnimations.lua
