local unit = require( "UnitTypes" );

return {
    Map = {
        TestBot     = 0;
        -- Territorial = 1;
        -- Conqueror   = 2;
    };
    TestBot = {
        [unit.LightInfantry] = {
            DefensiveInfluence = 0.5;
            OffensiveInfluence = 0.75;
            ScoutingInfluence  = 0.9;
        },
        [unit.HeavyInfantry] = {
            DefensiveInfluence = 0.5;
            OffensiveInfluence = 0.75;
            ScoutingInfluence  = 0.9;
        },
        [unit.Phalanx] = {
            DefensiveInfluence = 0.5;
            OffensiveInfluence = 0.75;
            ScoutingInfluence  = 0.9;
        }
    };
};
