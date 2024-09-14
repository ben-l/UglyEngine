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
    v_TilingFactor= a_TilingFactor;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

// Inputs
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

// Uniforms
uniform sampler2D u_TextureSlots[16];

// Outputs
out vec4 o_FragColor;

// Main Function
void main () {
  vec4 l_Texture;
  int l_TexIndex = int(v_TexIndex);

  switch (l_TexIndex) {
    case 0:  l_Texture = texture(u_TextureSlots[0],  v_TexCoord); break;
    case 1:  l_Texture = texture(u_TextureSlots[1],  v_TexCoord); break;
    case 2:  l_Texture = texture(u_TextureSlots[2],  v_TexCoord); break;
    case 3:  l_Texture = texture(u_TextureSlots[3],  v_TexCoord); break;
    case 4:  l_Texture = texture(u_TextureSlots[4],  v_TexCoord); break;
    case 5:  l_Texture = texture(u_TextureSlots[5],  v_TexCoord); break;
    case 6:  l_Texture = texture(u_TextureSlots[6],  v_TexCoord); break;
    case 7:  l_Texture = texture(u_TextureSlots[7],  v_TexCoord); break;
    case 8:  l_Texture = texture(u_TextureSlots[8],  v_TexCoord); break;
    case 9:  l_Texture = texture(u_TextureSlots[9],  v_TexCoord); break;
    case 10: l_Texture = texture(u_TextureSlots[10], v_TexCoord); break;
    case 11: l_Texture = texture(u_TextureSlots[11], v_TexCoord); break;
    case 12: l_Texture = texture(u_TextureSlots[12], v_TexCoord); break;
    case 13: l_Texture = texture(u_TextureSlots[13], v_TexCoord); break;
    case 14: l_Texture = texture(u_TextureSlots[14], v_TexCoord); break;
    case 15: l_Texture = texture(u_TextureSlots[15], v_TexCoord); break;
  }

  o_FragColor = l_Texture * v_Color;
}
