#ifndef PARTICLENODE_HPP
#define PARTICLENODE_HPP

#include "Graphics/SceneNode.hpp"
#include "Core/ResourceIdentifiers.hpp"
#include "Graphics/Particle.hpp"

#include <SFML/Graphics/VertexArray.hpp>

#include <deque>


class ParticleNode : public SceneNode
{
	public:
                                ParticleNode( unsigned int type, const TextureManager& textures );

        void					addParticle( sf::Vector2f position );
        unsigned int			getParticleType( ) const;
        virtual unsigned int	getCategory( ) const;

	private:
        virtual void			updateCurrent( sf::Time dt, CommandQueue& commands );
        virtual void			drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
		
        void					addVertex( float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color ) const;
        void					computeVertices( ) const;

	private:
		std::deque<Particle>	mParticles;
		const sf::Texture&		mTexture;
        unsigned int			mType;

		mutable sf::VertexArray	mVertexArray;
		mutable bool			mNeedsVertexUpdate;
};

#endif // PARTICLENODE_HPP
