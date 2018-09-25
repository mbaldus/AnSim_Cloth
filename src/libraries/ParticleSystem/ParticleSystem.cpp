#define PI 3.14159265359

#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(float delta, unsigned int num, RenderingType renderingType)
        : m_numParticles(num), m_renderingType(renderingType), dt(delta), lifeDeduction(0.25)
{
	printf("Number of Particles = %d \n", m_numParticles);

	pos.resize(m_numParticles);
	vel.resize(m_numParticles);
	life.resize(m_numParticles);
	isAlive.resize(m_numParticles);
	aliveHelper.resize(m_numParticles);
	mass.resize(m_numParticles);
	m_posInit.resize(m_numParticles);
	m_veloInit.resize(m_numParticles);

    initParticleValues();
    createBuffers();
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::initParticleValues()
{
    float x,y,z;
    float rand_x,rand_y,rand_z;

    for (int i = 0; i < m_numParticles; i++) {
        x = rand_float(-0.125f, 0.125f);
        z = rand_float(-0.125f, 0.125f);
        y = rand_float(0.125f, 0.25f);
        pos[i] = glm::vec4(0.f, 0.f, 0.f, 1.0f);

        rand_x = rand_float(-0.5f, 0.5f);
        rand_y = rand_float(1.f, 7.f);
        rand_z = rand_float(-0.3f, 0.3f);
        vel[i] = glm::vec4(rand_x, rand_y, rand_z, 0);

        setLifeDeduction(0.45);

        mass[i] =0.0005f;

        m_posInit[i] = pos[i];
        m_veloInit[i] = vel[i];

        life[i] = rand_float(0.0f,1.0f);

        isAlive[i] = 0.0;
        aliveHelper[i] = 0;
        if(i < 500)
        {
            isAlive[i] = 1.0;
            aliveHelper[i] = 1;
        }
    }
}

void ParticleSystem::createBuffers()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    if(m_renderingType == RenderingType::glpoints)
    {
        glGenBuffers(1, &pos_vbo);	//create VBO
        glBindBuffer(GL_ARRAY_BUFFER, pos_vbo); //activate vbo id to use
        glBufferData(GL_ARRAY_BUFFER,  m_numParticles * sizeof(glm::vec4) , &pos[0], GL_STATIC_DRAW); //upload data to video card

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), nullptr);

        glBindBuffer(GL_ARRAY_BUFFER,0);

        glGenBuffers(1, &life_vbo);	//create VBO
        glBindBuffer(GL_ARRAY_BUFFER, life_vbo); //activate vbo id to use
        glBufferData(GL_ARRAY_BUFFER,  m_numParticles * sizeof(float) , &life[0], GL_STATIC_DRAW); //upload data to video card

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);

        glBindBuffer(GL_ARRAY_BUFFER,0);
    }
    if(m_renderingType == RenderingType::instancing)
    {
        std::vector<glm::vec2> quadVertices = {
                glm::vec2(-0.1f, 0.1f),
                glm::vec2(-0.1f, -0.1f),
                glm::vec2(0.1f, -0.1f),

                glm::vec2(-0.1f, 0.1f),
                glm::vec2(0.1f, -0.1f),
                glm::vec2(0.1f, 0.1f)
        };

        //quad vertices Buffer
        glGenBuffers(1, &quad_vbo); //location 0
        glBindBuffer(GL_ARRAY_BUFFER,quad_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * quadVertices.size(), quadVertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT,GL_FALSE, sizeof(glm::vec2),nullptr);

        //position (offset) buffer
        glGenBuffers(1, &pos_vbo); //location 1
        glBindBuffer(GL_ARRAY_BUFFER, pos_vbo); //activate vbo id to use
        glBufferData(GL_ARRAY_BUFFER,  m_numParticles * sizeof(glm::vec4) , &pos[0], GL_STATIC_DRAW); //upload data to video card
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), nullptr);

        glVertexAttribDivisor(1, 1); // Tell OpenGL this is an instanced vertex attribute.

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // finished with the VAO, unbind it
    glBindVertexArray(0);
}

void ParticleSystem::integration()
{
	for(int i = 0; i < m_numParticles; i++)
	{
		if(aliveHelper[i] == 1)
		isAlive[i] = 1.0f;
        if(isAlive[i] > 0.5)
        {



            glm::vec4 p_old = pos[i];
            glm::vec4 v_old = vel[i];
            glm::vec4 p_new = p_old;
            glm::vec4 v_new = v_old;

            life[i] -= lifeDeduction*dt;
            if(life[i] <= 0)
            {
                p_old = m_posInit[i];
                v_old = m_veloInit[i];
                life[i] = 1.0;
            }

        v_new.x = v_old.x + mass[i]* dt;
        v_new.y = v_old.y + -9.81f * dt;
        v_new.z = v_old.z + mass[i]*dt;

		p_new = p_old + v_new * dt ;

//        v_new *= 0.99999f;

	      //update the arrays with newly computed values
		 pos[i] = p_new;
		 vel[i] = v_new;
//        printf("v_new.x %f, v_new.y %f, v_new.z %f \n", vel[i].x, vel[i].y, vel[i].z );
	}
    }
}

void ParticleSystem::updateBuffers()
{
    if(m_renderingType == RenderingType::glpoints)
    {
	    glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_numParticles * sizeof(glm::vec4), &pos[0], GL_DYNAMIC_DRAW);

	    glBindBuffer(GL_ARRAY_BUFFER,life_vbo);
	    glBufferData(GL_ARRAY_BUFFER, m_numParticles * sizeof(float), &life[0], GL_DYNAMIC_DRAW);

    //	glBindBuffer(GL_ARRAY_BUFFER,alive_vbo);
    //	glBufferData(GL_ARRAY_BUFFER, float_size, &isAlive[0], GL_DYNAMIC_DRAW);
    }

    if(m_renderingType == RenderingType::instancing)
    {
        glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_numParticles * sizeof(glm::vec4), &pos[0], GL_STATIC_DRAW);
    }
}

void ParticleSystem::render()
{
    if (m_renderingType == RenderingType::glpoints)
    {
        //render Particles from VBOS
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_PROGRAM_POINT_SIZE);

        glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
        glDrawArrays(GL_POINTS, 0, m_numParticles);
    }

    if (m_renderingType == RenderingType::instancing)
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_numParticles);
}

void ParticleSystem::execute()
{
    glBindVertexArray(m_VAO);
    integration();
    updateBuffers();
    render();
    checkGLError("after execution");
}

RenderingType ParticleSystem::getRenderingType()
{
    return m_renderingType;
}

void ParticleSystem::setLifeDeduction(float f)
{
	lifeDeduction = f;
}

void ParticleSystem::checkGLError(std::string when) {
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "gl Error in " << when << ": " << err << std::endl;
    }
}

