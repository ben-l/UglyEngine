#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TilingFactor;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

// Outputs
out vec4 o_FragColor;

// Main Function
void main () {
  vec4 l_Texture;
  int l_TexIndex = int(v_TexIndex);

  switch (l_TexIndex) {
    case 0:  l_Texture = texture(u_Textures[0],  v_TexCoord * v_TilingFactor) * v_Color; break;
    case 1:  l_Texture = texture(u_Textures[1],  v_TexCoord * v_TilingFactor) * v_Color; break;
    case 2:  l_Texture = texture(u_Textures[2],  v_TexCoord * v_TilingFactor) * v_Color; break;
    case 3:  l_Texture = texture(u_Textures[3],  v_TexCoord * v_TilingFactor) * v_Color; break;
    case 4:  l_Texture = texture(u_Textures[4],  v_TexCoord * v_TilingFactor) * v_Color; break;
    case 5:  l_Texture = texture(u_Textures[5],  v_TexCoord * v_TilingFactor) * v_Color; break;
    case 6:  l_Texture = texture(u_Textures[6],  v_TexCoord * v_TilingFactor) * v_Color; break;
    case 7:  l_Texture = texture(u_Textures[7],  v_TexCoord * v_TilingFactor) * v_Color; break;
    case 8:  l_Texture = texture(u_Textures[8],  v_TexCoord * v_TilingFactor) * v_Color; break;
    case 9:  l_Texture = texture(u_Textures[9],  v_TexCoord * v_TilingFactor) * v_Color; break;
    case 10: l_Texture = texture(u_Textures[10], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 11: l_Texture = texture(u_Textures[11], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 12: l_Texture = texture(u_Textures[12], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 13: l_Texture = texture(u_Textures[13], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 14: l_Texture = texture(u_Textures[14], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 15: l_Texture = texture(u_Textures[15], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 16: l_Texture = texture(u_Textures[16], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 17: l_Texture = texture(u_Textures[17], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 18: l_Texture = texture(u_Textures[18], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 19: l_Texture = texture(u_Textures[19], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 20: l_Texture = texture(u_Textures[20], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 21: l_Texture = texture(u_Textures[21], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 22: l_Texture = texture(u_Textures[22], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 23: l_Texture = texture(u_Textures[23], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 24: l_Texture = texture(u_Textures[24], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 25: l_Texture = texture(u_Textures[25], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 26: l_Texture = texture(u_Textures[26], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 27: l_Texture = texture(u_Textures[27], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 28: l_Texture = texture(u_Textures[28], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 29: l_Texture = texture(u_Textures[29], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 30: l_Texture = texture(u_Textures[30], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 31: l_Texture = texture(u_Textures[31], v_TexCoord * v_TilingFactor) * v_Color; break;
  }

  o_FragColor = l_Texture * v_Color;

  color2 = 50;
}
