-- If you are using a Tiled map, all resources needed to load the map must
-- be defined here.
return {
    Textures = {
        DivittoUnlimitedLogo = 0;
        Default              = 1;
        Buttons              = 2;
        -- Demo
        Tiles                = 3;
        BackgroundImage      = 4;
        ExteriorTiles        = 5;
        Trainer              = 6;
    };

    SoundEffects = {
        -- Explosion           = 0;
    };

    Music = {
        --MenuTheme           = 0;
        --GameTheme           = 1;
        --FuelLow             = 2;
    };

    Fonts = {
        Default             = 0;
    };

    Layers = {
        BottomTileLayer     = 0;
        ObjectLayer         = 1;
    };

    Objects = {
        Wall                = 0;
        Trigger             = 1;
        Actor               = 2;
        Item                = 3;
    };

    Shaders = {
        BrightnessPass      = 0;
        DownSamplePass      = 1;
        GaussianBlurPass    = 3;
        addPass             = 4;
    };
};
