#pragma once
#include "NPCState.h"

class NPCBehaviorComponent;

class NPCIdle : public NPCState
{
public:
	NPCIdle(NPCBehaviorComponent* owner);
	~NPCIdle();

	NPCStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;

};