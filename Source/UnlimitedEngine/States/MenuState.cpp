#include "MenuState.hpp"
#include "Gui/Button.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"
#include "Core/MusicPlayer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include "Core/Globals.hpp"
#include "Core/Utility.hpp"

MenuState::MenuState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mGUIContainer( )
, mTimeOut( sf::Time::Zero )
{

    //mContext.music->setVolume( 30 );
    // mContext.music->play( MusicMap.at( "MenuTheme" ) );

    mTitleText = sf::Text( "MAIN MENU", mContext.fonts->get( FontMap.at( "Default" ) ), 100 );
    mTitleText.setFillColor( sf::Color( 181, 182, 228, 255 ) );
    centerOrigin( mTitleText );
    mTitleText.setPosition( WINDOW_WIDTH / 2, 200 );
    mTitleText.setOutlineColor( sf::Color( 79, 67, 174, 255 ) );
    mTitleText.setOutlineThickness( 5 );
    mTitleText.setStyle( sf::Text::Bold );

    auto playButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    playButton->setPosition( WINDOW_WIDTH / 2 - 100, 450 );
    playButton->setText( "Play" );
    playButton->setCallback( [this] ( )
	{
        requestStackPop( );
        requestStackPush( States::Loading );
	});

    /*
     * Removed for debugging
    auto settingsButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    settingsButton->setPosition( WINDOW_WIDTH / 2 - 100, 525 );
    settingsButton->setText( "Settings" );
    settingsButton->setCallback([this] ( )
	{
        requestStackPush( States::Settings );
	});
    */
    auto exitButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exitButton->setPosition( WINDOW_WIDTH / 2 - 100, 600 );
    exitButton->setText( "Exit" );
    exitButton->setCallback([this] ( )
	{
        mContext.music->stop( );
        requestStateClear( );
	});

    mGUIContainer.pack( playButton );
    //mGUIContainer.pack( settingsButton );
    mGUIContainer.pack( exitButton );

}

MenuState::~MenuState()
{
   // mContext.music->stop( );
}

void MenuState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;

    window.setView( window.getDefaultView( ) );

    window.draw( mTitleText );
    window.draw( mGUIContainer );
}

bool MenuState::update( sf::Time dt )
{   
    mTimeOut += dt;
    if( mTimeOut > sf::seconds( 30 ) )
    {
        requestStateClear( );
        requestStackPush( States::SplashScreen );
    }
	return true;
}

bool MenuState::handleEvent( const sf::Event& event )
{
    mGUIContainer.handleEvent( event );
	return false;
}
