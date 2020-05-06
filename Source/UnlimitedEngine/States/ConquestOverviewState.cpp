

#include "ConquestOverviewState.hpp"
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

ConquestOverviewState::ConquestOverviewState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mGUIContainer( )
{
    // mContext.music->setVolume( 30 );
    // mContext.music->play( MusicMap.at( "MenuTheme" ) );

    mTitleText = sf::Text( "Conquest Overview\n\tState", mContext.fonts->get( FontMap.at( "Default" ) ), 100 );
    mTitleText.setFillColor( sf::Color( 181, 182, 228, 255 ) );
    centerOrigin( mTitleText );
    mTitleText.setPosition( WINDOW_WIDTH / 2, 200 );
    mTitleText.setOutlineColor( sf::Color( 79, 67, 174, 255 ) );
    mTitleText.setOutlineThickness( 5 );
    mTitleText.setStyle( sf::Text::Bold );

    auto startBattleButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    startBattleButton->setPosition( WINDOW_WIDTH / 2 - 100, 450 );
    startBattleButton->setText( "Start Battle!" );
    startBattleButton->setCallback( [this] ( )
    {
        requestStackPush( States::ConquestBattleSetupState );
    });

    auto ShopButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    ShopButton->setPosition( WINDOW_WIDTH / 2 - 100, 510 );
    ShopButton->setText( "Shop and upgrades" );
    ShopButton->setCallback( [this] ( )
    {
        requestStackPush( States::ShopState );
    });

    auto exitButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exitButton->setPosition( WINDOW_WIDTH / 2 - 100, 570 );
    exitButton->setText( "Return to Menu" );
    exitButton->setCallback( [this] ( )
    {
        requestStackPop( );
        requestStackPush( States::Menu );
    });

    mGUIContainer.pack( startBattleButton );
    mGUIContainer.pack( ShopButton );
    mGUIContainer.pack( exitButton );
}

ConquestOverviewState::~ConquestOverviewState()
{
   // mContext.music->stop( );
}

void ConquestOverviewState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;

    window.setView( window.getDefaultView( ) );

    window.draw( mTitleText );
    window.draw( mGUIContainer );
}

bool ConquestOverviewState::update( sf::Time )
{
    return true;
}

bool ConquestOverviewState::handleEvent( const sf::Event& event )
{
    mGUIContainer.handleEvent( event );
    return false;
}
