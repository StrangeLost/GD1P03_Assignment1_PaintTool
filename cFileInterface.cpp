/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name : cFileInterface.cpp
Description : Implementation of cFileInterface.h
Author : Rony Day Song
Mail : rony.song@mds.ac.nz
**************************************************************************/

#include <filesystem>
#include <optional>
#include "cFileInterface.h"

cFileInterface::cFileInterface()
{
}

cFileInterface::~cFileInterface()
{
}

void cFileInterface::LoadFile(sf::RenderTexture* canvas)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* pFileOpen;

		// here we create the ofd
		hr = CoCreateInstance(
			CLSID_FileOpenDialog, 
			NULL, 
			CLSCTX_ALL,
			IID_IFileOpenDialog,
			reinterpret_cast<void**>(&pFileOpen)
		);

		LPCWSTR szJPG = L"Images";

		COMDLG_FILTERSPEC rgSpec[] =
		{
			{szJPG, L"*.jpg;*.jpeg;*.png;*.bmp"},
		};

		pFileOpen->SetFileTypes(1, rgSpec);

		if (SUCCEEDED(hr))
		{
			// Show the Dialog Box
			hr = pFileOpen->Show(NULL);

			if (SUCCEEDED(hr))
			{
				// Get the name of the file
				IShellItem* pItem;

				hr = pFileOpen->GetResult(&pItem);

				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;

					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						sf::Texture newTex(pszFilePath);
						sf::Sprite newSprite(newTex);

						canvas->draw(newSprite);
						canvas->display();

						MessageBoxW(
							NULL,
							L"Image Loaded Successfully!",
							L"Success!",
							MB_OK
						);

						CoTaskMemFree(pszFilePath);
					}

					pItem->Release();
				}
			}

			pFileOpen->Release();
		}

		CoUninitialize();
	}
}

void cFileInterface::SaveFile(sf::RenderTexture* canvas)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileSaveDialog* pFileSave;

		// here we create the ofd
		hr = CoCreateInstance(
			CLSID_FileSaveDialog,
			NULL,
			CLSCTX_ALL,
			IID_IFileSaveDialog,
			reinterpret_cast<void**>(&pFileSave)
		);

		LPCWSTR szPNG = L"Image PNG";

		COMDLG_FILTERSPEC rgSpec[] =
		{
			{szPNG, L"*.png"}
		};

		if (SUCCEEDED(hr))
		{
			// Show the Dialog Box
			pFileSave->SetFileTypes(1, rgSpec);
			pFileSave->SetDefaultExtension(L"png");
			pFileSave->SetFileName(L"Painting.png");
			hr = pFileSave->Show(NULL);

			if (SUCCEEDED(hr))
			{
				// Get the name of the file
				IShellItem* pItem;

				hr = pFileSave->GetResult(&pItem);

				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;

					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						canvas->display();

						const sf::Image image = canvas->getTexture().copyToImage();

						std::filesystem::path filePath(pszFilePath);
						filePath.replace_extension(".png");

						if (image.saveToFile(filePath))
						{
							MessageBoxW(
								NULL,
								L"Image Saved Successfully!",
								L"Success!",
								MB_OK
							);
						}
						else
						{
							MessageBoxW(
								NULL,
								L"The image could not be saved.",
								L"Save Error",
								MB_OK | MB_ICONERROR
							);
						}
					}

					pItem->Release();
				}
			}

			pFileSave->Release();
		}

		CoUninitialize();
	}
}

void cFileInterface::LoadPath(sf::RenderTexture* canvas, sf::Texture& texture)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* pFileOpen;

		// here we create the ofd
		hr = CoCreateInstance(
			CLSID_FileOpenDialog,
			NULL,
			CLSCTX_ALL,
			IID_IFileOpenDialog,
			reinterpret_cast<void**>(&pFileOpen)
		);

		LPCWSTR szJPG = L"Images";

		COMDLG_FILTERSPEC rgSpec[] =
		{
			{szJPG, L"*.jpg;*.jpeg;*.png;*.bmp"},
		};

		pFileOpen->SetFileTypes(1, rgSpec);

		if (SUCCEEDED(hr))
		{
			// Show the Dialog Box
			hr = pFileOpen->Show(NULL);

			if (SUCCEEDED(hr))
			{
				// Get the name of the file
				IShellItem* pItem;

				hr = pFileOpen->GetResult(&pItem);

				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
				
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						texture.loadFromFile(pszFilePath);

						MessageBoxW(
							NULL,
							L"Stamp Loaded Successfully!",
							L"Success!",
							MB_OK
						);

						CoTaskMemFree(pszFilePath);
					}

					pItem->Release();
				}
			}

			pFileOpen->Release();
		}

		CoUninitialize();
	}
}
