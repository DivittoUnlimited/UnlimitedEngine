/*
 * This screen presents the player with all the play options that are available to play against the AI
 * including:
 * - QuickBattle
 * - StoryMode
 * - Conquest Mode
 *
 */
#include "SinglePlayerMenuState.hpp"
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

#include <fstream>

SinglePlayerMenuState::SinglePlayerMenuState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mGUIContainer( )
{
    // mContext.music->setVolume( 30 );
    // mContext.music->play( MusicMap.at( "MenuTheme" ) );

    mTitleText = sf::Text( "Single Player!", mContext.fonts->get( FontMap.at( "Default" ) ), 100 );
    mTitleText.setFillColor( sf::Color( 181, 182, 228, 255 ) );
    centerOrigin( mTitleText );
    mTitleText.setPosition( WINDOW_WIDTH / 2, 200 );
    mTitleText.setOutlineColor( sf::Color( 79, 67, 174, 255 ) );
    mTitleText.setOutlineThickness( 5 );
    mTitleText.setStyle( sf::Text::Bold );

    auto quickBattle = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    quickBattle->setPosition( WINDOW_WIDTH / 2 - 100, 450 );
    quickBattle->setText( "Quick Battle" );
    quickBattle->setCallback( [this] ( )
    {
        GAME_MODE = GameModes::QuickBattle;
        requestStackPop( );
        requestStackPush( States::QuickBattleSetupState );
    });

    auto storyMode = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    storyMode->setPosition( WINDOW_WIDTH / 2 - 100, 510 );
    storyMode->setText( "Story Mode" );
    storyMode->setCallback( [this] ( )
    {
        GAME_MODE = GameModes::StoryMode;
        requestStackPop( );
        requestStackPush( States::StoryModeSetupState );
    });

    auto conquestMode = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    conquestMode->setPosition( WINDOW_WIDTH / 2 - 100, 570 );
    conquestMode->setText( "Conquest" );
    conquestMode->setCallback( [this] ( )
    {
        GAME_MODE = GameModes::Conquest;
        requestStackPop( );
        requestStackPush( States::ConquestModeSetup );
    });

    auto exitButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exitButton->setPosition( WINDOW_WIDTH / 2 - 100, 630 );
    exitButton->setText( "Return to Menu" );
    exitButton->setCallback( [this] ( )
    {
        GAME_MODE = GameModes::NONE;
        requestStackPop( );
        requestStackPush( States::Menu );
    });

    mGUIContainer.pack( quickBattle );
    mGUIContainer.pack( storyMode );
    mGUIContainer.pack( conquestMode );
    mGUIContainer.pack( exitButton );
}

SinglePlayerMenuState::~SinglePlayerMenuState()
{
   // mContext.music->stop( );
}

void SinglePlayerMenuState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;

    window.setView( window.getDefaultView( ) );

    window.draw( mTitleText );
    window.draw( mGUIContainer );
}

bool SinglePlayerMenuState::update( sf::Time )
{
    return true;
}

bool SinglePlayerMenuState::handleEvent( const sf::Event& event )
{
    mGUIContainer.handleEvent( event );
    return false;
}
