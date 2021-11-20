#pragma once
#include <vector>

class UIScreen;
class HUD;

class UIManager
{
public:
	static UIManager* GetInstance() { return mManager; }

	// UIマネージャーインスタンスを生成・破棄
	static void CreateInstance();	
	static void DeleteInstance();

	void Update(float deltaTime);	// UIスクリーンを更新
	void AddUI(UIScreen* screen);	// UIスクリーンを追加

	// 全てのUIスクリーンの参照を返す
	const std::vector<UIScreen*>& GetUIScreenStack() { return mUIScreenStack; }

private:
	UIManager();
	~UIManager() {}

	std::vector<UIScreen*> mUIScreenStack;	// UIスクリーンを格納する配列

	static UIManager* mManager;
};

#define UI_MANAGER UIManager::GetInstance();