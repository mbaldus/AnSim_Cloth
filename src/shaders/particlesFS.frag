#version 330

//!< in-variables
in vec4 passPosition;
in float passLifeBuffer;
//in float passDensityBuffer;
//in float isAlive;

//!< uniforms
uniform bool sphereColor;

//!< out-variables
out vec4 fragcolor;

void main()
{
//	fragcolor.w *= passLifeBuffer;
//	fragcolor.w = 0;

	fragcolor = vec4(1,0,0.1,passLifeBuffer);
	if(passLifeBuffer <0.05)
	    discard;
//	if (isAlive > 0.5)
//	{
//	fragcolor.w *= 0.2*passLifeBuffer; //0.2 * [0,1] -> [0,0.2]
//		fragcolor.xyz -= 0.375*passDensityBuffer;
//	}
//	else if (isAlive < 0.5)
//	{
//	fragcolor.w = 0;
//	}
		
}