TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11
# SFML
LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system

DISTFILES += \
    Core/ip.txt \
    GDD.txt \
    Game/Application.lua \
    Game/Game.lua \
    Game/Lua/Application.lua \
    Game/Lua/Game.lua \
    Game/Lua/Resources/Actors.lua \
    Game/Lua/Resources/Assets.lua \
    Game/Lua/Resources/Conversations.lua \
    Game/Lua/Resources/Maps/Courtyard.lua \
    Game/Lua/Resources/Maps/TacticsTribeDemoLevel.lua \
    Game/Lua/Resources/Maps/smallMap.lua \
    Game/Lua/Resources/Particles.lua \
    Game/Lua/Resources/SourceFileMap.lua \
    Game/Lua/Resources/Warps.lua \
    Media/Sansation.ttf \
    Media/Shaders/Add.frag \
    Media/Shaders/Brightness.frag \
    Media/Shaders/DownSample.frag \
    Media/Shaders/Fullpass.vert \
    Media/Shaders/GuassianBlur.frag \
    Media/Sound/Button.wav \
    Media/Textures/Buttons.png \
    Media/Textures/Default.png \
    Media/Textures/DivittoUnlimitedLogo1024X768.png \
    Media/Textures/Particle.png \
    Media/Textures/PlayerAvatar.png \
    Media/Textures/TexturePack.png \
    Notes.txt \
    Story.txt

HEADERS += \
    Core/Application.hpp \
    Core/Category.hpp \
    Core/CollisionMan.hpp \
    Core/Command.hpp \
    Core/CommandQueue.hpp \
    Core/DialogNode.hpp \
    Core/Entity.hpp \
    Core/FiniteStateMachine.hpp \
    Core/FiniteStateMachine.inl \
    Core/GameModeIdentifiers.hpp \
    Core/GameServer.hpp \
    Core/Globals.hpp \
    Core/KeyBinding.hpp \
    Core/MusicPlayer.hpp \
    Core/NetworkNode.hpp \
    Core/NetworkProtocol.hpp \
    Core/ParallelTask.hpp \
    Core/PathFinder.cpp.autosave \
    Core/PathFinder.hpp \
    Core/PathFinder.inl \
    Core/PerlinNoise.hpp \
    Core/Player.hpp \
    Core/ResourceIdentifiers.hpp \
    Core/ResourceManager.hpp \
    Core/ResourceManager.inl \
    Core/SoundNode.hpp \
    Core/SoundPlayer.hpp \
    Core/State.hpp \
    Core/StateIdentifiers.hpp \
    Core/StateStack.hpp \
    Core/SteeringBehaviors.hpp \
    Core/SteeringBehaviors.inl \
    Core/Utility.hpp \
    Core/Utility.inl \
    Game/DataTables.hpp \
    Game/Dice.hpp \
    Game/Wall.hpp \
    Game/World.hpp \
    Graphics/Animation.hpp \
    Graphics/BloomEffect.hpp \
    Graphics/EmitterNode.hpp \
    Graphics/Particle.hpp \
    Graphics/ParticleNode.hpp \
    Graphics/PostEffect.hpp \
    Graphics/RectangleShapeNode.hpp \
    Graphics/SceneNode.hpp \
    Graphics/SpriteNode.hpp \
    Graphics/TextNode.hpp \
    Graphics/VertexArrayNode.hpp \
    Gui/Button.hpp \
    Gui/Component.hpp \
    Gui/Container.hpp \
    Gui/Label.hpp \
    Gui/MessageBoxNode.hpp \
    Gui/ResponseSelector.hpp \
    States/GameState.hpp \
    States/IntroCutSceneState.cpp.autosave \
    States/LoadingState.hpp \
    States/MenuState.hpp \
    States/MessageBoxState.hpp \
    States/MultiplayerGameState.hpp \
    States/PauseState.hpp \
    States/SettingsState.hpp \
    States/SplashScreen.hpp \
    States/TitleState.hpp \
    States/cutSceneState.hpp.autosave \
    Tiled/TiledManager.hpp \
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
    lua/lzio.h

SOURCES += \
    Core/Application.cpp \
    Core/CollisionMan.cpp \
    Core/Command.cpp \
    Core/CommandQueue.cpp \
    Core/Entity.cpp \
    Core/GameServer.cpp \
    Core/KeyBinding.cpp \
    Core/MusicPlayer.cpp \
    Core/NetworkNode.cpp \
    Core/ParallelTask.cpp \
    Core/PerlinNoise.cpp \
    Core/Player.cpp \
    Core/SoundNode.cpp \
    Core/SoundPlayer.cpp \
    Core/State.cpp \
    Core/StateStack.cpp \
    Core/Utility.cpp \
    Game/DataTables.cpp \
    Game/Dice.cpp \
    Game/Wall.cpp \
    Game/World.cpp \
    Graphics/Animation.cpp \
    Graphics/BloomEffect.cpp \
    Graphics/EmitterNode.cpp \
    Graphics/ParticleNode.cpp \
    Graphics/PostEffect.cpp \
    Graphics/RectangleShapeNode.cpp \
    Graphics/SceneNode.cpp \
    Graphics/SpriteNode.cpp \
    Graphics/TextNode.cpp \
    Graphics/VertexArrayNode.cpp \
    Gui/Button.cpp \
    Gui/Component.cpp \
    Gui/Container.cpp \
    Gui/Label.cpp \
    Gui/MessageBoxNode.cpp \
    Gui/ResponseSelector.cpp \
    States/CutSceneState.cpp \
    States/GameState.cpp \
    States/LoadingState.cpp \
    States/MenuState.cpp \
    States/MessageBoxState.cpp \
    States/MultiplayerGameState.cpp \
    States/PauseState.cpp \
    States/SettingsState.cpp \
    States/SplashScreen.cpp \
    States/TitleState.cpp \
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
    main.cpp
