#include "util.h"

float rand_float(float mn, float mx)
{
    float r = rand() / (float) RAND_MAX;
    return mn + (mx-mn)*r;
}


std::string loadfromfile(std::string filename)
{
	std::ifstream file;
	file.open(filename.c_str());
	if(!file.good())
	{
		std::cout << " Failed to open file: " << filename << std::endl;
		exit(-1);
	}
	std::stringstream stream;
	stream << file.rdbuf();

	file.close();
	return stream.str();
}

