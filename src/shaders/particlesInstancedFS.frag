#version 330

//!< in-variables
in vec4 passPosition;
//in float passLifeBuffer;

//!< uniforms

//!< out-variables
out vec4 fragcolor;

void main()
{
//   fragcolor =passPosition;
   fragcolor = vec4(1.0f,0.0f,0.0f,1.0f);
}