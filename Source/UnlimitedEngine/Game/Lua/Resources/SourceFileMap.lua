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
        [Tex.DemoGirl]                  = "Media/Textures/Girl.png";
        [Tex.DemoLevel]                 = "Media/Textures/DemoLevel.png";
        [Tex.LightInfantry]             = "Media/Textures/LightInfantry.png";
        [Tex.SpawnPoint]                = "Media/Textures/SpawnPoint.png";
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
        [Map.DemoMap] = "Game/Lua/Resources/Maps/TacticsTribeDemoLevel.lua";
    };
}