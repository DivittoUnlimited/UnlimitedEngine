#include "MusicPlayer.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceManager.hpp"
#include "Game/DataTables.hpp"

MusicPlayer::MusicPlayer( )
: mMusic( )
, mFilenames( )
, mVolume( 100.f )
{
    /*
    mFilenames[MusicMap.at( "MenuTheme" )] = "Media/Music/MenuTheme.ogg";
    mFilenames[MusicMap.at( "GameTheme" )] = "Media/Music/MissionTheme.ogg";
    mFilenames[MusicMap.at( "FuelLow"   )] = "Media/Music/FuelLow.ogg";
    */
}

void MusicPlayer::play( unsigned int theme )
{
	std::string filename = mFilenames[theme];

    if( !mMusic.openFromFile( filename ) )
        throw std::runtime_error( "Music " + filename + " could not be loaded." );

    mMusic.setVolume( mVolume );
    mMusic.setLoop( true );
    mMusic.play( );
}

void MusicPlayer::stop( )
{
    mMusic.stop( );
}

void MusicPlayer::setVolume( float volume )
{
	mVolume = volume;
}

void MusicPlayer::setPaused( bool paused )
{
    if( paused )
        mMusic.pause( );
	else
        mMusic.play( );
}

