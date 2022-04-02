#pragma once

struct WindowProps
{
	float x;                // top-left corner position x
	float y;                // top-left corner position y
	float width;            // width
	float height;           // height
};

class BaseWindow
{
public:
	virtual ~BaseWindow() {}

	// Getters and Setters
	virtual void SetWindow(const WindowProps& window) { m_Window = window; }
	virtual const WindowProps& GetWindow() const { return m_Window; }

private:
	WindowProps m_Window;
};