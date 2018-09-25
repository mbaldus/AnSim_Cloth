#version 330

 //!< in-variables
layout(location = 0) in vec2 position;
layout(location = 1) in vec4 offset; //translation
//layout(location = 1) in float lifeBuffer;

//!< out-variables
out vec4 passPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main()
{
    //apply particle offset (translation) to modelmatrix
    mat4 offset_model = mat4(vec4(model[0]), vec4(model[1]), vec4(model[2]), offset);

	gl_Position = projection * view * offset_model * vec4(position, 0.0f,1.0f);
	passPosition = offset_model* vec4(position, 0.0f,1.0f);

//	passLifeBuffer = lifeBuffer;
//	isAlive = alive;
}