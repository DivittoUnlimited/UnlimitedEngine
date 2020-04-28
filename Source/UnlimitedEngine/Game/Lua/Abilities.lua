return {
    Map = {
        -- Wait  = 0;
        Slash = 0;
        Stab  = 1;
        Parry = 2;
    };

    Slash =
    {
        coolDown        = 2; -- in turns
        range           = 0; -- in squares, signals that the ability orginates from user w/ no target
        hasRotation     = true;
        AOE = {
            north = { { -1, -1 }, {  0, -1 }, {  1, -1 } };
            east  = { {  1, -1 }, {  1,  0 }, {  1,  1 } };
            south = { { -1,  1 }, {  0,  1 }, {  1,  1 } };
            west  = { { -1, -1 }, { -1,  0 }, { -1,  1 } };
        };
        targetMods =
        {
            { stat = "health"; power = "4-16"; duration = 1; },
            { stat = "armour"; power = "1-2"; duration = 1; }
        };
        userMods =
        {
            { stat = "morale";  power = "1+4"; duration = 1; },
            { stat = "stamina"; power = "1-4"; duration = 1; }
        };
    };
    Stab = {
        coolDown    = 2; -- in turns
        range       = 2; -- in squares
        hasRotation = false;
        AOE = { { 0, 0 } };
        targetMods =
        {
            { stat = "health"; power = "36-52"; duration = 1; },
            { stat = "morale"; power = "30-45"; duration = 1; },
            { stat = "armour"; power = "2-4";   duration = 1; }
        };
        userMods =
        {
            { stat = "morale";  power = "10+20"; duration = 1; },
            { stat = "stamina"; power = "10-20"; duration = 1; }
        };
    };
    Parry = {
        coolDown    = 2; -- in turns
        range       = 1;
        hasRotation = false;
        AOE = { { 0, 0 } };
        targetMods  =
        {
            { stat = "health"; power = "4-16"; duration = 1; },
            { stat = "armour"; power = "4-16"; duration = 1; }
        };
        userMods =
        {
            { stat = "morale";  power = "1+4"; duration = 1; },
            { stat = "stamina"; power = "1-4"; duration = 1; }
        };
    };
};
