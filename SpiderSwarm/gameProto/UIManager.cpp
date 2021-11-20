#include "UIManager.h"
#include "UIScreen.h"

// UI�}�l�[�W���[�̃V���O���g����������
UIManager* UIManager::mManager = nullptr;

// UI�}�l�[�W���[�C���X�^���X�̐���
void UIManager::CreateInstance()
{
	// UI�}�l�[�W���[�����݂��Ȃ������琶��
	if (!mManager)
	{
		mManager = new UIManager();
	}
}

// UI�}�l�[�W���[�C���X�^���X�̔j��
void UIManager::DeleteInstance()
{
	// �}�l�[�W���[�����݂�����폜
	if (mManager)
	{
		delete mManager;
		mManager = nullptr;
	}
}

// UI�̍X�V����
void UIManager::Update(float deltaTime)
{
	// �A�N�e�B�u��Ԃ�UI��S�čX�V
	for (auto ui : mUIScreenStack)
	{
		if (ui->GetState() == UIScreen::ACTIVE)
		{
			ui->Update(deltaTime);
		}
	}

	// ���Ă����Ԃ�UI��S�Ĕj��
	for (auto ui : mUIScreenStack)
	{
		if (ui->GetState() == UIScreen::UIScreen::CLOSING)
		{
			delete ui;
		}
	}
}

// UI�X�N���[����z��ɓo�^
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
