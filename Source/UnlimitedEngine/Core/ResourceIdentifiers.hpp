#ifndef RESOURCEIDENTIFIERS_HPP
#define RESOURCEIDENTIFIERS_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>
#include <map>

#include "Tiled/TiledManager.hpp"

// Forward declaration of SFML classes
namespace sf
{
    class Texture;
    class Font;
    class Sound;
    class Music;
    class Shader;
}

///
///
///
/// This should be obsoleted by Lua version and needs to be removed.
///
/// |
/// V
namespace Shaders
{
    enum ID
    {
        BrightnessPass,
        DownSamplePass,
        GaussianBlurPass,
        AddPass,
    };
}

// Forward declaration and a few type definitions
template <typename Resource>
class ResourceManager;

typedef ResourceManager<sf::Texture>        TextureManager;
typedef ResourceManager<sf::Font>           FontManager;
typedef ResourceManager<sf::SoundBuffer>    SoundManager;
typedef ResourceManager<sf::Shader>         ShaderManager;
typedef ResourceManager<Tiled::TiledMap>    TiledMapManager;

#endif // RESOURCEIDENTIFIERS_HPP
