#ifndef STATE_HPP
#define STATE_HPP

#include "StateIdentifiers.hpp"
#include "ResourceManager.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

// Forward decs
namespace sf
{
	class RenderWindow;
}
class StateStack;
//class Player;
class MusicPlayer;
class SoundPlayer;
class KeyBinding;

class State
{
public:
    typedef std::unique_ptr<State> Ptr;
    struct Context
    {
        Context( )
        {

        }

        Context( States::ID stateID, sf::RenderWindow& window, TextureManager& textures, FontManager& fonts, MusicPlayer& music, SoundPlayer& sounds,
                 std::vector<std::pair<std::string, int>>* highScores, KeyBinding& keys1, KeyBinding& keys2 )
        : stateID( stateID )
        , window( &window )
        , textures( &textures )
        , fonts( &fonts )
        , music( &music )
        , sounds( &sounds )
        //, player( nullptr )
        , highScores( highScores )
        , keys1( &keys1 )
        , keys2( &keys2 )
        {
        }

        States::ID        stateID;
        sf::RenderWindow* window;
        TextureManager*	  textures;
        FontManager*	  fonts;
        MusicPlayer*      music;
        SoundPlayer*      sounds;
        //Player*			  player;
        std::string       tiledMapFilePath;
        std::vector<std::pair<std::string, int>>* highScores;
        KeyBinding*			keys1;
        KeyBinding*			keys2;
    };

    State( States::ID stateID, StateStack& stack, Context context );
    virtual	~State( );

    //## Interface
    virtual void draw( ) = 0;
    virtual bool update( sf::Time dt ) = 0;
    virtual bool handleEvent( const sf::Event& event ) = 0;
protected:
    void requestStackPush( States::ID stateID );
    void requestStackPop( );
    void requestStateClear( );
    Context getContext( ) const;
    States::ID getStateID( ) const;

    //## Attributes
    StateStack*			mStack;
    Context				mContext;
    States::ID          mStateID;
};

#endif // STATE_HPP
