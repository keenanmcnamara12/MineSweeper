#include "Game.h"

std::map<std::string, sf::Texture*> textureGlobal;

sf::Texture* getTexture(std::string file) {
  map<string, sf::Texture*>::iterator a = textureGlobal.find(file);
  
  // If the texture is already loaded in the map, return it
  if (a != textureGlobal.end())
    return a->second;
  
  sf::Texture *texture = new sf::Texture();
  if (texture->loadFromFile(file)) {
    textureGlobal[file] = texture;
    return textureGlobal[file];
  } 
  
  delete texture; 
  std::cout << "Error: Loading from file\n";
  return textureGlobal[file];
}
