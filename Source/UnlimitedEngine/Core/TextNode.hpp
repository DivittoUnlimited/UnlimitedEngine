#ifndef TEXTNODE_HPP
#define TEXTNODE_HPP

#include "ResourceManager.hpp"
#include "SceneNode.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class TextNode : public SceneNode
{
public:
    explicit TextNode(const FontManager& fonts, const std::string& text);

    void setString(const std::string& text);
    void setColor( const sf::Color color );
    sf::Text* getText( void ) { return &mText; }

private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

    //## Attributes
    sf::Text mText;
};

#endif // TEXTNODE_HPP
