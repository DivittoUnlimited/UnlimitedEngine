local Assets = require( "Game/Resources/Assets" );

local Tex   = Assets.Textures;
local Font  = Assets.Fonts;
local Sound = Assets.SoundEffects;
local Music = Assets.Music;

return {
    Textures = {
        [Tex.DivittoUnlimitedLogo]  = "Media/Textures/DivittoUnlimitedLogo1024X768.png" ;
        [Tex.Buttons]                = "Media/Textures/Buttons.png"
    };
    Fonts = {
        [Font.Default]              = "Media/Sansation.ttf";
    };
    SoundEffects = {
    };
    Music = {
        --[Music.MenuTheme]           = "Media/Music/MenuTheme.ogg";
        --[Music.GameTheme]           = "Media/Music/MissionTheme.ogg";
        --[Music.FuelLow]             = "Media/Music/FuelLow.ogg";
    };
};
