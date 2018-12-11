#ifndef SOUNDPLAYER_HPP
#define SOUNDPLAYER_HPP

#include "Core/ResourceManager.hpp"
#include  "Core/ResourceIdentifiers.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <list>


class SoundPlayer : private sf::NonCopyable
{
    public:
        SoundPlayer( );

        void play(unsigned int effect, bool repeated = false );
        void play( unsigned int effect, sf::Vector2f position, unsigned int volume, bool repeated = false );
        void setVolume( unsigned int volume );

        void removeStoppedSounds( );
        void setListenerPosition( sf::Vector2f position );


        sf::Vector2f getListenerPosition( ) const;


        SoundManager     			mSoundBuffers;

private:
		std::list<sf::Sound>		mSounds;
};

#endif // BOOK_SOUNDPLAYER_HPP
