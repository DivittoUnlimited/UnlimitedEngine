
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
        strength        = 40;
        dexterity       = 50;
        constitution    = 100;
        defense         = 50;
    };

    HeavyInfantry = {
        strength        = 60;
        dexterity       = 30;
        constitution    = 100;
        defense         = 70;
    };

    Phalanx = {
        strength        = 100;
        dexterity       = 40;
        constitution    = 100;
        defense         = 100;
    };

};
