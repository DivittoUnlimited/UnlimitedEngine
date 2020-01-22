
-- This is the definition file for all unit types found in the game as well as their data

-- The current Units and their values are stub examples used for engine testing change them as needed and then erase this
-- message :)

return {
    Map = {
        LightInfantry = 0;
        HeavyInfantry = 1;
        Phalanx       = 2;
    };

    LightInfantry = {
        class           = "LightInfantry";
        strength        = 4;
        dexterity       = 10;
        constitution    = 100;
        defense         = 5;
        texture         = "LightInfantry";
    };

    HeavyInfantry = {
        class           = "LightInfantry";
        strength        = 6;
        dexterity       = 6;
        constitution    = 100;
        defense         = 7;
        texture         = "LightInfantry";
    };

    Phalanx = {
        class           = "LightInfantry";
        strength        = 10;
        dexterity       = 8;
        constitution    = 100;
        defense         = 10;
        texture         = "LightInfantry";
    };

};
