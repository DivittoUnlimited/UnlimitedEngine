-- this is a lookup chart that the levels use to determine where a unit will be able to move
-- based on their speed and their unit types ability to cross x terrain type

-- Keep this chart up to date with the array it represents below to make your life a lot easier.
-- the unit types and terrain types are in the same order as they are found in their respective lua files TerrainTypes.lua and UnitTypes.lua
-- to keep things simple this should be kept this way.

--                  Ground, Road, Mud, Water, Mountain, Forest
-- LightInfantry    [    1,    1,   3,     5,        2,      2  ],
-- HeavyInfantry    [    1,    2,   5,     5,        5,      3  ],
-- Phalanx          [    2,    1,   3,     5,        2,      2  ],
-- Fighter          {    4,    1,   5,     1,        1,      1  },
-- Knight           {    4,    1,   5,     1,        1,      1  },
-- Archer           {    4,    1,   5,     1,        1,      1  },
-- Medic            {    4,    1,   5,     1,        1,      1  },
-- Wizard           {    4,    1,   5,     1,        1,      1  }
-- Rogue
-- Scout
-- Bard

return  {
    { 2, 1, 3, 3, 3, 2 },
    { 2, 1, 3, 3, 3, 2 },
    { 2, 1, 3, 3, 3, 2 },
    { 2, 1, 3, 3, 3, 2 },
    { 2, 1, 3, 3, 3, 2 },
    { 2, 1, 3, 3, 3, 2 },
    { 2, 1, 3, 3, 3, 2 },
    { 2, 1, 3, 3, 3, 2 },
    { 2, 1, 3, 3, 3, 2 },
    { 2, 1, 3, 3, 3, 2 },
    { 2, 1, 3, 3, 3, 2 }
};
