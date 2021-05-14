#include "BitMapText.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"

const int maxImageNum = 256;

BitMapText::BitMapText()
	: mUseflag(false)
	, mSplitXnum(0)
	, mSplitYnum(0)
	, mSizeX(0)
	, mSizeY(0)
	, mTexture(nullptr)
{
	for (int i = 0; i < maxImageNum; i++)
	{
		mFontMap[i] = -1;
	}
}

BitMapText::~BitMapText()
{
}

void BitMapText::SetFontImage(int xNum, int yNum, const std::string& fontImgfilename)
{
	mTexture = RENDERER->GetTexture(fontImgfilename);
	if (!mTexture)
	{
		return;
	}
	mSplitXnum = xNum;
	mSplitYnum = yNum;
	mSizeX = mTexture->GetWidth() / mSplitXnum;
	mSizeY = mTexture->GetHeight() / mSplitYnum;

	mUseflag = true;
}

void BitMapText::SetFontColor(int r, int g, int b)
{
}

void BitMapText::TextDraw(int x, int y, const char* str)
{
	//フォント使用可能か
	if (!mUseflag)
	{
		return;
	}

	//テキスト描画開始
	int text_x, text_y, idx;
	text_x = x;
	text_y = y;

	while (*str)
	{
		// 文字が改行の場合は改行＋戻す
		if (*str == '\n')
		{
			text_x = x;
			text_y += mSizeY;
			str++;
			continue;
		}
		//文字描画
		idx = GetIndex(*str);
		if (idx >= 0)
		{
			RENDERER->DrawTexture(mTexture,
				idx,
				mSplitXnum,
				mSplitYnum,
				Vector2(static_cast<float>(text_x), static_cast<float>(text_y)));
		}
		// 文字描画位置進める
		text_x += mSizeX;
		str++;
	}
}

// ReMapText　画像内に表示されている文字列を引数に入れ
// 描画位置を割り出すためのインデックステーブルを作成する関数
void BitMapText::ReMapText(const char* remapText)
{
	int num = 0;
	const char* p;

	// 文字コード→画像マップのインデックスに変換するテーブルを作る
	// RemapTextが"ABC"なら
	// mFontMap['A'] = 0;mFontMap['B'] = 1;  ・・・となる
	p = remapText;
	char n = 0;
	while (*p && n <= 255)
	{
		mFontMap[*p++] = n++;
	}
}

int BitMapText::GetIndex(char c)
{
	return mFontMap[c];
}
