
-- This is the definition file for all unit types found in the game as well as their data

-- The current Units and their values are stub examples used for engine testing change them as needed and then erase this
-- message :)

local abilities = require( "Game/Lua/Abilities" ).Map;

return {
    Map = {
        Fighter       = 0;
       -- Templar       = 1;
       -- Archer        = 2;
       -- Medic         = 3;
       -- Wizard        = 4;
       -- Rogue         = 5;
       -- Scout         = 6;
       -- Bard          = 7;
    };

    Fighter = {
        class           = "Fighter";
        attack          = 8;
        armour          = 4;
        speed           = 3;
        perception      = 3;
        health          = 100;
        morale          = 100;
        stamina         = 100;
        texture         = "UnitSpriteSheet";
        textureRect     = { top = 0; left = 0; width = 64; height  = 64; };
        Abilities       =
        {
            "Stab",
            "Slash",
            "Parry"
        };
    };

--    Templar = {
--        class           = "Templar";
--        attack          = 8;
--        dexterity       = 2;
--        constitution    = 100;
--        defense         = 8;
--        perception      = 3;
--        texture         = "UnitSpriteSheet";
--        textureRect     = { top = 0; left = 128; width = 64; height  = 64; };
--    };
--
--    Archer = {
--        class           = "Archer";
--        attack          = 8;
--        dexterity       = 4;
--        constitution    = 100;
 --       defense         = 2;
--        perception      = 3;
--        texture         = "UnitSpriteSheet";
--        textureRect     = { top = 0; left = 64; width = 64; height  = 64; };
--    };

--    Medic = {
--        class           = "Medic";
--        attack          = 8;
--        dexterity       = 3;
--        constitution    = 100;
--        defense         = 3;
--        perception      = 3;
--        texture         = "UnitSpriteSheet";
--        textureRect     = { top = 0; left = 192; width = 64; height  = 64; };
--    };

--    Wizard = {
--        class           = "Wizard";
--        attack          = 8;
--        dexterity       = 2;
--        constitution    = 100;
--        defense         = 3;
--        perception      = 3;
--        texture         = "UnitSpriteSheet";
--        textureRect     = { top = 0; left = 256; width = 64; height  = 64; };
--    };

--    Rogue = {
--        class           = "Rogue";
--        attack        = 8;
 --       dexterity       = 4;
--        constitution    = 100;
--        defense         = 3;
--        perception      = 3;
--        texture         = "UnitSpriteSheet";
--        textureRect     = { top = 0; left = 320; width = 64; height  = 64; };
--    };

--    Scout = {
--        class           = "Scout";
--        attack        = 8;
--        dexterity       = 5;
--        constitution    = 100;
--        defense         = 3;
--        perception      = 3;
 --       texture         = "UnitSpriteSheet";
  --      textureRect     = { top = 0; left = 384; width = 64; height  = 64; };
--    };

--    Bard = {
--        class           = "Bard";
--        attack        = 8;
--        dexterity       = 3;
--        constitution    = 100;
--        defense         = 3;
--        perception      = 3;
--        texture         = "UnitSpriteSheet";
 --       textureRect     = { top = 0; left = 448; width = 64; height  = 64; };
--    };

};
