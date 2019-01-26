#include "VertexArrayNode.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

VertexArrayNode::VertexArrayNode( void )
{
}

bool VertexArrayNode::load( sf::Texture tex, sf::Vector2u tileSize, std::vector<unsigned int> tiles, unsigned int width, unsigned int height )
{
     // load the tileset texture
     //if( !m_tileset.loadFromFile( tex ) )
     //   return false;

     m_tileset = tex;

     // resize the vertex array to fit the level size
     m_vertices.setPrimitiveType( sf::Quads );
     m_vertices.resize( width * height * 4 );
     // populate the vertex array, with one quad per tile
     for( unsigned int i = 0; i < width; ++i )
        for( unsigned int j = 0; j < height; ++j )
        {
            // get the current tile number
            int tileNumber = tiles[i + j * width];

            if( tileNumber )
            {
                // find its position in the tileset texture
                int tu = tileNumber % ( m_tileset.getSize( ).x / tileSize.x );
                int tv = tileNumber / ( m_tileset.getSize( ).x / tileSize.x );

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[( i + j * width ) * 4 ];
                // define its 4 corners
                quad[0].position = sf::Vector2f( i * tileSize.x, j * tileSize.y );
                quad[1].position = sf::Vector2f( (i + 1) * tileSize.x, j * tileSize.y );
                quad[2].position = sf::Vector2f( (i + 1) * tileSize.x, (j + 1) * tileSize.y );
                quad[3].position = sf::Vector2f( i * tileSize.x, (j + 1) * tileSize.y );

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f( (tu * tileSize.x) - tileSize.x , tv * tileSize.y );
                quad[1].texCoords = sf::Vector2f( ( tu + 1 ) * tileSize.x- tileSize.x, tv * tileSize.y );
                quad[2].texCoords = sf::Vector2f( ( tu + 1 ) * tileSize.x- tileSize.x, (tv + 1) * tileSize.y );
                quad[3].texCoords = sf::Vector2f( tu * tileSize.x- tileSize.x, (tv + 1) * tileSize.y );
            }
        }
    return true;
}

void VertexArrayNode::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    // apply the entity's transform -- combine it with the one that was passed by the caller
    states.transform *=  getTransform( ); // getTransform() is defined by sf::Transformable

    // apply the texture
    states.texture = &m_tileset;

    // you may also override states.shader or states.blendMode if you want

    // draw the vertex array
    target.draw( m_vertices, states );
}
