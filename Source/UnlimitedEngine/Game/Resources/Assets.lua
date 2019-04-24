-- If you are using a Tiled map, all resources needed to load the map must
-- be defined here.
return {
    Textures = {
        DivittoUnlimitedLogo = 0;
        Default              = 1;
        Particle             = 2;
        Buttons              = 3;
        -- Demo
        Hat                  = 4;
        Bullet               = 5;
        Dirt                 = 6;
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
        TileLayer1          = 0;
        ObjectLayer         = 1;
    };

-- maybe usefull in World::buildScene dont delete yet
    Objects = {
        Wall                = 0;
        Trigger             = 1;
        Actor               = 2;
        Item                = 3;
    };

    Shaders = {
        BrightnessPass      = 0;
        DownSamplePass      = 1;
        GaussianBlurPass    = 2;
        addPass             = 3;
    };
};
