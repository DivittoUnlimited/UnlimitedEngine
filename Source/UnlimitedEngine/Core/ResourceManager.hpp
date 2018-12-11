#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

template<typename Resource>
class ResourceManager
{
public:
    void load( unsigned int id, const std::string& filename );

    template <typename Parameter>
    void load( unsigned int id, const std::string& filename, const Parameter& secondParam );

    Resource& get( unsigned int id );
    const Resource&	get( unsigned int id ) const;

    void remove( unsigned int id );

private:
    void insertResource( unsigned int id, std::unique_ptr<Resource> resource );

    // ## Attributes
    std::map<unsigned int, std::unique_ptr<Resource>>	mResourceMap;
};

#include "ResourceManager.inl"

#endif // RESOURCEMANAGER_HPP
