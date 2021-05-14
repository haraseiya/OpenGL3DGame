#include "Font.h"
#include "Texture.h"
#include "Game.h"

Font::Font(Game* _game)
	: m_game(_game)
{
}

Font::~Font()
{
}

bool Font::Load(const std::string& _fileName)
{
	// サポートするフォントサイズ
	std::vector<int> fontSizes =
	{
		8,9,
		10,11,12,14,16,18,
		20,22,24,26,28,
		30,32,34,36,38,
		40,42,44,46,48,
		52,56,
		60,64,68,
		72
	};

	// サイズごとにTTF_OpenFontを1回ずつ呼び出す
	for (auto& size : fontSizes)
	{
		TTF_Font* font = TTF_OpenFont(_fileName.c_str(), size);
		if (font == nullptr)
		{
			SDL_Log("フォント%sサイズ%dのロードの失敗しました",
				_fileName.c_str(), size);
			return false;
		}
		m_fontData.emplace(size, font);
	}
	return true;
}

void Font::UnLoad()
{
	for (auto& font : m_fontData)
	{
		TTF_CloseFont(font.second);
	}
}

Texture* Font::RenderText(const std::string& _text, const Vector3& _color, int _pointSize)
{
	Texture* texture = nullptr;

	// 色をSDL_Colorに登録する
	SDL_Color sdlColor;
	sdlColor.r = static_cast<Uint8>(_color.x * 255);
	sdlColor.g = static_cast<Uint8>(_color.y * 255);
	sdlColor.b = static_cast<Uint8>(_color.z * 255);
	sdlColor.a = 255;

	// 指定サイズのフォントデータを探す
	auto iter = m_fontData.find(_pointSize);
	if (iter != m_fontData.end())
	{
		TTF_Font* font = iter->second;

		// SDL_Surfaceに描画（アルファブレンディングする）
		SDL_Surface* surf = TTF_RenderText_Blended(font, _text.c_str(),sdlColor);

		if (surf != nullptr)
		{
			// SDL_Surfaceからテクスチャに変換する
			texture = new Texture();
			texture->CreateFromSurface(surf);
			SDL_FreeSurface(surf);
		}
	}
	else
	{
		SDL_Log("ポイントサイズ%dが未対応です", _pointSize);
	}

	return texture;
}
