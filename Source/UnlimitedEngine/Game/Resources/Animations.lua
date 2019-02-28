local textures = require( "Game/Resources/Assets" ).Textures;

return {
    Map = {
        RedWalkUp = 0;
    };

RedWalkUp = {
    texture         = textures.trainers;
    frameSize       = { 32, 32 };
    numFrames       = 3;
    currentFrame    = 0;
    duration        = 1000; -- millisecs
    isRepeated      = true;
};

--Explosion = {
--    texture         = textures.Explosion;
--    frameSize       = { 256, 256 };
--    numFrames       = 16;
--    currentFrame    = 0; -- probably not needed
--    duration        = 1000; -- in milisecs
--    isRepeated      = false;
--};

--Example = {
--    texture         = textures.Explosion;
--    frameSize       = { 256, 256 };
--    numFrames       = 16;
--    currentFrame    = 0; -- probably not needed
--    duration        = 1000; -- in milisecs
--    isRepeated      = false;
--};

};
