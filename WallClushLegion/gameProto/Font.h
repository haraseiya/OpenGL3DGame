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

	// �t�@�C���̃��[�h�E�A�����[�h
	bool Load(const std::string& _fileName);
	void UnLoad();

	// ��������e�N�X�`���ɕ`��
	Texture* RenderText(const std::string& _text,
						const Vector3& _color = Color::White,
						int _pointSize = 72);

private:
	// �|�C���g�T�C�Y�ƃt�H���g�f�[�^�̘A�z�z��
	std::unordered_map<int, TTF_Font*> m_fontData;
	Game* m_game;
};

