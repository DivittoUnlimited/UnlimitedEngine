return {
    Map = {
        standing        = 0;
        walkRight       = 1;
        --walkLeft        = 2;
        -- drawRightGun    = 3;
        -- drawLeftGun     = 4;
        -- jump            = 5;
        -- tipHat          = 6;
    };
    standing = {
        {
            duration    = 500; -- millisecs
            leftArm     = { x = -2; y =  8; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightArm    = { x = -2; y =  8; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            leftLeg     = { x = -2; y = 20; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightLeg    = { x = -2; y = 20; radius = 3.5; color = {   r = 0; g = 0; b = 255; a = 255; }; };
            body        = { x =  0; y =  0; };
        }, -- Frame 1
        {
            duration    = 500; -- millisecs
            leftArm     = { x = -2; y =  7; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightArm    = { x = -2; y =  7; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            leftLeg     = { x = -2; y = 20; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightLeg    = { x = -2; y = 20; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            body        = { x =  0; y =  0; };
        }, -- Frame 2
        {
            duration    = 500; -- millisecs
            leftArm     = { x = -2; y =  5; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightArm    = { x = -2; y =  5; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            leftLeg     = { x = -2; y = 20; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightLeg    = { x = -2; y = 20; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            body        = { x =  0; y =  0; };
        }, -- Frame 3
        {
            duration    = 500; -- millisecs
            leftArm     = { x = -2; y =  6; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightArm    = { x = -2; y =  8; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            leftLeg     = { x = -2; y = 20; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightLeg    = { x = -2; y = 20; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            body        = { x =  0; y =  0; };
        }, -- Frame 4
        {
            duration    = 500; -- millisecs
            leftArm     = { x = -2; y =  7; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightArm    = { x = -2; y =  8; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            leftLeg     = { x = -2; y = 20; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightLeg    = { x = -2; y = 20; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            body        = { x =  0; y =  0; };
        }  -- Frame 5
    };
    walkRight = {
        {
            duration    = 300; -- millisecs
            leftArm     = { x =   5; y =  7; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightArm    = { x =  -9; y =  7; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            leftLeg     = { x =  -9; y = 16; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightLeg    = { x =   5; y = 20; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            body        = { x =   0; y =  0; };
        }, -- Frame 1
        {
            duration    = 300; -- millisecs
            leftArm     = { x =  3; y =  8; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightArm    = { x = -7; y =  8; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            leftLeg     = { x = -7; y = 16; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightLeg    = { x =  3; y = 20; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            body        = { x =  0; y =  0; };
        }, -- Frame 2
        {
            duration    = 300; -- millisecs
            leftArm     = { x = -2; y =  9; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightArm    = { x = -2; y =  9; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            leftLeg     = { x = -2; y = 16; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightLeg    = { x = -2; y = 20; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            body        = { x =  0; y =  0; };
        }, -- Frame 3
        {
            duration    = 300; -- millisecs
            leftArm     = { x = -7; y =  8; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightArm    = { x =  3; y =  8; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            leftLeg     = { x =  3; y = 16; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightLeg    = { x = -7; y = 20; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            body        = { x =  0; y =  0; };
        }, -- Frame 4
        {
            duration    = 300; -- millisecs
            leftArm     = { x = -9; y =  7; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightArm    = { x =  5; y =  7; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            leftLeg     = { x =  5; y = 20; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightLeg    = { x = -9; y = 16; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            body        = { x =  0; y = 0; };
        }, -- Frame 5
        {
            duration    = 300; -- millisecs
            leftArm     = { x = -7; y =  8; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightArm    = { x =  3; y =  8; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            leftLeg     = { x =  3; y = 20; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightLeg    = { x = -7; y = 16; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            body        = { x =  0; y =  0; };
        }, -- Frame 6
        {
            duration    = 300; -- millisecs
            leftArm     = { x = -2; y =  9; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightArm    = { x = -2; y =  9; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            leftLeg     = { x = -2; y = 20; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightLeg    = { x = -2; y = 16; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            body        = { x =  0; y =  0; };
        }, -- Frame 7
        {
            duration    = 300; -- millisecs
            leftArm     = { x =  3; y =  8; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightArm    = { x = -7; y =  8; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            leftLeg     = { x = -7; y = 20; radius = 3.5; color = { r = 255; g = 0; b =   0; a = 255; }; };
            rightLeg    = { x =  3; y = 16; radius = 3.5; color = { r =   0; g = 0; b = 255; a = 255; }; };
            body        = { x =  0; y =  0; };
        } -- Frame 8
    };
};
