#include "ObjectPool.h"
#include "GameObject.h"

ObjectPool::ObjectPool()
	: mObject(nullptr)
	, mSize(0)
{
}


ObjectPool::~ObjectPool()
{
	Clear();
}

void ObjectPool::Fill(GameObject* object, int size)
{
	for (int i = 0; i < size; i++)
	{
		std::unique_ptr<GameObject> obj = std::make_unique<GameObject>(*object);
		mObjectPool.push_back(std::move(obj));
	}
}

std::unique_ptr<GameObject> ObjectPool::GetGameObject()
{
	// �I�u�W�F�N�g�v�[������Ȃ��
	if (!mObjectPool.empty())
	{
		// �z��̐擪�ɃQ�[���I�u�W�F�N�g���ړ�����
		std::unique_ptr<GameObject> obj = std::move(mObjectPool.front());

		// �擪�Ɉړ������I�u�W�F�N�g�����o��
		mObjectPool.pop_front();
		return obj;
	}
}

void ObjectPool::Clear()
{
	mObjectPool.clear();
}

void ObjectPool::ReturnToPool(std::unique_ptr<GameObject> returningObject)
{
	mObjectPool.emplace_back(std::move(returningObject));
}

