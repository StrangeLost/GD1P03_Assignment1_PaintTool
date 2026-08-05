/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name : cFileInterface.h
Description : This class helps with handling file browser dialog
Author : Rony Day Song
Mail : rony.song@mds.ac.nz
**************************************************************************/

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

