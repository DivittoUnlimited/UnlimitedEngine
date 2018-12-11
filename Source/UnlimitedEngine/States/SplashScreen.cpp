#include "SplashScreen.hpp"

#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

SplashScreen::SplashScreen( States::ID id, StateStack& stack, Context context )
    : State( id, stack, context )
    , mLogo( )
    , mElapsedTime( sf::Time::Zero )
{
    mContext.textures->load( TextureMap.at( "DivittoUnlimitedLogo" ), MediaFileMap.at( "Textures" ).at( TextureMap.at( "DivittoUnlimitedLogo" ) ) );

    mLogo.setTexture( mContext.textures->get( TextureMap.at( "DivittoUnlimitedLogo" ) )  );
    mLogo.setPosition( 0, 0 );
}

SplashScreen::~SplashScreen( )
{
    mContext.textures->remove( TextureMap.at( "DivittoUnlimitedLogo" ) );
}

void SplashScreen::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView( window.getDefaultView( ) );
    window.draw( mLogo );
}

bool SplashScreen::update( sf::Time dt )
{
    // Show state for 3 seconds, after return to menu
    mElapsedTime += dt;
    if( mElapsedTime > sf::seconds( 3 ) )
    {
        requestStateClear( );
        requestStackPush( States::Title );
    }
    return false;
}

bool SplashScreen::handleEvent( const sf::Event& )
{
    return false;
}
