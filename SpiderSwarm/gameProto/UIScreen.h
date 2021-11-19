#pragma once

#include <stdint.h>
#include "Math.h"

class Game;
class Shader;
class Texture;
class Font;

class UIScreen
{
public:
	enum UIState
	{
		ACTIVE=0,	// アクティブ状態
		CLOSING		// クロージング状態
	};

	UIScreen();
	virtual ~UIScreen();

	virtual void Update(float deltaTime);
	virtual void Draw(Shader* shader);
	virtual void ProcessInput(const uint8_t* keys);
	virtual void HandleKeyPress(int key);

	void Close();

	UIState GetState()const { return mState; }

protected:
	void DrawTexture(
		Shader* shader, Texture* texture,
		const Vector2& offset = Vector2::Zero, float scale = 1.0f);

	Font* mFont;
	Texture* mTexture;

	Vector2 mPos;

	UIState mState;

};

