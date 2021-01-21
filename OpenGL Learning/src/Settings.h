#pragma once

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;

const unsigned int SHADOW_MAP_WIDTH = 1024;
const unsigned int SHADOW_MAP_HEIGHT = 1024;

class Settings
{
public:
	static Settings& GetInstance()
	{
		static Settings inst;
		return inst;
	}
	int screenWidth = SCREEN_WIDTH;
	int screenHeight = SCREEN_HEIGHT;

	int shadowMapWidth = SHADOW_MAP_WIDTH;
	int shadowMapHeight = SHADOW_MAP_HEIGHT;

private:
	Settings() { }
	Settings(Settings& other) = delete;
	void operator=(const Settings&) = delete;
};