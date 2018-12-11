#ifndef SOUNDNODE_HPP
#define SOUNDNODE_HPP

#include "Core/SceneNode.hpp"
#include "ResourceIdentifiers.hpp"

class SoundPlayer;

class SoundNode : public SceneNode
{
public:
    explicit SoundNode( SoundPlayer& player );
    void playSound( unsigned int sound, sf::Vector2f position, bool repeated = false );

    virtual unsigned int	getCategory( ) const;

private:
        SoundPlayer& mSounds;
};

#endif // SOUNDNODE_HPP
