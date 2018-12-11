#ifndef EMITTERNODE_HPP
#define EMITTERNODE_HPP

#include "Core/SceneNode.hpp"
#include "Core/Particle.hpp"


class ParticleNode;

class EmitterNode : public SceneNode
{
	public:
        explicit				EmitterNode(unsigned int type);


	private:
		virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);
		
		void					emitParticles(sf::Time dt);


	private:
		sf::Time				mAccumulatedTime;
        unsigned int			mType;
		ParticleNode*			mParticleSystem;
};

#endif // EMITTERNODE_HPP
