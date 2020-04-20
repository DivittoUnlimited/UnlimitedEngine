#include "SceneNode.hpp"
#include "Core/Command.hpp"
#include "Core/Utility.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>

SceneNode::SceneNode( Category::Type category )
: mChildren( )
, mParent( nullptr )
, mDefaultCategory( category )
{
}

unsigned int SceneNode::attachChild( Ptr child )
{
    child->mParent = this;
    mChildren.push_back( std::move( child ) );
    return static_cast<unsigned int>( mChildren.size( ) - 1 ); // index of new child
}

SceneNode::Ptr SceneNode::detachChild( const SceneNode& node )
{
    auto found = std::find_if( mChildren.begin( ), mChildren.end(), [&] (Ptr& p) { return p.get() == &node; });
    assert(found != mChildren.end( ) );

    Ptr result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase( found );
    return result;
}

SceneNode::Ptr SceneNode::detachChild( const unsigned int index )
{
    assert( mChildren.size() > index );
    Ptr result = std::move( mChildren.at( index ) );

    return result;
}

void SceneNode::update( sf::Time dt, CommandQueue& commands )
{
    updateCurrent( dt, commands );
    updateChildren( dt, commands );
}

void SceneNode::updateCurrent( sf::Time, CommandQueue& )
{
    // Do nothing by default
}

void SceneNode::updateChildren( sf::Time dt, CommandQueue& commands )
{
    for( Ptr& child : mChildren )
        child->update( dt, commands );
}

void SceneNode::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
    // Apply transform of current node
    states.transform *= getTransform( );

    // Draw node and children with changed transform
    drawCurrent( target, states );
    drawChildren( target, states );

    // Draw bounding rectangle - disabled by default      /// MEGA resource drain use with caution
    drawBoundingRect( target, states );
}

void SceneNode::drawCurrent( sf::RenderTarget&, sf::RenderStates ) const
{
    // Do nothing by default
}

void SceneNode::drawChildren( sf::RenderTarget& target, sf::RenderStates states ) const
{
    for( const Ptr& child : mChildren )
        child->draw(target, states);
}

void SceneNode::drawBoundingRect( sf::RenderTarget& target, sf::RenderStates ) const
{
    sf::FloatRect rect = getBoundingRect( );

    sf::RectangleShape shape;
    shape.setPosition( sf::Vector2f( rect.left, rect.top ) );
    shape.setSize( sf::Vector2f( rect.width, rect.height ) );
    shape.setFillColor( sf::Color::Transparent );
    shape.setOutlineColor( sf::Color( 255, 0, 255, 255 ) );
    shape.setOutlineThickness( 1.f );

    target.draw( shape );
}

sf::Vector2f SceneNode::getWorldPosition( ) const
{
    return getWorldTransform() * sf::Vector2f( );
}

sf::Transform SceneNode::getWorldTransform( ) const
{
    sf::Transform transform = sf::Transform::Identity;

    for (const SceneNode* node = this; node != nullptr; node = node->mParent)
        transform = node->getTransform() * transform;

    return transform;
}

void SceneNode::onCommand( const Command& command, sf::Time dt )
{
    // Command current node, if category matches
    if( command.category & getCategory( ) )
        command.action( *this, dt );

    // Command children
    for( Ptr& child : mChildren )
        child->onCommand( command, dt );
}

unsigned int SceneNode::getCategory( ) const
{
    return mDefaultCategory;
}

void SceneNode::checkSceneCollision( SceneNode& sceneGraph, std::set<std::pair<SceneNode *, SceneNode *> > &collisionPairs )
{
    checkNodeCollision( sceneGraph, collisionPairs );

    for( Ptr& child : sceneGraph.mChildren )
        checkSceneCollision( *child, collisionPairs );
}

void SceneNode::checkNodeCollision( SceneNode& node, std::set<std::pair<SceneNode*, SceneNode*>>& collisionPairs )
{
    if( this != &node && !isDestroyed( ) && !node.isDestroyed( ) && collision( *this, node )  )
        collisionPairs.insert( std::minmax( this, &node ) );

    for( Ptr& child : mChildren )
        child->checkNodeCollision( node, collisionPairs );
}

void SceneNode::removeWrecks( )
{
    // Remove all children which request so
    auto wreckfieldBegin = std::remove_if( mChildren.begin( ), mChildren.end( ), std::mem_fn( &SceneNode::isMarkedForRemoval ) );
    mChildren.erase( wreckfieldBegin, mChildren.end( ) );

    // Call function recursively for all remaining children
    std::for_each( mChildren.begin( ), mChildren.end( ), std::mem_fn( &SceneNode::removeWrecks ) );
}

sf::FloatRect SceneNode::getBoundingRect( void ) const
{
    return sf::FloatRect( );
}

bool SceneNode::isMarkedForRemoval( void ) const
{
    // By default, remove node if entity is destroyed
    return isDestroyed( );
}

bool SceneNode::isDestroyed( void ) const
{
    // By default, scene node needn't be removed
    return false;
}

void SceneNode::buildList( std::vector<SceneNode*>& objs )
{
    for( const Ptr& child : mChildren )
    {
        if( child->getCategory() != Category::ParticleSystem )
            child->buildList( objs );
    }
    if( this->getCategory() != Category::ParticleSystem )
        objs.push_back( this );
}


bool collision( const SceneNode& lhs, const SceneNode& rhs )
{
    return lhs.getBoundingRect( ).intersects( rhs.getBoundingRect ( ) );
}

float distance( const SceneNode& lhs, const SceneNode& rhs )
{
    return length( lhs.getWorldPosition( ) - rhs.getWorldPosition( ) );
}
