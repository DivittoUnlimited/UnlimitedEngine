#include "ShopState.hpp"


#include "ShopState.hpp"
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

ShopState::ShopState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mGUIContainer( )
{
    // mContext.music->setVolume( 30 );
    // mContext.music->play( MusicMap.at( "MenuTheme" ) );

    mTitleText = sf::Text( "Shop and\nUpgrades", mContext.fonts->get( FontMap.at( "Default" ) ), 100 );
    mTitleText.setFillColor( sf::Color( 181, 182, 228, 255 ) );
    centerOrigin( mTitleText );
    mTitleText.setPosition( WINDOW_WIDTH / 2, 200 );
    mTitleText.setOutlineColor( sf::Color( 79, 67, 174, 255 ) );
    mTitleText.setOutlineThickness( 5 );
    mTitleText.setStyle( sf::Text::Bold );

    auto exitButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exitButton->setPosition( WINDOW_WIDTH / 2 - 100, 450 );
    exitButton->setText( "Exit" );
    exitButton->setCallback( [this] ( )
    {
        requestStackPop( );
    });

    mGUIContainer.pack( exitButton );
}

ShopState::~ShopState()
{
   // mContext.music->stop( );
}

void ShopState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView( window.getDefaultView( ) );
    window.clear( sf::Color::Black );
    window.draw( mTitleText );
    window.draw( mGUIContainer );
}

bool ShopState::update( sf::Time )
{
    return true;
}

bool ShopState::handleEvent( const sf::Event& event )
{
    mGUIContainer.handleEvent( event );
    return false;
}
