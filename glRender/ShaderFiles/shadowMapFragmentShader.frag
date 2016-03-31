#version 430 core

in vec2 fragTexCoord;
uniform sampler2D shadowMapTexture;
 
out vec4 fragColor;

void main()
{
	float depth = texture2D(shadowMapTexture, fragTexCoord).x;
	depth = 1.0 - (1.0 - depth) * 25.0;
	fragColor = vec4(depth);
}