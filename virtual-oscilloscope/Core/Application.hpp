#pragma once

#include "raygui.h"

class Application
{
public:
	~Application() { CloseWindow(); }

	// Default Full screen
	void Construct(const char* title, 
					int width,
					int height)
	{
		SetConfigFlags(FLAG_WINDOW_RESIZABLE);

		InitWindow(width, height, title);
		SetTargetFPS(60);
	}

	void Start()
	{
		if (OnCreate())
		{
			while (!WindowShouldClose())
			{
				OnUpdate();

				BeginDrawing();
					ClearBackground(RAYWHITE);
					OnDraw();
				EndDrawing();
			
			}
		}
		OnDestroy();
	}
	
	// called once at the start
	virtual bool OnCreate() { return false; }

	// called once application termination
	virtual bool OnDestroy() { return true; }

	// called once per frame
	virtual bool OnUpdate() { return false; }

	// called once per frame to draw
	virtual bool OnDraw() { return false; }
};