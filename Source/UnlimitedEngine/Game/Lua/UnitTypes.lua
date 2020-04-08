
-- This is the definition file for all unit types found in the game as well as their data

-- The current Units and their values are stub examples used for engine testing change them as needed and then erase this
-- message :)

return {
    Map = {
        Fighter       = 0;
        Templar       = 1;
        Archer        = 2;
        Medic         = 3;
        Wizard        = 4;
        Rogue         = 5;
        Scout         = 6;
        Bard          = 7;
    };

    Fighter = {
        class           = "Fighter";
        strength        = 8;
        dexterity       = 3;
        constitution    = 100;
        defense         = 3;
        range           = { innerRange = 0; outerRange = 2; }; -- Can attack adjacent squares only.
        perception      = 3;
        texture         = "UnitSpriteSheet";
        textureRect     = { top = 0; left = 0; width = 64; height  = 64; };
    };

    Templar = {
        class           = "Templar";
        strength        = 8;
        dexterity       = 2;
        constitution    = 100;
        defense         = 8;
        range           = { innerRange = 0; outerRange = 2; }; -- Can attack adjacent squares only.
        perception      = 3;
        texture         = "UnitSpriteSheet";
        textureRect     = { top = 0; left = 128; width = 64; height  = 64; };
    };

    Archer = {
        class           = "Archer";
        strength        = 8;
        dexterity       = 4;
        constitution    = 100;
        defense         = 2;
        range           = { innerRange = 0; outerRange = 2; }; -- Can attack adjacent squares only.
        perception      = 3;
        texture         = "UnitSpriteSheet";
        textureRect     = { top = 0; left = 64; width = 64; height  = 64; };
    };

    Medic = {
        class           = "Medic";
        strength        = 8;
        dexterity       = 3;
        constitution    = 100;
        defense         = 3;
        range           = { innerRange = 0; outerRange = 2; }; -- Can attack adjacent squares only.
        perception      = 3;
        texture         = "UnitSpriteSheet";
        textureRect     = { top = 0; left = 192; width = 64; height  = 64; };
    };

    Wizard = {
        class           = "Wizard";
        strength        = 8;
        dexterity       = 2;
        constitution    = 100;
        defense         = 3;
        range           = { innerRange = 0; outerRange = 2; }; -- Can attack adjacent squares only.
        perception      = 3;
        texture         = "UnitSpriteSheet";
        textureRect     = { top = 0; left = 256; width = 64; height  = 64; };
    };

    Rogue = {
        class           = "Rogue";
        strength        = 8;
        dexterity       = 4;
        constitution    = 100;
        defense         = 3;
        range           = { innerRange = 0; outerRange = 2; }; -- Can attack adjacent squares only.
        perception      = 3;
        texture         = "UnitSpriteSheet";
        textureRect     = { top = 0; left = 320; width = 64; height  = 64; };
    };

    Scout = {
        class           = "Scout";
        strength        = 8;
        dexterity       = 5;
        constitution    = 100;
        defense         = 3;
        range           = { innerRange = 0; outerRange = 2; }; -- Can attack adjacent squares only.
        perception      = 3;
        texture         = "UnitSpriteSheet";
        textureRect     = { top = 0; left = 384; width = 64; height  = 64; };
    };

    Bard = {
        class           = "Bard";
        strength        = 8;
        dexterity       = 3;
        constitution    = 100;
        defense         = 3;
        range           = { innerRange = 0; outerRange = 2; }; -- Can attack adjacent squares only.
        perception      = 3;
        texture         = "UnitSpriteSheet";
        textureRect     = { top = 0; left = 448; width = 64; height  = 64; };
    };

};
