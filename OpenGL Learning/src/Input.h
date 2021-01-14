#pragma once
#include <memory>
#include <unordered_map>
class Input
{
public:
	static Input& GetInstance();

	Input(Input& other) = delete;
	void operator=(const Input&) = delete;
	bool IsKeyPressed(int keycode);

	bool IsKeyReleased(int keycode);
	bool IsKeyDown(int keycode);
	bool IsKeyUp(int keycode);

	bool IsMouseButtonPressed(int button);

	float GetMouseX();
	float GetMouseY();
	float GetMouseDeltaX();
	float GetMouseDeltaY();

private:
	Input() { }
	static Input* m_Instance;
	float lastMouseX = 0;
	float lastMouseY = 0;

	std::unordered_map<int, bool> keysPressState;
};

