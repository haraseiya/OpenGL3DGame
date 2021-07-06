#include "CubeMap.h"
#include "GL/glew.h"
#include "Shader.h"
#include "Texture.h"
#include "Math.h"
#include "Game.h"
#include "Renderer.h"
#include "CameraActor.h"
#include "Game.h"

#include <SDL/SDL_image.h>
#include <iostream>

CubeMap::CubeMap()
{
    // �L���[�u�}�b�v�p�̃V�F�[�_�[
    m_cubemapShader = new Shader();
    m_skyboxShader = new Shader();

    // �V�F�[�_�[�̃��[�h
    m_cubemapShader->Load("Shaders/CubeMap.vert", "Shaders/CubeMap.frag");
    m_skyboxShader->Load("Shaders/SkyBox.vert", "Shaders/SkyBox.frag");

    // �L���[�u�p�̒��_�z��
    float cubeVertices[] = {
        // �ʒu               // �e�N�X�`��
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // �X�J�C�{�b�N�X�p�̒��_�z��
    float skyboxVertices[] = {
        // �ʒu        
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

	// �L���[�u�쐬
    glGenVertexArrays(1, &m_cubeVAO);
    glGenBuffers(1, &m_cubeVBO);
    glBindVertexArray(m_cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // �X�J�C�{�b�N�X�쐬
    glGenVertexArrays(1, &m_skyboxVAO);
    glGenBuffers(1, &m_skyboxVBO);
    glBindVertexArray(m_skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // �S���ʂ̃e�N�X�`���p�X���i�[
    m_faces.push_back("assets/SkyBox/right.png");
    m_faces.push_back("assets/SkyBox/left.png");
    m_faces.push_back("assets/SkyBox/top.png");
    m_faces.push_back("assets/SkyBox/bottom.png");
    m_faces.push_back("assets/SkyBox/front.png");
    m_faces.push_back("assets/SkyBox/back.png");

    m_cubeMapTexture = LoadCubeMap(m_faces);

    m_cubemapShader->SetActive();
    m_cubemapShader->SetIntUniform("texture1", 0);

    m_skyboxShader->SetActive();
    m_skyboxShader->SetIntUniform("skybox", 0);
}

CubeMap::~CubeMap()
{
    glDeleteVertexArrays(1, &m_cubeVAO);
    glDeleteVertexArrays(1, &m_skyboxVAO);
    glDeleteBuffers(1, &m_cubeVBO);
    glDeleteBuffers(1, &m_skyboxVAO);
}

void CubeMap::Draw()
{
    //Matrix4 model=Matrix4::CreateScale(1.0f,1.0f,1.0f);
    Matrix4 view = RENDERER->GetViewMatrix();
    Matrix4 projection = RENDERER->GetProjectionMatrix();
    //m_cubemapShader->SetMatrixUniform("model",model);
    //m_cubemapShader->SetMatrixUniform("view", view);
    //m_cubemapShader->SetMatrixUniform("projection", projection);

    // �L���[�u�`��
    //m_cubemapShader->SetActive();
    //glBindVertexArray(m_cubeVAO);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, m_cubeTexture);
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    //glBindVertexArray(0);

    // �X�J�C�{�b�N�X�`��
    glDepthFunc(GL_LEQUAL);
    m_skyboxShader->SetActive();
    m_skyboxShader->SetMatrixUniform("view", view);
    m_skyboxShader->SetMatrixUniform("projection", projection);
    glBindVertexArray(m_skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

// �L���[�u�}�b�v�����[�h
unsigned int CubeMap::LoadCubeMap(std::vector<std::string> _faces)
{
    // �e�N�X�`���𐶐�
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < _faces.size(); i++)
    {
        SDL_Surface* surf = IMG_Load(_faces[i].c_str());
        int width = surf->w;
        int height = surf->h;

        if (surf)
        {
            // �E�A���A��A���A�w�ʁA�O�ʂ̏��Ńe�N�X�`���𒣂�t���Ă���
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, surf->pixels);
            SDL_FreeSurface(surf);
        }
        else
        {
            std::cout << "�L���[�u�}�b�v�e�N�X�`���̃��[�h�Ɏ��s���܂���" << std::endl;
            SDL_FreeSurface(surf);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return m_textureID;
}
