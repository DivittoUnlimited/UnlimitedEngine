return {
    Map = {
        -- Wait  = 0;
        Slash     = 0; -- User-origin non-symetrical AOE
        SpinSlash = 1; -- User-origin symetrical AOE
        Stab      = 2; -- basic attack
        Parry     = 3; -- basic attack
        FireBlast = 4; -- ranged symetrical AOE
    };

    Slash =
    {
        coolDown        = 2; -- in turns
        range           = 0; -- in squares, signals that the ability orginates from user w/ no target
        hasRotation     = true;
        AOE = {
            north = { { x = -1, y = -1 }, { x =  0, y = -1 }, { x =  1, y = -1 } };
            east  = { { x =  1, y = -1 }, { x =  1, y =  0 }, { x =  1, y =  1 } };
            south = { { x = -1, y =  1 }, { x =  0, y =  1 }, { x =  1, y =  1 } };
            west  = { { x = -1, y = -1 }, { x = -1, y =  0 }, { x = -1, y =  1 } };
        };
        targetMods =
        {
            { stat = "health"; power = "15-25"; duration = 1; },
            { stat = "armour"; power = "1-2"; duration = 1; }
        };
        userMods =
        {
            { stat = "morale";  power = "1+4"; duration = 1; },
            { stat = "stamina"; power = "20-25"; duration = 1; }
        };
    };
    SpinSlash = {
        coolDown    = 2; -- in turns
        range       = 0; -- in squares
        hasRotation = false;
        AOE = { only =  {
                            { x =  0, y = -1 }, { x =  1, y = -1 }, { x =  1, y = 0  },
                            { x =  1, y =  1 }, { x =  0, y =  1 }, { x = -1, y = 1 },
                            { x = -1, y =  0 }, { x = -1, y = -1 }
                        } };
        targetMods =
        {
            { stat = "health"; power = "80-100"; duration = 1; },
            { stat = "morale"; power = "30-45"; duration = 1; },
            { stat = "armour"; power = "2-4";   duration = 1; }
        };
        userMods =
        {
            { stat = "morale";  power = "10+20"; duration = 1; },
            { stat = "stamina"; power = "10-20"; duration = 1; }
        };
    };
    Stab = {
        coolDown    = 2; -- in turns
        range       = 1; -- in squares
        hasRotation = false;
        AOE = { only = { { x = 0, y = 0 } } };
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
        AOE = { only = { { x = 0, y = 0 } } };
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
    FireBlast = {
        coolDown    = 2; -- in turns
        range       = 5; -- in squares
        hasRotation = false;
        AOE = { only =  {
                            { x =  0, y = -1 }, { x =  1, y = -1 }, { x =  1, y = 0 },
                            { x =  1, y =  1 }, { x =  0, y =  1 }, { x = -1, y = 1 },
                            { x = -1, y =  0 }, { x = -1, y = -1 }, { x =  0, y = 0 }
                        } };
        targetMods =
        {
            { stat = "health"; power = "24-52"; duration = 3; },
            { stat = "morale"; power = "45-65"; duration = 1; },
            { stat = "armour"; power = "2-4";   duration = 1; }
        };
        userMods =
        {
            { stat = "morale";  power = "10+20"; duration = 1; },
            { stat = "stamina"; power = "10-20"; duration = 1; }
        };
    };

};
