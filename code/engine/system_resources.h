#pragma once
#include <SFML/Audio.hpp>
#ifdef SOUND
#include <SFML/Audio/Sound.hpp>

#endif // SOUND

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <string>
#include <map>

namespace Resources {

template <typename T> std::shared_ptr<T> load(const std::string& name) {
  throw("Resource loader unavailable for this type");
}

template <> // explicit specialization for T = Font
std::shared_ptr<sf::Font> load(const std::string& name);

template <> // explicit specialization for T = texture
std::shared_ptr<sf::Texture> load(const std::string& name);

#ifdef SOUND
template <> // explicit specialization for T = SoundBuffer
std::shared_ptr<sf::SoundBuffer> load(const std::string& name);

template <> // explicit specialization for T = Music
std::shared_ptr<sf::Music> load(const std::string& name);
#endif // SOUND

template <typename T>
static std::shared_ptr<T> get(const std::string& name) {
  static std::map<std::string, std::shared_ptr<T>> _things;
  auto search = _things.find(name);
  if (search != _things.end()) {
    return search->second;
  } else {
    _things[name] = load<T>(name);
    return _things[name];
  }
};

} // namespace Resources