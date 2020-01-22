#include "TextNode.hpp"
#include "Core/Utility.hpp"
#include "Core/DataTables.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

    
TextNode::TextNode(const FontManager& fonts, const std::string& text)
{
    mText.setFont( fonts.get( FontMap.at( "Default" ) ) );
    mText.setCharacterSize( 20 );
    setString( text );
}

void TextNode::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mText, states );
}

void TextNode::setString( const std::string& text )
{
    mText.setString( text );
    centerOrigin( mText );
}

void TextNode::setColor( const sf::Color color )
{
    this->mText.setFillColor( color );
}