/*
 * Alows the player to select new or load game
 * as well as the dificulty amoung other settings before they begin
 * the story mode
 */

#include "StoryModeSetupState.hpp"
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

StoryModeSetupState::StoryModeSetupState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mGUIContainer( )
{
    // mContext.music->setVolume( 30 );
    // mContext.music->play( MusicMap.at( "MenuTheme" ) );

    mTitleText = sf::Text( "Story Mode\nSetup", mContext.fonts->get( FontMap.at( "Default" ) ), 100 );
    mTitleText.setFillColor( sf::Color( 181, 182, 228, 255 ) );
    centerOrigin( mTitleText );
    mTitleText.setPosition( WINDOW_WIDTH / 2, 200 );
    mTitleText.setOutlineColor( sf::Color( 79, 67, 174, 255 ) );
    mTitleText.setOutlineThickness( 5 );
    mTitleText.setStyle( sf::Text::Bold );

    auto newGameButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    newGameButton->setPosition( WINDOW_WIDTH / 2 - 100, 450 );
    newGameButton->setText( "New Game" );
    newGameButton->setCallback( [this] ( )
    {
        requestStackPop( );
        requestStackPush( States::IntroCutSceneDialogState );
    });

    auto loadGameButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    loadGameButton->setPosition( WINDOW_WIDTH / 2 - 100, 510 );
    loadGameButton->setText( "Load Game(STUB)" );
    loadGameButton->setCallback( [this] ( )
    {
        //requestStackPop( );
        //requestStackPush(  );
    });

    auto exitButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exitButton->setPosition( WINDOW_WIDTH / 2 - 100, 570 );
    exitButton->setText( "Exit" );
    exitButton->setCallback( [this] ( )
    {
        requestStackPop( );
        requestStackPush( States::Menu );
    });

    mGUIContainer.pack( newGameButton );
    mGUIContainer.pack( loadGameButton );
    mGUIContainer.pack( exitButton );
}

StoryModeSetupState::~StoryModeSetupState()
{
   // mContext.music->stop( );
}

void StoryModeSetupState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;

    window.setView( window.getDefaultView( ) );

    window.draw( mTitleText );
    window.draw( mGUIContainer );
}

bool StoryModeSetupState::update( sf::Time )
{
    return true;
}

bool StoryModeSetupState::handleEvent( const sf::Event& event )
{
    mGUIContainer.handleEvent( event );
    return false;
}
