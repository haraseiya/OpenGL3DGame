#pragma once
#include <iostream>
#include <unordered_map>
#include <SDL/SDL_ttf.h>
#include "Math.h"

class Game;
class Texture;

class Font
{
public:
	Font(Game* _game);
	~Font();

	// ファイルのロード・アンロード
	bool Load(const std::string& _fileName);
	void UnLoad();

	// 文字列をテクスチャに描画
	Texture* RenderText(const std::string& _text,
						const Vector3& _color = Color::White,
						int _pointSize = 72);

private:
	// ポイントサイズとフォントデータの連想配列
	std::unordered_map<int, TTF_Font*> m_fontData;
	Game* m_game;
};

