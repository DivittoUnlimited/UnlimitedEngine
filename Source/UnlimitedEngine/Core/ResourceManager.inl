#include "ResourceManager.hpp"

template <typename Resource>
void ResourceManager<Resource>::load( unsigned int id, const std::string& filename )
{
    // Create and load resource
    std::unique_ptr<Resource> resource( new Resource( ) );
    if( !resource->loadFromFile( filename ) )
        throw std::runtime_error( "ResourceManager::load - Failed to load " + filename );

    // If loading successful, insert resource to map
    insertResource( id, std::move( resource ) );
}

template <typename Resource>
template <typename Parameter>
void ResourceManager<Resource>::load( unsigned int id, const std::string& filename, const Parameter& secondParam )
{
    // Create and load resource
    std::unique_ptr<Resource> resource( new Resource( ) );
    if( !resource->loadFromFile( filename, secondParam ) )
        throw std::runtime_error( "ResourceHolder::load - Failed to load " + filename );

    // If loading successful, insert resource to map
    insertResource( id, std::move( resource ) );
}

template <typename Resource>
Resource& ResourceManager<Resource>::get( unsigned int id )
{
    auto found = mResourceMap.find( id );
    assert( found != mResourceMap.end( ) );

    return *found->second;
}

template <typename Resource>
const Resource& ResourceManager<Resource>::get( unsigned int id) const
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());

    return *found->second;
}

template <typename Resource>
void ResourceManager<Resource>::insertResource( unsigned int id, std::unique_ptr<Resource> resource)
{
    // Insert and check success
    auto inserted = mResourceMap.insert( std::make_pair( id, std::move( resource ) ) );
    assert( inserted.second );
}

template<typename Resource>
void ResourceManager<Resource>::remove( unsigned int id )
{
    mResourceMap.erase( id );
}
