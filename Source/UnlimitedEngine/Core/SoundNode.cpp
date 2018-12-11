#include "Core/SoundNode.hpp"
#include "Core/SoundPlayer.hpp"

SoundNode::SoundNode( SoundPlayer& player )
: SceneNode( )
, mSounds( player )
{
}

void SoundNode::playSound( unsigned int sound, sf::Vector2f position, bool repeated )
{
    mSounds.play( sound, position, repeated );
}

unsigned int SoundNode::getCategory( ) const
{
	return Category::SoundEffect;
}
