#pragma once
#include <windows.h>
#include <ShObjIdl.h>
#include <string>
#include "cButton.h"

class cFileInterface
{
public:
	cFileInterface();
	~cFileInterface();

	void LoadFile(sf::RenderTexture* canvas);
	void SaveFile(sf::RenderTexture* canvas);
	void LoadPath(sf::RenderTexture* canvas, sf::Texture& texture);
};

