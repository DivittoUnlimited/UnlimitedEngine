-- if you want to use an asset ( image file, sound effect etc.. ) then it better be registered in the
-- Assets.lua file and then defined with it's file path here.



local Assets = require( "Game/Lua/Resources/Assets" );

local Tex   = Assets.Textures;
local Font  = Assets.Fonts;
local Sound = Assets.SoundEffects;
local Music = Assets.Music;
local Map   = Assets.Maps;

return {
    Textures = {
        [Tex.DivittoUnlimitedLogo]      = "Media/Textures/DivittoUnlimitedLogo1024X768.png";
        [Tex.Default]                   = "Media/Textures/Default.png";
        [Tex.Particle]                  = "Media/Textures/Particle.png";
        [Tex.Buttons]                   = "Media/Textures/Buttons.png";

        -- Game Specific
        [Tex.TexturePack]               = "Media/Textures/TexturePack.png";
        [Tex.PlayerAvatar]              = "Media/Textures/PlayerAvatar.png";
    };
    Fonts = {
        [Font.Default]              = "Media/Sansation.ttf";
    };
    SoundEffects = {
    };
    Music = {
    };
    Maps =
    {
        [Map.TacticsTribeDemoLevel] = "Game/Lua/Resources/Maps/TacticsTribeDemoLevel.lua";
        [Map.Courtyard]             = "Game/Lua/Resources/Maps/Courtyard.lua";
        [Map.SmallMap]              = "Game/Lua/Resources/Maps/smallMap.lua";
        [Map.GeneratedGrassyPlains] = "Game/Lua/Resources/Maps/GeneratedGrassyPlains.lua";

    };
}
