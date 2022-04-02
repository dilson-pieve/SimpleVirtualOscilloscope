#pragma once

#include "raygui.h"
#include "BaseWindow.hpp"

class Trace : public BaseWindow
{
public:
	// Pixels
	int GetXScreen(int i) const { return i * GetWindow().width / m_NPoints; }
	float GetYScreen(float voltage) const { return GetWindow().height * (0.5f - (voltage / m_YFactor)); }

	// Seconds 
	int GetXSignal(int xScreen) const { return xScreen * m_NPoints / GetWindow().width; }
	// Voltage
	float GetYSignal(float yScreen) const { return  (((-yScreen + (GetWindow().height / 2.0f)) * m_YFactor)) / GetWindow().height; }

	void RecalculateNPoints() { m_NPoints = m_Fs * m_MaxSec; }
	
	void RecalculateStepAxis() 
	{ 
		m_StepDivX = GetWindow().width / m_NDivX;
		m_StepDivY = GetWindow().height / m_NDivY;
	}
	
	// Getters and setters
	
	void SetMaxSignalSec(float maxSec) { m_MaxSec = maxSec; RecalculateNPoints(); }
	float GetMaxSignalSec() const { return m_MaxSec;  }

	void SetFs(float fs) { m_Fs = fs; RecalculateNPoints(); }
	float GetFs() const { return m_Fs; }

	float GetNPoints() const { return m_NPoints; }

	void SetYFactor(int yFactor) { m_YFactor = yFactor; RecalculateNPoints(); }
	float GetYFactor() const { return m_YFactor; }

	void SetNDivY(int nDivY) { m_NDivY = nDivY;  RecalculateStepAxis(); }
	float GetNDivY() const { return m_NDivY; }

	void SetNDivX(int nDivX) { m_NDivX = nDivX;  RecalculateStepAxis(); }
	float GetNDivX() const { return m_NDivX; }

	float GetStepDivX() const { return m_StepDivX; }
	float GetStepDivY() const { return m_StepDivY; }

	// change outide the class too
	std::vector<int> SecPerDiv = { 1, 5, 10, 15, 20, 30, 50, 100, 500, 1000 };
	std::vector<int> GainPerDiv = { 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10'000 };
	std::vector<int> Graticule = { 5, 10, 15, 20 };

private:
	float m_MaxSec = 10; // seconds screen shows the signal
	float m_Fs = 100; // sampling frequency
	float m_NPoints = m_Fs * m_MaxSec; // number points screen shows = MaxSec * Fs
	float m_YFactor = 10; // number divisions Y axis

	int m_NDivX = 5;
	int m_NDivY = 5;

	float  m_StepDivX = 900.0f/10.0f;
	float  m_StepDivY = 450.0f/8.0f;
};

// change inside the class too
const std::string SecPerDiv = "1 ms/div;5 ms/div;10 ms/div;15 ms/div;20 ms/div;30 ms/div;50 ms/div;100 ms/div;500 ms/div; 1000 ms/div";
const std::string GainPerDiv = "2 uV/div;5 uV/div;10 uV/div;20 uV/div;50 uV/div;100 uV/div;200 uV/div;500 uV/div;1 mV/div;5 mV/div;10 mV/div";

const std::string Graticule = "5;10;15;20";

const std::string HiCut = "30 Hz;50 Hz;100 Hz;200 Hz;300 Hz;500 Hz;1 kHz;1.5 kHz;2 kHz;3 kHz;5 kHz;10 kHz;15 kHz";
const std::string LoCut = "0.3 Hz;1 Hz;2 Hz;5 Hz;10 Hz;20 Hz;30 Hz;100 Hz;150 Hz;500 Hz;1 kHz;2 kHz";

const std::string Notch = "Off;50 Hz;60 Hz";

