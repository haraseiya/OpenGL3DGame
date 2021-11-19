#pragma once

#define UI_MANAGER UIManager::GetInstance();

#include <vector>


class UIScreen;

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
	~UIManager();

	std::vector<UIScreen*> mUIScreenStack;

	static UIManager* mManager;
};
