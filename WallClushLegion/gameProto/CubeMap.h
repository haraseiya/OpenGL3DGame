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

	unsigned int m_cubeVAO;			// �L���[�u�}�b�v���_�z��
	unsigned int m_cubeVBO;			// �L���[�u�}�b�v
	unsigned int m_cubeTexture;		
	unsigned int m_cubeMapTexture;	// �L���[�u�}�b�v�e�N�X�`��

	// �X�J�C�{�b�N�X�p
	unsigned int m_skyboxVAO;		// �L���[�u�}�b�v���_�z��
	unsigned int m_skyboxVBO;

	int m_width;		// ��
	int m_height;		// ����
	int m_nrChannel;	// �`�����l��
};

