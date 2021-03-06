#version 330

 //!< in-variables
layout(location = 0) in vec4 positionAttribute;
layout(location = 1) in float lifeBuffer;
//layout(location = 2) in float densityBuffer;
//layout(location = 3) in float alive;
//
//!< out-variables

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 passPosition;
out float passLifeBuffer;
//out float passDensityBuffer;
//out float isAlive;

void main()
{
	gl_Position = projection * view * model * positionAttribute;
	passPosition = model * positionAttribute;

	gl_PointSize = (2-lifeBuffer) * 20  / gl_Position.w;

	//passthroughs
	passLifeBuffer = lifeBuffer;
//	isAlive = alive;
}