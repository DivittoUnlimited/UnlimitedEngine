#ifndef PARALLELTASK_HPP
#define PARALLELTASK_HPP

#include "State.hpp"
#include "DataTables.hpp"
#include "World.hpp"
#include "ResourceIdentifiers.hpp"
#include "Core/SoundPlayer.hpp"

#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Clock.hpp>

extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}


class ParallelTask
{
public:
    ParallelTask();
    //## Interface
    void  execute( State::Context context, std::string fileName );
    bool  isFinished();
    float getCompletion();

private:
    //## Attributes
    sf::Thread                 mThread;
    bool                       mFinished;
    sf::Mutex                  mMutex;
    float                      mCompletion;
    std::string                mFileName;
    State::Context             mContext;

    //## Behaviors
    void runTask();

    static int luaLoadTexture( lua_State* L );
};

#endif // PARALLELTASK_HPP
