#ifndef TEXTURES_H
#define TEXTURES_H

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <string>
#include <memory>

#define IMAGES_PATH "../data/images/" // Relative path to images folder
#define IMAGESIZE 32

#define TILE_HIDDEN_PNG IMAGES_PATH "tile_hidden.png"
#define TILE_REVEALED_PNG IMAGES_PATH "tile_revealed.png"
#define TILE_FLAG_PNG IMAGES_PATH "flag.png"
#define TILE_MINE_PNG IMAGES_PATH "mine.png"
#define TILE_NUMBER_PNG_PREFIX IMAGES_PATH "number_" // Add character number 1-8.png to this

#define FACE_PLAY_PNG IMAGES_PATH "face_happy.png"
#define FACE_LOSE_PNG IMAGES_PATH "face_lose.png"
#define FACE_WIN_PNG IMAGES_PATH "face_win.png"

#define DEBUG_BTN_PNG IMAGES_PATH "debug.png"
#define TEST_PNG_PREFIX IMAGES_PATH "test_" // Add character number 1-3.png to this

#define DIGITS_PNG IMAGES_PATH "digits.png"
#define DIGITS_PNG_OFFSET 21 // Each digit in the digits.png file is offset by 21 pixels

/// @brief Container for all sf::Texture objects.
class Textures
{
public:
	/// @param name The name of the texture to get
	/// @return The texture object associated with the passed in name
	static std::shared_ptr<sf::Texture> getTexture(const std::string &name);

private:
	// Use smart pointers to ensure proper deletion of textures once they are no longer being used

	static std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures; // Texture map

	/// @brief Load a texture into the texture map from an image file
	static void loadTexture(const std::string &file);
};
#endif