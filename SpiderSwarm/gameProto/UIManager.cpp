#include "UIManager.h"
#include "UIScreen.h"

// UIマネージャーのシングルトンを初期化
UIManager* UIManager::mManager = nullptr;

// UIマネージャーインスタンスの生成
void UIManager::CreateInstance()
{
	// UIマネージャーが存在しなかったら生成
	if (!mManager)
	{
		mManager = new UIManager();
	}
}

// UIマネージャーインスタンスの破棄
void UIManager::DeleteInstance()
{
	// マネージャーが存在したら削除
	if (mManager)
	{
		delete mManager;
		mManager = nullptr;
	}
}

// UIの更新処理
void UIManager::Update(float deltaTime)
{
	// アクティブ状態のUIを全て更新
	for (auto ui : mUIScreenStack)
	{
		if (ui->GetState() == UIScreen::ACTIVE)
		{
			ui->Update(deltaTime);
		}
	}

	// 閉じている状態のUIを全て破棄
	for (auto ui : mUIScreenStack)
	{
		if (ui->GetState() == UIScreen::UIScreen::CLOSING)
		{
			delete ui;
		}
	}
}

// UIスクリーンを配列に登録
void UIManager::AddUI(UIScreen* screen)
{
	mUIScreenStack.emplace_back(screen);
}

UIManager::UIManager()
{

}

UIManager::~UIManager()
{

}
