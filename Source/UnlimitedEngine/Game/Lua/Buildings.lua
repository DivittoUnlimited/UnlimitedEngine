
-- All The buildings that can be captured for bonuses in the game.

return {
    Map = {
        SpawnPoint  = 0;
--        Hospital    = 1;
--        Temple      = 2;
--        Village     = 3;
    };

    SpawnPoint = {
            type        = "SpawnPoint";
            goldPerTurn = 100;
            ownedBy     = "NONE"; -- "RED", "BLUE"
            texture     = "SpawnPoint";
    };
--    Hospital = {
--        type        = "Hospital";
--        goldPerTurn = 100;
--        texture     = "Hospital";
--    };
--    Temple = {
--        type        = "Temple";
--        goldPerTurn = 100;
--        texture     = "Temple";
--    };
--    Village = {
--        type        = "Village";
--        goldPerTurn = 100;
--        texture     = "Village";
--    };
};
