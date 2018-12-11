#ifndef LABEL_HPP
#define LABEL_HPP

#include "Component.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"
#include <SFML/Graphics/Text.hpp>


namespace GUI
{

class Label : public Component
{
public:
    typedef std::shared_ptr<Label> Ptr;
            
    Label(const std::string& text, const FontManager& fonts);

    virtual bool isSelectable() const;
    void		 setText(const std::string& text);

    virtual void handleEvent(const sf::Event& event);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    //## Attributes
    sf::Text mText;
};

}

#endif // LABEL_HPP
