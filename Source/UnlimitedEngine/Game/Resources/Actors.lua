
return {
    Map = {
        Red     = 0;
        Bob     = 1;
        Mary    = 2;
    };

    Red = {
        walkUp = "RedWalkUp"; -- refers to animations for movement not sure if this is active or not...
        path = { };
    };

    Bob = {
        path = {
                    {   0.0,  40.0 }
                  , {    90,  10.0 }
                  , { 180.0, 80.00 }
                  , {   270,  10.0 }
                  , {   0.0,  40.0 }
        };
    };

    Mary = {
        path = {
              {   0.0, 50.0 }
            , { 180.0, 50.0 }
        };
    };
};
