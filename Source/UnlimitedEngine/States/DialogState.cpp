#include "DialogState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include "Gui/Button.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"
#include "Core/MusicPlayer.hpp"
#include "Core/Globals.hpp"
#include "Core/Utility.hpp"

DialogState::DialogState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mGUIContainer( )
{

    //mContext.music->setVolume( 30 );
    // mContext.music->play( MusicMap.at( "MenuTheme" ) );

    mTitleText = sf::Text( "Prompt:", mContext.fonts->get( FontMap.at( "Default" ) ), 20 );
    mTitleText.setFillColor( sf::Color( 0, 0, 0, 255 ) );
    centerOrigin( mTitleText );
    mTitleText.setPosition( WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 100 );
    mTitleText.setStyle( sf::Text::Bold );

    mMsg = sf::Text( "DialogState is dumb. You need to implement a state machine for\nthe Player class and make message boxes from SceneNodes insidethe world.\npossible on the hud layer.", mContext.fonts->get( FontMap.at( "Default" ) ), 16 );
    mMsg.setFillColor( sf::Color( 0, 0, 0, 255 ) );
    centerOrigin( mMsg );
    mMsg.setPosition( WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 200 );

    mBackgroundBox = sf::RectangleShape( sf::Vector2f( WINDOW_WIDTH - 24, 300 ) );
    mBackgroundBox.setPosition( 10, WINDOW_HEIGHT - 310 );
    mBackgroundBox.setOutlineThickness( 2 );
    mBackgroundBox.setOutlineColor( sf::Color::Black );
    mBackgroundBox.setFillColor( sf::Color( 181, 182, 228, 150 ) );

    auto nextButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    nextButton->setPosition( mTitleText.getPosition().x-100, mTitleText.getPosition().y + 190 );
    nextButton->setText( "Next..." );
    nextButton->setCallback( [this] ( )
    {
        requestStackPop( );
    });

    mGUIContainer.pack( nextButton );
}

DialogState::~DialogState( void )
{
   // mContext.music->stop( );
}

void DialogState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;

    window.setView( window.getDefaultView( ) );

    window.draw( mBackgroundBox );
    window.draw( mTitleText );
    window.draw( mMsg );
    window.draw( mGUIContainer );
}

bool DialogState::update( sf::Time )
{
    return false;
}

bool DialogState::handleEvent( const sf::Event& event )
{
    mGUIContainer.handleEvent( event );
    return false;
}
