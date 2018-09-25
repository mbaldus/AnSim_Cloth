#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>

#include <GL/glew.h>
#include <GL/glcorearb.h>
#include <ownGL/buffer.h>

#include <Util/util.h>

#include <glm/glm.hpp>

enum class RenderingType
{
	glpoints,
	instancing
};

class ParticleSystem
{
	public:
		//default constructor
		ParticleSystem(float delta, unsigned int num, RenderingType renderingType = RenderingType::glpoints);
		//default destructor
		~ParticleSystem();

        void execute();

        //getter
        RenderingType getRenderingType();

		//setter
		void setLifeDeduction(float f);

	private:

        RenderingType m_renderingType;

        std::vector<glm::vec4> m_posInit;		//want to have the start points reseting particles
        std::vector<glm::vec4> m_veloInit;

        float dt;
        float lifeDeduction;
        unsigned int m_numParticles; //number of particles

        GLuint m_VAO; //VAO

        GLuint pos_vbo; //position VBO
        GLuint life_vbo; //life VBO
        GLuint alive_vbo; //particle vision

        GLuint quad_vbo;   //quad for instancing

        std::vector<glm::vec4> pos;
        std::vector<glm::vec4> vel;
        std::vector<float> life;
        std::vector<float> isAlive;
        std::vector<int> aliveHelper;
        std::vector<float> mass;

        void initParticleValues();

        void createBuffers();

        //particle value calculation
        void integration();

        //update data and push it to the gpu
        void updateBuffers();

        //render function
        void render();

        void checkGLError(std::string when);
};

#endif //PARTICLESYSTEM_H