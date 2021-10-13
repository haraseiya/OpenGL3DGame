#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include "Game.h"
#include "Renderer.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "InstanceMeshManager.h"

int main(int argc, char** argv)
{
	if (!GAMEINSTANCE.Initialize(1280, 768, false)) return -1;

	GAMEINSTANCE.GetRenderer()->GetInstanceMeshManager()->
		SetInstanceMesh(GAMEINSTANCE.GetRenderer()->GetMesh("assets/Mesh/FloorBlock.gpmesh"),InstanceType::PlayerBullet1,1000);
	GAMEINSTANCE.GetRenderer()->GetInstanceMeshManager()->
		SetInstanceMesh(GAMEINSTANCE.GetRenderer()->GetMesh("assets/Mesh/Bullet.gpmesh"), InstanceType::EnemyBullet1, 1000);


	GAMEINSTANCE.setFirstScene(new TitleScene);
	GAMEINSTANCE.GetRenderer()->SetWindowTitle("Clash Legion");
	GAMEINSTANCE.Run();
	GAMEINSTANCE.Shutdown();
	return 0;
}