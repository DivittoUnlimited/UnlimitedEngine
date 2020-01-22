-- If you are using a Tiled map, all resources needed to load the map must
-- be defined here.

-- This is a registration list to define all assets to the engine so that it knows
-- what assets it has available and how to call them.

-- This file closely corresponds to the SourceFileMap.lua file for loading
-- all the assets listed here into memory.

return {
    Textures = {
        -- required by engine
        DivittoUnlimitedLogo = 0;
        Default              = 1;
        Particle             = 2;
        Buttons              = 3;

        -- Game specific stuff
        DemoGirl                 = 4;
        DemoLevel                = 5;
        LightInfantry            = 6;
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

    -- it is very important that the layer names here match the layer names in the tiled files/editor
    Layers = {
        TileLayer1          = 0;
        TileLayer2          = 1;
        ObjectLayer         = 2;
        TileLayer3          = 3;
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
        GaussianBlurPass    = 3;
        addPass             = 4;
    };

    Maps = {
        DemoMap = 0;
    };
};
