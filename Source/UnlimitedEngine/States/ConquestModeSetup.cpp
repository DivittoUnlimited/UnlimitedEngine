/*
 * This page is where the players will choose the AI difficulty and other settings to begin a conquest mode match.
 *
 */

#include "ConquestModeSetup.hpp"
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

ConquestModeSetup::ConquestModeSetup( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mGUIContainer( )
{
    // mContext.music->setVolume( 30 );
    // mContext.music->play( MusicMap.at( "MenuTheme" ) );

    mTitleText = sf::Text( "Conquest Setup", mContext.fonts->get( FontMap.at( "Default" ) ), 100 );
    mTitleText.setFillColor( sf::Color( 181, 182, 228, 255 ) );
    centerOrigin( mTitleText );
    mTitleText.setPosition( WINDOW_WIDTH / 2, 200 );
    mTitleText.setOutlineColor( sf::Color( 79, 67, 174, 255 ) );
    mTitleText.setOutlineThickness( 5 );
    mTitleText.setStyle( sf::Text::Bold );

    auto startConquestButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    startConquestButton->setPosition( WINDOW_WIDTH / 2 - 100, 450 );
    startConquestButton->setText( "Start Conquest" );
    startConquestButton->setCallback( [this] ( )
    {
        requestStackPop( );
        requestStackPush( States::ConquestOverviewState );
    });

    auto exitButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exitButton->setPosition( WINDOW_WIDTH / 2 - 100, 510 );
    exitButton->setText( "Return to Menu" );
    exitButton->setCallback( [this] ( )
    {
        requestStackPop( );
        requestStackPush( States::Menu );
    });

    mGUIContainer.pack( startConquestButton );
    mGUIContainer.pack( exitButton );
}

ConquestModeSetup::~ConquestModeSetup()
{
   // mContext.music->stop( );
}

void ConquestModeSetup::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;

    window.setView( window.getDefaultView( ) );

    window.draw( mTitleText );
    window.draw( mGUIContainer );
}

bool ConquestModeSetup::update( sf::Time )
{
    return true;
}

bool ConquestModeSetup::handleEvent( const sf::Event& event )
{
    mGUIContainer.handleEvent( event );
    return false;
}

