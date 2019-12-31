local Assets = require( "Game/Resources/Assets" );

local Tex   = Assets.Textures;
local Font  = Assets.Fonts;
local Sound = Assets.SoundEffects;
local Music = Assets.Music;
local Level = Assets.Levels;

return {
    Textures = {
        [Tex.DivittoUnlimitedLogo]      = "Media/Textures/DivittoUnlimitedLogo1024X768.png";
        [Tex.Default]                   = "Media/Textures/Default.png";
        [Tex.Particle]                  = "Media/Textures/Particle.png";
        [Tex.Buttons]                   = "Media/Textures/Buttons.png";

        -- Demo
        [Tex.tiles]                     = "Media/Textures/tiles.png";
        [Tex.trainers]                  = "Media/Textures/trainers.png";
        [Tex.cowboy]                    = "Media/Textures/cowboy.png";
        [Tex.woodTexture]               = "Media/Textures/woodTexture.png";
        [Tex.SevenSunSetDemoTexture]    = "Media/Textures/SevenSunSetDemoTexture.png";
        [Tex.DemoGirl]                  = "Media/Textures/Girl.png";
        [Tex.DemoLevel]                 = "Media/Textures/DemoLevel.png";
    };
    Fonts = {
        [Font.Default]              = "Media/Sansation.ttf";
    };
    SoundEffects = {
    };
    Music = {
    };
};
