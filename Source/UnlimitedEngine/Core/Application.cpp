#include "Application.hpp"
#include "Utility.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"
#include "States/TitleState.hpp"
#include "States/MenuState.hpp"
#include "States/PauseState.hpp"
#include "States/LoadingState.hpp"
#include "States/SettingsState.hpp"
#include "States/MultiplayerGameState.hpp"
#include "States/GameState.hpp"
#include "States/SplashScreen.hpp"
#include "States/MessageBoxState.hpp"
#include "States/CutSceneState.hpp"
#include "Game/DataTables.hpp"
#include "Globals.hpp"


// Debugging only!!
#include <iostream>
#include <exception>

const sf::Time Application::TimePerFrame = sf::seconds( 1.f/ 60.f );

// Defined in Globals.hpp
sf::RenderWindow* mWindow = new sf::RenderWindow( sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "EMPTY TITLE", sf::Style::Default );
GameModes::ID GAME_MODE = GameModes::NONE;
States::ID CURRENT_LEVEL = States::GameState;


Application::Application( )
    : mTextures( )
    , mFonts( )
    , mMusic( )
    , mKeyBinding1( 1 )
    , mKeyBinding2( 2 )
    , mStateStack(State::Context( States::None, *mWindow, mTextures, mFonts, mMusic, mSoundEffects, mKeyBinding1, mKeyBinding2))
    , mStatisticsNumFrames( 0 )
    , mStatisticsText( )
    , mStatisticsUpdateTime( sf::Time::Zero )
{

    mWindow->setKeyRepeatEnabled( false );
    mWindow->setVerticalSyncEnabled( true );

    mFonts.load(    FontMap.at( "Default"         ), MediaFileMap.at( "Fonts"    ).at(    FontMap.at( "Default"  ) ) );
    mTextures.load( TextureMap.at( "Buttons"         ), MediaFileMap.at( "Textures" ).at( TextureMap.at( "Buttons"  ) ) );

    mStatisticsText.setFont( mFonts.get( FontMap.at( "Default" ) ) );
    mStatisticsText.setPosition( 5.f, 5.f );
    mStatisticsText.setCharacterSize( 10u );

    registerStates( );
    mStateStack.pushState( States::Menu );
}

Application::~Application( void )
{
}

void Application::run( )
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while( mWindow->isOpen( ) )
	{
        try{
        sf::Time dt = clock.restart( );
		timeSinceLastUpdate += dt;
        while( timeSinceLastUpdate > TimePerFrame )
		{
			timeSinceLastUpdate -= TimePerFrame;

            processInput( );
            update( TimePerFrame );

			// Check inside this loop, because stack might be empty before update() call
            if( mStateStack.isEmpty( ) )
                mWindow->close( );
		}
        updateStatistics( dt );
        render( );
        }catch( std::exception& e )
        {
            std::cout << e.what() << std::endl;
        }
	}
}

void Application::processInput( )
{
	sf::Event event;
    while( mWindow->pollEvent( event ) )
	{
        mStateStack.handleEvent( event );
        if( event.type == sf::Event::Closed )
            mWindow->close( );
	}
}

void Application::update( sf::Time dt )
{
    mStateStack.update( dt );
}

void Application::render( )
{
    mWindow->clear( sf::Color::Black );
    mStateStack.draw( );

    mWindow->draw( mStatisticsText );
    mWindow->display( );
}

void Application::updateStatistics( sf::Time dt )
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
    if( mStatisticsUpdateTime >= sf::seconds( 1.0f ) )
	{
        mStatisticsText.setString( "FPS: " + toString( mStatisticsNumFrames ) );
        mStatisticsUpdateTime -= sf::seconds( 1.0f );
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates( )
{
    // Engine States
    mStateStack.registerState<SplashScreen>             ( States::SplashScreen          );
    mStateStack.registerState<SettingsState>            ( States::SettingsState         );
    mStateStack.registerState<TitleState>               ( States::Title                 );
    mStateStack.registerState<LoadingState>             ( States::Loading               );
    mStateStack.registerState<MenuState>                ( States::Menu                  );
    mStateStack.registerState<MultiplayerGameState>     ( States::HostGame,      true   );
    mStateStack.registerState<MultiplayerGameState>     ( States::JoinGame,      false  );
    mStateStack.registerState<PauseState>               ( States::Pause,         false  );
    mStateStack.registerState<MessageBoxState>          ( States::MessageBox            );
    mStateStack.registerState<PauseState>               ( States::NetworkPause, true    );

    // Iconic States
    mStateStack.registerState<GameState>                ( States::GameState             );
}

