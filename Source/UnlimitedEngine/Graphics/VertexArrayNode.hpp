
#ifndef VERTEXARRAYNODE_HPP
#define VERTEXARRAYNODE_HPP

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Graphics/SceneNode.hpp"
#include "Core/DataTables.hpp"
#include "Core/ResourceIdentifiers.hpp"
#include "Core/ResourceManager.hpp"

class VertexArrayNode : public SceneNode
{
public:
    explicit VertexArrayNode( Category::Type = Category::TileLayer );
    bool load( sf::Texture tex, sf::Vector2u tileSize, std::vector<unsigned int> tiles, unsigned int width, unsigned int height );

private:
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;

    virtual void			drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
};

#endif // VERTEXARRAYNODE_HPP
