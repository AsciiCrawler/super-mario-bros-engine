#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
  
uniform mat4 projection;
uniform mat4 view;

// Sprites arrive in world space; this depth sits inside the orthographic near/far range (0.1 - 100)
const float spriteDepth = -5.0;

void main()
{
	gl_Position = projection * vec4(aPos, spriteDepth, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}