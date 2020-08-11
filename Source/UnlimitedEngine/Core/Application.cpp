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
#include "States/SinglePlayerMenuState.hpp"
#include "States/QuickBattleSetupState.hpp"
#include "States/ConquestModeSetup.hpp"
#include "States/ConquestOverviewState.hpp"
#include "States/ConquestBattleSetupState.hpp"
#include "States/ShopState.hpp"
#include "States/StoryModeSetupState.hpp"
#include "States/CutSceneState.hpp"
#include "States/BattleStatScreen.hpp"
#include "States/SinglePlayerBattle.hpp"
#include "Game/DataTables.hpp"
#include "Globals.hpp"


// Debugging only!!
#include <iostream>
#include <exception>

const sf::Time Application::TimePerFrame = sf::seconds( 1.f/ 60.f );

// Defined in Globals.hpp
sf::RenderWindow* mWindow = new sf::RenderWindow( sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "Wizard's Request", sf::Style::Default );
GameModes::ID GAME_MODE = GameModes::NONE;
States::ID CURRENT_LEVEL = States::SinglePlayerLevel1;
Category::Type CURRENT_TURN = Category::Blue;

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
    mStateStack.registerState<SplashScreen>             ( States::SplashScreen          );
    mStateStack.registerState<SettingsState>            ( States::SettingsState         );
    mStateStack.registerState<TitleState>               ( States::Title                 );
    mStateStack.registerState<LoadingState>             ( States::Loading               );
    mStateStack.registerState<MenuState>                ( States::Menu                  );
    mStateStack.registerState<MultiplayerGameState>     ( States::HostGame,      true   );
    mStateStack.registerState<MultiplayerGameState>     ( States::JoinGame,      false  );
    mStateStack.registerState<PauseState>               ( States::Pause,         false  );
    mStateStack.registerState<MessageBoxState>          ( States::MessageBox            );
    mStateStack.registerState<SinglePlayerMenuState>    ( States::SinglePlayerMenuState );
    mStateStack.registerState<QuickBattleSetupState>    ( States::QuickBattleSetupState );
    mStateStack.registerState<ConquestModeSetup>        ( States::ConquestModeSetup     );
    mStateStack.registerState<ConquestOverviewState>    ( States::ConquestOverviewState );
    mStateStack.registerState<ConquestBattleSetupState> ( States::ConquestBattleSetupState );
    mStateStack.registerState<ShopState>                ( States::ShopState );
    mStateStack.registerState<StoryModeSetupState>      ( States::StoryModeSetupState );
    mStateStack.registerState<BattleStatScreen>         ( States::BattleStatScreen );
    mStateStack.registerState<PauseState>               ( States::NetworkPause, true );

    // Levels 1 Player vs A.I
    mStateStack.registerState<SinglePlayerBattle>       ( States::SinglePlayerLevel1, LevelMap.at( "GeneratedGrassyPlains" ) );
    mStateStack.registerState<SinglePlayerBattle>       ( States::SinglePlayerLevel2, LevelMap.at( "Courtyard" ) );
    mStateStack.registerState<SinglePlayerBattle>       ( States::SinglePlayerLevel3, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<SinglePlayerBattle>       ( States::SinglePlayerLevel4, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<SinglePlayerBattle>       ( States::SinglePlayerLevel5, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<SinglePlayerBattle>       ( States::SinglePlayerLevel6, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<SinglePlayerBattle>       ( States::SinglePlayerLevel7, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<SinglePlayerBattle>       ( States::SinglePlayerLevel8, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<SinglePlayerBattle>       ( States::SinglePlayerLevel9, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<SinglePlayerBattle>       ( States::SinglePlayerLevel10, LevelMap.at( "TacticsTribeDemoLevel" ) );


    // Levels (2 player local machine)
    mStateStack.registerState<GameState>                ( States::Level1, LevelMap.at( "Courtyard" ) );
    mStateStack.registerState<GameState>                ( States::Level2, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<GameState>                ( States::Level3, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<GameState>                ( States::Level4, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<GameState>                ( States::Level5, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<GameState>                ( States::Level6, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<GameState>                ( States::Level7, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<GameState>                ( States::Level8, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<GameState>                ( States::Level9, LevelMap.at( "TacticsTribeDemoLevel" ) );
    mStateStack.registerState<GameState>                ( States::Level10, LevelMap.at( "TacticsTribeDemoLevel" ) );

    // CutScenes
    mStateStack.registerState<CutSceneState>            ( States::IntroCutSceneDialogState, States::SinglePlayerLevel1 );
    mStateStack.registerState<CutSceneState>            ( States::cutScene1State, States::SinglePlayerLevel2 );
    mStateStack.registerState<CutSceneState>            ( States::cutScene2State, States::SinglePlayerLevel3 );
    mStateStack.registerState<CutSceneState>            ( States::cutScene3State, States::SinglePlayerLevel4 );
    mStateStack.registerState<CutSceneState>            ( States::cutScene4State, States::SinglePlayerLevel5 );
    mStateStack.registerState<CutSceneState>            ( States::cutScene5State, States::SinglePlayerLevel6 );
    mStateStack.registerState<CutSceneState>            ( States::cutScene6State, States::SinglePlayerLevel7 );
    mStateStack.registerState<CutSceneState>            ( States::cutScene7State, States::SinglePlayerLevel8 );
    mStateStack.registerState<CutSceneState>            ( States::cutScene8State, States::SinglePlayerLevel9 );
    mStateStack.registerState<CutSceneState>            ( States::cutScene9State, States::SinglePlayerLevel10 );

}

