#include "ConquestBattleSetupState.hpp"

#include "ConquestBattleSetupState.hpp"
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

ConquestBattleSetupState::ConquestBattleSetupState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mGUIContainer( )
{
    // mContext.music->setVolume( 30 );
    // mContext.music->play( MusicMap.at( "MenuTheme" ) );

    mTitleText = sf::Text( "Battle Setup", mContext.fonts->get( FontMap.at( "Default" ) ), 100 );
    mTitleText.setFillColor( sf::Color( 181, 182, 228, 255 ) );
    centerOrigin( mTitleText );
    mTitleText.setPosition( WINDOW_WIDTH / 2, 200 );
    mTitleText.setOutlineColor( sf::Color( 79, 67, 174, 255 ) );
    mTitleText.setOutlineThickness( 5 );
    mTitleText.setStyle( sf::Text::Bold );

    auto startBattleButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    startBattleButton->setPosition( WINDOW_WIDTH / 2 - 100, 450 );
    startBattleButton->setText( "Start Battle!(TEST_MAP)" );
    startBattleButton->setCallback( [this] ( )
    {
        requestStackPop( );
        requestStackPush( States::BattleState );
    });

    auto exitButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exitButton->setPosition( WINDOW_WIDTH / 2 - 100, 510 );
    exitButton->setText( "Return to Menu" );
    exitButton->setCallback( [this] ( )
    {
        requestStackPop( );
    });

    mGUIContainer.pack( startBattleButton );
    mGUIContainer.pack( exitButton );
}

ConquestBattleSetupState::~ConquestBattleSetupState()
{
   // mContext.music->stop( );
}

void ConquestBattleSetupState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;

    window.clear( sf::Color::Black );
    window.setView( window.getDefaultView( ) );

    window.draw( mTitleText );
    window.draw( mGUIContainer );
}

bool ConquestBattleSetupState::update( sf::Time )
{
    return true;
}

bool ConquestBattleSetupState::handleEvent( const sf::Event& event )
{
    mGUIContainer.handleEvent( event );
    return false;
}
