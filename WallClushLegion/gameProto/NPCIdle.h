#pragma once
#include "NPCState.h"

class NPCBehaviorComponent;
class Player;

class NPCIdle : public NPCState
{
public:
	NPCIdle(NPCBehaviorComponent* owner,Player* player);
	~NPCIdle();

	NPCStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;

private:
	Player* mPlayer;
};