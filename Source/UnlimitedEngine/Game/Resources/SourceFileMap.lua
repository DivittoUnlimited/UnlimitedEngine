local Assets = require( "Game/Resources/Assets" );

local Tex   = Assets.Textures;
local Font  = Assets.Fonts;
local Sound = Assets.SoundEffects;
local Music = Assets.Music;
-- local Level = Assets.Levels;

return {
    Textures = {
        [Tex.DivittoUnlimitedLogo]  = "Media/Textures/DivittoUnlimitedLogo1024X768.png";
        [Tex.Default]               = "Media/Textures/Default.png";
        [Tex.Particle]              = "Media/Textures/Particle.png";
        [Tex.Buttons]               = "Media/Textures/Buttons.png";
         -- Demo
        [Tex.Hat]                   = "Media/Textures/Hat.png";
        [Tex.Dirt]                  = "Media/Textures/Dirt.png";
        [Tex.Bullet]                = "Media/Textures/Bullet.png";
    };
    Fonts = {
        [Font.Default]              = "Media/Sansation.ttf";
    };
    SoundEffects = {
    };
    Music = {
    };
};
