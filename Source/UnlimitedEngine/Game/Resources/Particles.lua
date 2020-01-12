
-- Particle Effects!!!
-- This is left over from building the engine using a shoot'em up style game.
-- the engine does not currently display them, although it could.


return {
    Map = {
          PortalEffect        = 0;
        --MissilePropellant   = 0;
        --AircraftPropellant  = 1;
        --MissileSmoke        = 2;
        --AircraftSmoke       = 3;
        --DamageSmoke         = 4;
    };

    PortalEffect = {
        color          = { 0, 0, 255 }; -- does not need alpha value
        lifetime       = 5000;          -- millisecs
    };
--    MissilePropellant   = {
--        color           = { 255, 150, 0 };
--        lifetime        = 200; -- milliseconds
--    };
--    MissileSmoke        = {
--        color           = { 200, 200, 190 };
--        lifetime        = 1100; -- milliseconds
--    };
--    AircraftPropellant  = {
--        color           = { 130, 0, 255 };
--        lifetime        = 200;
--    };
--    AircraftSmoke       = {
--        color           = { 180, 180, 255 };
--        lifetime        = 500;
--    };
--    DamageSmoke         = {
--        color           = { 50, 50, 50 };
--        lifetime        = 1000;
--    };
--
};
