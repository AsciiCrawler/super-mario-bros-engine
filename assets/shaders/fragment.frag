#version 330 core
out vec4 fragColor;

in vec2 TexCoord;
uniform sampler2D ourTexture;

void main()
{
    vec4 tex = texture(ourTexture, TexCoord);
	if (tex.a < 1.0) {
        discard;
    }

	fragColor = tex;
} 