#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include "Category.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <memory>
#include <set>
#include <utility>

// Forward decs
struct Command;
struct CommandQueue;

///
/// \brief The SceneNode class
/// The base class for all things drawable.
class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
    typedef std::unique_ptr<SceneNode> Ptr;
    //typedef std::pair<SceneNode*, SceneNode*> Pair;

    explicit SceneNode(Category::Type category = Category::None);

    unsigned int            attachChild( Ptr child );
    Ptr						detachChild(const SceneNode& node);
    Ptr                     detachChild( const unsigned int index );

    void					update(sf::Time dt, CommandQueue& commands);

    sf::Vector2f			getWorldPosition() const;
    sf::Transform			getWorldTransform() const;

    void					onCommand(const Command& command, sf::Time dt);
    virtual unsigned int	getCategory() const;

    void					checkSceneCollision(SceneNode& sceneGraph, std::set<std::pair<SceneNode*, SceneNode*>>& collisionPairs);
    void					checkNodeCollision(SceneNode& node, std::set<std::pair<SceneNode*, SceneNode*>>& collisionPairs);
    void					removeWrecks();
    virtual sf::FloatRect	getBoundingRect() const;
    virtual bool			isMarkedForRemoval() const;
    virtual bool			isDestroyed() const;



private:
    virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);
    void					updateChildren(sf::Time dt, CommandQueue& commands);

    virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    void					drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
    void					drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;

    std::vector<Ptr>		mChildren;
    SceneNode*				mParent;
    Category::Type			mDefaultCategory;
};

bool	collision(const SceneNode& lhs, const SceneNode& rhs);
float	distance(const SceneNode& lhs, const SceneNode& rhs);

#endif // SCENENODE_HPP
