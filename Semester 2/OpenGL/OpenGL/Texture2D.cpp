#include "Texture2D.h"

Texture2D::Texture2D()
{
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &_ID);
}

bool Texture2D::Load(char* path, int width, int height)
{
	char* tempTextureData;
	int fileSize;
	std::ifstream inFile;

	_width = width;
	_height = height;

	inFile.open(path, std::ios::binary);

	if (!inFile.good())
	{
		std::cerr << "Can't Open Texture File" << std::endl;
		return false;
	}

	inFile.seekg(0, std::ios::end); //Go To End Of File
	fileSize = (int)inFile.tellg(); //Get Position In File
	tempTextureData = new char[fileSize]; //Create Array To Store Data
	inFile.seekg(0, std::ios::beg); //Seek Back To Beginning of File
	inFile.read(tempTextureData, fileSize); //Read In All Data
	inFile.close(); //Close File

	//std::cout << path << " Loaded." << std::endl;

	glGenTextures(1, &_ID); //Get Next Texture ID
	glBindTexture(GL_TEXTURE_2D, _ID); //Bind The Texture To The ID
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);

	delete[] tempTextureData; //Delete Temp Data

	return true;
}
