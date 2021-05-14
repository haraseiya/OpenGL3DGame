#pragma once

#include <vector>
#include <string>

class Shader;
class Texture;
class CameraActor;

class CubeMap
{
public:
	CubeMap();
	~CubeMap();

	unsigned int LoadCubeMap(std::vector<std::string>_faces);
	void Draw();

private:
	Texture* m_texture;
	CameraActor* m_camera;

	Shader* m_cubemapShader;
	Shader* m_skyboxShader;	

	std::vector<std::string> m_faces;

	unsigned int m_textureID;
	unsigned char* m_data;

	unsigned int m_cubeVAO;			// キューブマップ頂点配列
	unsigned int m_cubeVBO;			// キューブマップ
	unsigned int m_cubeTexture;		
	unsigned int m_cubeMapTexture;	// キューブマップテクスチャ

	// スカイボックス用
	unsigned int m_skyboxVAO;		// キューブマップ頂点配列
	unsigned int m_skyboxVBO;

	int m_width;		// 幅
	int m_height;		// 高さ
	int m_nrChannel;	// チャンネル
};

