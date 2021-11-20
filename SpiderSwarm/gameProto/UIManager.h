#pragma once
#include <vector>

class UIScreen;
class HUD;

class UIManager
{
public:
	static UIManager* GetInstance() { return mManager; }

	// UI�}�l�[�W���[�C���X�^���X�𐶐��E�j��
	static void CreateInstance();	
	static void DeleteInstance();

	void Update(float deltaTime);	// UI�X�N���[�����X�V
	void AddUI(UIScreen* screen);	// UI�X�N���[����ǉ�

	// �S�Ă�UI�X�N���[���̎Q�Ƃ�Ԃ�
	const std::vector<UIScreen*>& GetUIScreenStack() { return mUIScreenStack; }

private:
	UIManager();
	~UIManager() {}

	std::vector<UIScreen*> mUIScreenStack;	// UI�X�N���[�����i�[����z��

	static UIManager* mManager;
};

#define UI_MANAGER UIManager::GetInstance();