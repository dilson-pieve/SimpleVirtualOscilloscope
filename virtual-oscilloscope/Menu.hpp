#pragma once

#include "BaseWindow.hpp"

class Menu : public BaseWindow
{
public:
	// Signal Type
	bool m_DropDownboxEditModeSignalType = false;
	int m_DropDownboxActiveSignalType = 0;

	// SecPerDiv
	bool m_DropDownboxEditModeSecPerDiv = false;
	int m_DropDownboxActiveSecPerDiv = 0;

	// GainPerDiv
	bool m_DropDownboxEditModeGainPerDiv = false;
	int m_DropDownboxActiveGainPerDiv = 0;

	// Graticule X
	bool m_DropDownboxEditModeGraticuleX = false;
	int m_DropDownboxActiveGraticuleX = 0;

	// Graticule Y
	bool m_DropDownboxEditModeGraticuleY = false;
	int m_DropDownboxActiveGraticuleY = 0;

	// HiCut 
	bool m_DropDownboxEditModeHiCut = false;
	int m_DropDownboxActiveHiCut = 0;

	// LoCut 
	bool m_DropDownboxEditModeLoCut = false;
	int m_DropDownboxActiveLoCut = 0;

	// Notch
	bool m_DropDownboxEditModeNotch = false;
	int m_DropDownboxActiveNotch = 0;
};
