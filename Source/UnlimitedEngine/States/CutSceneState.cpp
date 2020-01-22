#include "CutSceneState.hpp"


#include "CutSceneState.hpp"
#include "MenuState.hpp"
#include "Gui/Button.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"
#include "Core/MusicPlayer.hpp"
#include "Gui/MessageBoxNode.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include "Core/Globals.hpp"
#include "Core/Utility.hpp"

#include <fstream>

CutSceneState::CutSceneState( States::ID id, StateStack& stack, Context context, States::ID nextState )
    : State( id, stack, context )
    , mNextState( nextState )
    , mHasRanDialog( false )
    , mPortrait( )
{
    // mContext.music->setVolume( 30 );
    // mContext.music->play( MusicMap.at( "MenuTheme" ) );

    mContext.textures->load( TextureMap.at( "DemoGirl" ), MediaFileMap.at( "Textures" ).at( TextureMap.at( "DemoGirl" ) ) );


    switch( CURRENT_LEVEL )
    {
        case States::Level1:
            mMessageBox = new MessageBoxNode( "IntroCutSceneDialog",  *context.fonts );
        break;
        case States::Level2:
            mMessageBox = new MessageBoxNode( "Chapter1", *context.fonts );
        break;
        case States::Level3:
            mMessageBox = new MessageBoxNode( "Chapter2", *context.fonts );
        break;
        case States::Level4:
            mMessageBox = new MessageBoxNode( "Chapter3", *context.fonts );
        break;
        case States::Level5:
            mMessageBox = new MessageBoxNode( "Chapter4", *context.fonts );
        break;
        case States::Level6:
            mMessageBox = new MessageBoxNode( "Chapter5", *context.fonts );
        break;
        case States::Level7:
            mMessageBox = new MessageBoxNode( "Chapter6", *context.fonts );
        break;
        case States::Level8:
            mMessageBox = new MessageBoxNode( "Chapter7", *context.fonts );
        break;
        case States::Level9:
            mMessageBox = new MessageBoxNode( "Chapter8", *context.fonts );
        break;
        case States::Level10:
            mMessageBox = new MessageBoxNode( "Chapter9", *context.fonts );
            std::cout << "You Win! Role Credits..." << std::endl;
            requestStackPop( );
            requestStackPush( States::Menu );
        break;
        default: break;
    }
    mMessageBox->setPosition( 50, 400 );



}

CutSceneState::~CutSceneState()
{
    // mContext.music->stop( );
    mContext.textures->remove( TextureMap.at( "DemoGirl" ) );
}

void CutSceneState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView( window.getDefaultView( ) );
    window.clear( sf::Color::Black );

    if( mMessageBox->getPortrait() != -1 )
        window.draw( mPortrait );
    window.draw( *mMessageBox );

}

bool CutSceneState::update( sf::Time )
{
    if( mMessageBox->complete( ) )
    {
        requestStackPop( );
        requestStackPush( CURRENT_LEVEL );
        switch( CURRENT_LEVEL )
        {
            case States::Level1:
                CURRENT_LEVEL = States::Level2;
            break;
            case States::Level2:
                CURRENT_LEVEL = States::Level3;
            break;
            case States::Level3:
                CURRENT_LEVEL = States::Level4;
            break;
            case States::Level4:
                CURRENT_LEVEL = States::Level5;
            break;
            case States::Level5:
                CURRENT_LEVEL = States::Level6;
            break;
            case States::Level6:
                CURRENT_LEVEL = States::Level7;
            break;
            case States::Level7:
                CURRENT_LEVEL = States::Level8;
            break;
            case States::Level8:
                CURRENT_LEVEL = States::Level9;
            break;
            case States::Level9:
                CURRENT_LEVEL = States::Level10;
            break;

            default: break;
        }

    }
    else
    {
        // make sure portrait(s) are in correctly and in the right place

        if( mMessageBox->getPortrait() != -1 )
        {
            mPortrait.setTexture( mContext.textures->get( static_cast<unsigned int>( mMessageBox->getPortrait( ) ) ) );
            mPortrait.setPosition( mMessageBox->getPortraitPosition() );
        }


    }
    return true;
}

bool CutSceneState::handleEvent( const sf::Event& event )
{
    mMessageBox->handleEvent( event );
    return false;
}
