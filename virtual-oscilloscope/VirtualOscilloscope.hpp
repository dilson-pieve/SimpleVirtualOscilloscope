#pragma once

#include "FunctionGenerator.hpp"
#include "Application.hpp"
#include "Trace.hpp"
#include "Menu.hpp"

#include <vector>

constexpr static float TraceSizeFactor = 3.0f / 4.0f;

class VirtualOscilloscope : public Application
{
public:
	bool OnCreate() override
	{
		m_Signal.SetWindow({ 0, 0, TraceSizeFactor * GetScreenWidth(), (float)GetScreenHeight() });
		m_Menu.SetWindow({ m_Signal.GetWindow().x + m_Signal.GetWindow().width, 0, GetScreenWidth() - m_Signal.GetWindow().width, (float)GetScreenHeight()});
		
		// Initial Signal configurations 
		float maxSec = m_Signal.GetNDivX() * m_Signal.SecPerDiv[m_Menu.m_DropDownboxActiveSecPerDiv];
		m_Signal.SetMaxSignalSec(maxSec);

		float maxY = m_Signal.GetNDivY() * m_Signal.GainPerDiv[m_Menu.m_DropDownboxActiveGainPerDiv];
		m_Signal.SetYFactor(maxY);

		m_Signal.SetNDivX(m_Signal.Graticule[m_Menu.m_DropDownboxActiveGraticuleX]);
		m_Signal.SetNDivY(m_Signal.Graticule[m_Menu.m_DropDownboxActiveGraticuleY]);

		// Camera
		camera.target = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
		camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
		camera.rotation = 0.0f;
		camera.zoom = 1.0f;

		return true;
	}

	bool OnDestroy() override {

		return true;
	}

	bool OnUpdate() override
	{
		m_Signal.SetWindow({ 0, 0, TraceSizeFactor * GetScreenWidth(), (float)GetScreenHeight() });
		m_Menu.SetWindow({ m_Signal.GetWindow().x + m_Signal.GetWindow().width, 0, GetScreenWidth() - m_Signal.GetWindow().width, (float)GetScreenHeight()});


		camera.zoom += ((float)GetMouseWheelMove() * 0.05f);
		
		if (camera.zoom > 3.0f) camera.zoom = 3.0f;
		else if (camera.zoom < 1.0f) camera.zoom = 1.0f;

		return true;
	}

	bool OnDraw() override
	{
		BeginMode2D(camera);
			DrawSignal();
		EndMode2D();
		
		DrawMenu();
		
		//mouse [seconds, voltage]
		Vector2 mousePos = GetMousePosition();
		std::string xMousePos = std::to_string(m_Signal.GetXSignal(mousePos.x) / m_Signal.GetFs());
		std::string yMousePos = std::to_string(m_Signal.GetYSignal(mousePos.y));
		std::string text = "[x= " + xMousePos + "ms, y=" + yMousePos + "uV]";
		DrawText(text.c_str(), 10, 10, 20, RED);
		
		return true;
	}

	void DrawSignalBackground()
	{
		DrawRectangleRec({ m_Signal.GetWindow().x, m_Signal.GetWindow().y, m_Signal.GetWindow().width, m_Signal.GetWindow().height}, DARKGRAY);

		for (float x = m_Signal.GetWindow().x; x <= m_Signal.GetWindow().x + m_Signal.GetWindow().width; x += m_Signal.GetStepDivX())
			DrawLineEx({x, m_Signal.GetWindow().y }, {x, m_Signal.GetWindow().y + m_Signal.GetWindow().height }, 1.0f, GRAY);

		for (float y = m_Signal.GetWindow().y; y <= m_Signal.GetWindow().y + m_Signal.GetWindow().height; y += m_Signal.GetStepDivY())
			DrawLineEx({ m_Signal.GetWindow().x, y }, { m_Signal.GetWindow().x + m_Signal.GetWindow().width, y}, 1.0f, GRAY);
	}

