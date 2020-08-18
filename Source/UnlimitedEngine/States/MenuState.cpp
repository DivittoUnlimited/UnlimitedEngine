#include "MenuState.hpp"
#include "Gui/Button.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Game/DataTables.hpp"
#include "Core/MusicPlayer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include "Core/Globals.hpp"
#include "Core/Utility.hpp"

#include <fstream>

MenuState::MenuState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mGUIContainer( )
, mTimeOut( sf::Time::Zero )
{
    // mContext.music->setVolume( 30 );
    // mContext.music->play( MusicMap.at( "MenuTheme" ) );

    // get dev notes
    std::ifstream stream;
    std::string devNotes = "";
    std::string line = "";
    std::ifstream myfile( "Notes.txt" );
    if( myfile.is_open( ) ) {
        while( getline( myfile, line ) ) {
             devNotes += line;
             devNotes += "\n";
        }
        myfile.close( );
    } else std::cout << "Unable to open to-do.txt" << std::endl;

    mDevNotes = sf::Text( devNotes, mContext.fonts->get( FontMap.at( "Default" ) ), 20 );
    mDevNotes.setFillColor( sf::Color( 255, 255, 255, 255 ) );
    mDevNotes.setPosition( 10, 600 );
    mDevNotes.setOutlineThickness( 1 );
    mDevNotes.setOutlineColor( sf::Color( 0, 255, 0, 255 ) );

    mTitleText = sf::Text( "EMPTY TITLE", mContext.fonts->get( FontMap.at( "Default" ) ), 100 );
    mTitleText.setFillColor( sf::Color( 181, 182, 228, 255 ) );
    centerOrigin( mTitleText );
    mTitleText.setPosition( WINDOW_WIDTH / 2, 200 );
    mTitleText.setOutlineColor( sf::Color( 79, 67, 174, 255 ) );
    mTitleText.setOutlineThickness( 5 );
    mTitleText.setStyle( sf::Text::Bold );


    auto playButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    playButton->setPosition( WINDOW_WIDTH / 2 - 100, 390 );
    playButton->setText( "Local Game" );
    playButton->setCallback( [this] ( )
	{
        GAME_MODE = GameModes::QuickBattle;
        requestStackPop( );
        requestStackPush( States::Loading );
	});

    auto playLocalButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    playLocalButton->setPosition( WINDOW_WIDTH / 2 - 100, 450 );
    playLocalButton->setText( "Create Server" );
    playLocalButton->setCallback( [this] ( )
    {
        GAME_MODE = GameModes::QuickBattle;
        //requestStackPop( );
        //requestStackPush( States::HostGame );
    });

    auto playMultiplayerButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    playMultiplayerButton->setPosition( WINDOW_WIDTH / 2 - 100, 510 );
    playMultiplayerButton->setText( "Join Server" );
    playMultiplayerButton->setCallback( [this] ( )
    {
        GAME_MODE = GameModes::Online;
        // requestStackPop( );
        //requestStackPush( States::JoinGame ); // used to be hostGame not sure why
    });

    auto settingsButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    settingsButton->setPosition( WINDOW_WIDTH / 2 - 100, 570 );
    settingsButton->setText( "Settings" );
    settingsButton->setCallback([this] ( )
	{
        GAME_MODE = GameModes::NONE;
        requestStackPush( States::SettingsState );
	});

    auto exitButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exitButton->setPosition( WINDOW_WIDTH / 2 - 100, 630 );
    exitButton->setText( "Exit" );
    exitButton->setCallback([this] ( )
	{
        mContext.music->stop( );
        requestStateClear( );
	});

    mGUIContainer.pack( playButton );
    mGUIContainer.pack( playLocalButton );
    mGUIContainer.pack( playMultiplayerButton );
    mGUIContainer.pack( settingsButton );
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
    window.clear( sf::Color::Black );
    window.draw( mTitleText );
    window.draw( mGUIContainer );
    window.draw( mDevNotes );
}

bool MenuState::update( sf::Time dt )
{   
    mDevNotes.move( 0, -28 * dt.asSeconds( ) );
    mTimeOut += dt;
    if( mTimeOut > sf::seconds( 60 ) )
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
