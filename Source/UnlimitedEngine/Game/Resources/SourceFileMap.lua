local Assets = require( "Game/Resources/Assets" );

local Tex   = Assets.Textures;
local Font  = Assets.Fonts;
local Sound = Assets.SoundEffects;
local Music = Assets.Music;
local Level = Assets.Levels;

return {
    Textures = {
        [Tex.DivittoUnlimitedLogo]  = "Media/Textures/DivittoUnlimitedLogo1024X768.png";
        [Tex.Default]               = "Media/Textures/Default.png";
        [Tex.Buttons]               = "Media/Textures/Buttons.png";

        -- Demo
        [Tex.tiles]                 = "Media/Textures/tiles.png";
        [Tex.trainers]               = "Media/Textures/trainers.png";
    };
    Fonts = {
        [Font.Default]              = "Media/Sansation.ttf";
    };
    SoundEffects = {
    };
    Music = {
    };
};
