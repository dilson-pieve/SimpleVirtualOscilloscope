#pragma once

#include <math.h> // sin
#include <cmath>  // abs
#include <string>
#include <unordered_map>

constexpr static double M_PI = 3.14159265358979323846;

enum class SignalType
{
    //Remember to change the string SignalTypes
	Sine = 0,
	Square,
	Triangle
};

const std::string SignalTypes = "Sine;Square;Triangle";

class SignalGenerator 
{

public:

	SignalType SignalType = SignalType::Square;
	float Frequency = 1.0f;
	float Phase = 0.0f;
	float Amplitude = 1.0f;
	float Offset = 0.0f;
    bool Invert = false;
    
	float GetValue(float time)
	{
        float value = 0.0f;
        
        float t = Frequency * time + Phase;

        switch (SignalType)
        {
        case SignalType::Sine: 
                value = (float)sin(2.0f * M_PI * t);
                break;
        case SignalType::Square: 
                value = sin(2.0f * M_PI * t) < 0 ? -1 : 1;

                /* Creates a square wave using a low pass filter
                value = sin(2.0f * M_PI * t) < 0 ? -1 : 1;
                value = value * 0.04f + lastValue * (1.0f - 0.04f); // a=0.04f
                lastValue = value;*/

                break;
        case SignalType::Triangle:
                value = 1.0f - 4.0f * (float)std::abs(round(t - 0.25f) - (t - 0.25f));
                break;
        }

        return (Invert ? -1.0f : 1.0f) * Amplitude * value + Offset;
	}
};