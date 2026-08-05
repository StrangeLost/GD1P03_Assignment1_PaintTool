/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name : cPaint.h
Description : This is the core of the software.
Author : Rony Day Song
Mail : rony.song@mds.ac.nz
**************************************************************************/

#pragma once
#include <filesystem>
#include "imgui.h"
#include "imgui-SFML.h"
#include "cButton.h"
#include "cTextBox.h"
#include "cFileInterface.h"
#include "EllipseShape.h"

class cPaint
{
public:
	cPaint();
	~cPaint() = default;
	void Run();

private:
	// Core functions
	void HandleEvents();
	void Update(sf::Time deltaTime);
	void Draw();

	// Events Handling
	void HandleWindowResize(const sf::Event& event);
	void HandleButtonInput(const sf::Event& event);
	void HandleToolInput(const sf::Event& event);
	void HandleTextBoxes(const sf::Event& event);
	void HandleTextInput(const sf::Event& event);

	// Updates Handling
	void UpdateMousePosition();
	void UpdateButtons();
	void UpdateToolUse();
	void UpdateTextBoxes();

	// Draws handling
	void DrawButtons();
	void DrawTextBoxes();
	void DrawBrushColorPicker();
	void DrawOutlineColorPicker();

	// Helper Functions
	void SwitchTool(cButton& toolButton);
	sf::Vector2f WorldToCanvas(sf::Vector2f worldPosition) const;
	bool IsInsideCanvas(sf::Vector2f worldPosition) const;
	bool IsActiveButton(cButton& button) const;
	void SubmitTextValue();
	void FinishPolygon();

	// Use Drawing Tools
	void UseLineTool();
	void UseBoxFillTool();
	void UseEllipseFillTool();
	void UsePolygonTool();

	// Use Stamp Tool
	void LoadStampImage();
	void PlaceStamp();

	// Save & Load images
	void SaveCanvas(const std::filesystem::path& filePath);
	void LoadCanvas(const std::filesystem::path& filePath);
	
	// Window
	sf::RenderWindow m_window;
	sf::Vector2f m_mousePosition;

	// Canvas
	sf::RenderTexture m_canvas;
	sf::RenderTexture m_tempCanvas;
	sf::RectangleShape m_canvasDisplay;
	sf::RectangleShape m_tempCanvasDisplay;
	sf::Vector2f m_canvasPosition;
	sf::Vector2f m_canvasSize;

	// Drawing Brush
	bool m_isDrawing = false;
	float m_brushRadius = 8.f;
	size_t m_pointCount = 64;
	sf::Shape* m_brushShape;
	sf::Vector2f m_startDrawPosition;
	sf::Color m_brushColor = sf::Color::Black;
	float m_brushColorValues[4] = { 0.f, 0.f, 0.f, 1.f };
	bool m_showBrushColorPicker = false;

	// Brush Outlines
	float m_boxOutlineThickness;
	float m_ellipseOutlineThickness;
	float m_polygonOutlineThickness;
	sf::Color m_outlineColor = sf::Color::Red;
	float m_outlineColorValues[4] = { 0.f, 0.f, 0.f, 1.f };
	bool m_showOutlineColorPicker = false;
	
	// Polygon specific
	bool m_isPolygoning = false;
	std::vector<sf::Vector2f> m_polygonPoints;
	sf::VertexArray m_polygonVertex;
	sf::ConvexShape m_polygon;

	// Stamp specific
	sf::Texture m_stampImage;
	bool m_hasStampImage = false;
	float m_stampScale = 1.f;

	// Button base textures
	sf::Texture m_normalButtonTexture;
	sf::Texture m_hoveredButtonTexture;
	sf::Texture m_pressedButtonTexture;
	sf::Texture m_disabledButtonTexture;
	sf::Texture m_toggledButtonTexture;
	std::array<const sf::Texture*, 5> m_backgroundTextures;

	// Button icon textures
	sf::Texture m_lineTexture;
	sf::Texture m_boxFillTexture;
	sf::Texture m_ellipseFillTexture;
	sf::Texture m_polygonTexture;
	sf::Texture m_stampTexture;
	sf::Texture m_color1Texture;
	sf::Texture m_color2Texture;
	sf::Texture m_clearTexture;
	sf::Texture m_saveTexture;
	sf::Texture m_loadTexture;

	// Button base textures
	std::vector<cButton*> m_buttons;
	cButton* m_activeButton;
	cButton m_lineButton;
	cButton m_boxFillButton;
	cButton m_ellipseFillButton;
	cButton m_polygonButton;
	cButton m_stampButton;
	cButton m_color1Button;
	cButton m_color2Button;
	cButton m_clearButton;
	cButton m_saveButton;
	cButton m_loadButton;

	// Text boxes
	sf::Font m_font;
	std::vector<cTextBox*> m_textBoxes;
	cTextBox* m_activeTextBox;
	cTextBox m_brushTextBox;

	// File SAVE & ALOAD
	cFileInterface m_fileInterface;
};