	void DrawSignal()
	{
		DrawSignalBackground();

		// generate a signal and plot
		Vector2 prevPos = { (float)m_Signal.GetXScreen(0), m_Signal.GetYScreen(m_SignalGenerator.GetValue(0)) };

		for (int i = 0; i < m_Signal.GetNPoints(); ++i)
		{
			Vector2 pos = { (float)m_Signal.GetXScreen(i), m_Signal.GetYScreen(m_SignalGenerator.GetValue(i / m_Signal.GetFs())) };
			DrawLineV(prevPos, pos, RED);
			prevPos = pos;
		}
		// //
	}

	void DrawMenu()
	{
		DrawRectangleRec({ m_Menu.GetWindow().x, m_Menu.GetWindow().y, m_Menu.GetWindow().width, m_Menu.GetWindow().height }, GRAY); //background

		float stepY = -25.0f;
		float startY = m_Menu.GetWindow().y + m_Menu.GetWindow().height;
		float currentY = startY + stepY;

		// Start Signal Settings

		float paddingLeft = 60.0f;
		float paddingRight = 50.0f;
		float paddingTopText = 5.0f;

		float itemX = m_Menu.GetWindow().x + paddingLeft;
		float itemWidth = m_Menu.GetWindow().width - paddingRight - paddingLeft;
		float itemHeight = 20.0f;
		
		float fontSize = 5.0f;

		float xPosText = m_Menu.GetWindow().x + paddingLeft + itemWidth + 2;

		// Frequency
		m_SignalGenerator.Frequency = GuiSlider({ itemX, currentY, itemWidth, itemHeight },
			"Frequency", nullptr, m_SignalGenerator.Frequency, 0.0f, 10.0f);
		DrawText(std::to_string(m_SignalGenerator.Frequency).c_str(), xPosText, currentY + paddingTopText, fontSize, RED);
		currentY += stepY;

		// Phase
		m_SignalGenerator.Phase = GuiSlider({ itemX, currentY, itemWidth, itemHeight },
			"Phase", nullptr, m_SignalGenerator.Phase, 0.0f, 10.0f);
		DrawText(std::to_string(m_SignalGenerator.Phase).c_str(), xPosText, currentY + paddingTopText, fontSize, RED);
		currentY += stepY;

		// Amplitude
		m_SignalGenerator.Amplitude = GuiSlider({ itemX, currentY, itemWidth, itemHeight },
			"Amplitude", nullptr, m_SignalGenerator.Amplitude, 0.0f, 2.5f);
		DrawText(std::to_string(m_SignalGenerator.Amplitude).c_str(), xPosText, currentY + paddingTopText, fontSize, RED);
		currentY += stepY;

		// Offset
		m_SignalGenerator.Offset = GuiSlider({ itemX, currentY, itemWidth, itemHeight },
			"Offset", nullptr, m_SignalGenerator.Offset, -5.0f, 5.0f);
		DrawText(std::to_string(m_SignalGenerator.Offset).c_str(), xPosText, currentY + paddingTopText, fontSize, RED);
		currentY += stepY;

		// Invert
		m_SignalGenerator.Invert = GuiToggle({ itemX, currentY, itemWidth, itemHeight }, "Invert", m_SignalGenerator.Invert);
		currentY += stepY;

		// Choose Wave format
		if (GuiDropdownBox({ itemX, currentY, itemWidth, itemHeight },
			SignalTypes.c_str(), &m_Menu.m_DropDownboxActiveSignalType, m_Menu.m_DropDownboxEditModeSignalType))
			m_Menu.m_DropDownboxEditModeSignalType = !m_Menu.m_DropDownboxEditModeSignalType;
		m_SignalGenerator.SignalType = static_cast<SignalType>(m_Menu.m_DropDownboxActiveSignalType);
		currentY += stepY;

		// Signal title
		GuiGroupBox({ m_Menu.GetWindow().x, currentY, m_Menu.GetWindow().x + itemWidth + paddingRight / 2.0f, startY - currentY}, "Function Generator");
		currentY += 2 * stepY;
		
		// End Signal Settings
		startY = currentY - 2 * stepY;

		// Plot settings title - TODO: Improve position
		GuiGroupBox({ m_Menu.GetWindow().x, 30, m_Menu.GetWindow().x + itemWidth + paddingRight / 2.0f, m_Menu.GetWindow().height / 2.0f}, "Trace");
		currentY += 6 * stepY;
		
		// Choose SecPerDiv
		if (GuiDropdownBox({ itemX, currentY, itemWidth, itemHeight },
			SecPerDiv.c_str(), &m_Menu.m_DropDownboxActiveSecPerDiv, m_Menu.m_DropDownboxEditModeSecPerDiv))
		{
			m_Menu.m_DropDownboxEditModeSecPerDiv = !m_Menu.m_DropDownboxEditModeSecPerDiv;
			
			float maxSec = m_Signal.GetNDivX() * m_Signal.SecPerDiv[m_Menu.m_DropDownboxActiveSecPerDiv];
			m_Signal.SetMaxSignalSec(maxSec);
		}
		currentY += stepY;

		// Choose GainPerDiv
		if (GuiDropdownBox({ itemX, currentY, itemWidth, itemHeight },
			GainPerDiv.c_str(), &m_Menu.m_DropDownboxActiveGainPerDiv, m_Menu.m_DropDownboxEditModeGainPerDiv))
		{
			m_Menu.m_DropDownboxEditModeGainPerDiv = !m_Menu.m_DropDownboxEditModeGainPerDiv;

			float maxY = m_Signal.GetNDivY() * m_Signal.GainPerDiv[m_Menu.m_DropDownboxActiveGainPerDiv];
			m_Signal.SetYFactor(maxY);
		}
		currentY += stepY;

		// Choose Graticule X
		if (GuiDropdownBox({ itemX, currentY, itemWidth, itemHeight },
			Graticule.c_str(), &m_Menu.m_DropDownboxActiveGraticuleX, m_Menu.m_DropDownboxEditModeGraticuleX))
		{
			m_Menu.m_DropDownboxEditModeGraticuleX = !m_Menu.m_DropDownboxEditModeGraticuleX;
			m_Signal.SetNDivX(m_Signal.Graticule[m_Menu.m_DropDownboxActiveGraticuleX]);
		}
		currentY += stepY;

		// Choose Graticule Y
		if (GuiDropdownBox({ itemX, currentY, itemWidth, itemHeight },
			Graticule.c_str(), &m_Menu.m_DropDownboxActiveGraticuleY, m_Menu.m_DropDownboxEditModeGraticuleY))
		{
			m_Menu.m_DropDownboxEditModeGraticuleY = !m_Menu.m_DropDownboxEditModeGraticuleY;
			m_Signal.SetNDivY(m_Signal.Graticule[m_Menu.m_DropDownboxActiveGraticuleY]);
		}
		currentY += stepY;

		// Choose HiCut - TODO: Add filters
		if (GuiDropdownBox({ itemX, currentY, itemWidth, itemHeight },
			HiCut.c_str(), &m_Menu.m_DropDownboxActiveHiCut, m_Menu.m_DropDownboxEditModeHiCut))
		{
			m_Menu.m_DropDownboxEditModeHiCut = !m_Menu.m_DropDownboxEditModeHiCut;
			// m_Signal. = ;
		}
		currentY += stepY;

		// Choose LoCut
		if (GuiDropdownBox({ itemX, currentY, itemWidth, itemHeight },
			LoCut.c_str(), &m_Menu.m_DropDownboxActiveLoCut, m_Menu.m_DropDownboxEditModeLoCut))
		{
			m_Menu.m_DropDownboxEditModeLoCut = !m_Menu.m_DropDownboxEditModeLoCut;
			// m_Signal. = ;
		}
		currentY += stepY;

		// Choose Notch
		if (GuiDropdownBox({ itemX, currentY, itemWidth, itemHeight },
			Notch.c_str(), &m_Menu.m_DropDownboxActiveNotch, m_Menu.m_DropDownboxEditModeNotch))
		{
			m_Menu.m_DropDownboxEditModeNotch = !m_Menu.m_DropDownboxEditModeNotch;
			// m_Signal. = ;
		}
		currentY += stepY;

	}

private:
	Menu m_Menu;
	Trace m_Signal;
	SignalGenerator m_SignalGenerator;

	Camera2D camera = { 0 };
};