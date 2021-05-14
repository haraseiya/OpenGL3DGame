#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
const float offset = 1.0 / 300.0;

void main()
{
	vec2 offsets[9] = vec2[]
    (
        vec2(-offset,  offset), // ����
        vec2( 0.0f,    offset), // ������
        vec2( offset,  offset), // �E��
        vec2(-offset,  0.0f),   // ��
        vec2( 0.0f,    0.0f),   // ����
        vec2( offset,  0.0f),   // �E
        vec2(-offset, -offset), // ����
        vec2( 0.0f,   -offset), // ��
        vec2( offset, -offset)  // �E��    
    );

    float kernel[9] = float[]
    (
         1, 1, 1,
         1,-8, 1,
         1, 1, 1
    );

    vec3 sampleTex[9];
    for(int i = 0;i < 9;i++)
    {
        sampleTex[i] = vec3(texture(screenTexture,TexCoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
        for(int i = 0; i < 9; i++)
    {
        col += sampleTex[i] * kernel[i];
    }

    for(int i = 0;i<9;i++)
    col+=sampleTex[i]*kernel[i];

    FragColor = vec4(1 - col, 1.0);  
}