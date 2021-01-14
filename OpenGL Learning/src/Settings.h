#pragma once

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;

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

private:
	Settings() { }
	Settings(Settings& other) = delete;
	void operator=(const Settings&) = delete;
};