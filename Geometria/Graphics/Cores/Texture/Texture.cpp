#include "Texture.h"
#include "../../../Files/Files.h"
#include <algorithm>

std::vector<TextureGroup> TextureManager::textureGroups;

Texture::Texture() {}

Texture::Texture(const char* fileName, Type type)
{
	data = Files::GetImageData(fileName, width, height);
	filename = fileName;

	if (TextureManager::textureGroups.size() == 0)
	{
		TextureManager::textureGroups.push_back(TextureGroup());
		TextureManager::textureGroups[TextureManager::textureGroups.size() - 1].id = TextureManager::textureGroups.size() - 1;
	}

	TextureManager::textureGroups[TextureManager::textureGroups.size() - 1].AddTexture(*this);
}

Texture::Texture(std::string fileName, Type type)
{
	data = Files::GetImageData(fileName.c_str(), width, height);
	filename = fileName;

	if (TextureManager::textureGroups.size() == 0)
	{
		TextureManager::textureGroups.push_back(TextureGroup());
		TextureManager::textureGroups[TextureManager::textureGroups.size() - 1].id = TextureManager::textureGroups.size() - 1;
	}

	TextureManager::textureGroups[TextureManager::textureGroups.size() - 1].AddTexture(*this);
}

void TextureGroup::AddTexture(Texture& tex)
{
	allTextures.push_back(&tex);
	allTextures[allTextures.size() - 1]->texGroupId = id;
	
	Texture& t = *allTextures[allTextures.size() - 1];

	for (int h = 0; h < t.height; h++)
	{
		int widthEmptyCount = 0;
		for (int w = 0; w < t.width; w++)
		{
			int index = 4 * (h * t.width + w);
			if (t.data[index + 3] == 0)
			{
				if (t.data[index + 0] == 0 && t.data[index + 1] == 0 && t.data[index + 2] == 0)
				{
					widthEmptyCount++;
				}
			}
		}

		if (widthEmptyCount == t.width)
		{
			int begin = 4 * (h * t.width + 0), end = 4 * (h * t.width + widthEmptyCount);
			t.data.erase(t.data.begin() + begin, t.data.begin() + end);
			t.height -= 1;
		}
	}
}

void TextureGroup::UploadToGPU()
{
	bool packingDone = false;

	while (!packingDone)
	{
		if (width == 0 && height == 0)
		{
			width = height = 1024;
		}

		std::cout << width << " x " << height << std::endl;
		bin.Init(width, height);

		int countDone = 0;
		for (int i = 0; i < allTextures.size(); i++)
		{
			int imageWidth = allTextures[i]->width;
			int imageHeight = allTextures[i]->height;

			rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic = rbp::MaxRectsBinPack::RectContactPointRule; // This can be changed individually even for each rectangle packed.
			rbp::Rect packedRect = bin.Insert(imageWidth, imageHeight, heuristic);

			if (packedRect.height > 0)
			{
				printf("Packed to (x,y)=(%d,%d), (w,h)=(%d,%d). Free space left: %.2f%%\n", packedRect.x, packedRect.y, packedRect.width, packedRect.height, 100.f - bin.Occupancy() * 100.f);
				allTextures[i]->finalRect = packedRect;
				countDone++;
			}
			else
			{
				printf("Failed! Could not find a proper position to pack this rectangle into. Repacking!.\n");
				width += 1024;
				height += 1024;
				break;
			}
		}

		if (countDone == allTextures.size())
			packingDone = true;

	}

	std::cout << width << " x " << height << std::endl;

	int totalSize = 4 * (width * height);
	groupData.clear();
	groupData.resize(totalSize);

	std::cout << "TextureGroup size: " << allTextures.size() << std::endl;

	int count = 0;

	for (int i = 0; i < allTextures.size(); i++)
	{
		int forWidth = allTextures[i]->finalRect.x;
		int forHeight = allTextures[i]->finalRect.y;

		std::cout << forWidth << " + " << forWidth + allTextures[i]->width << " x " << forHeight << " + " << forHeight + allTextures[i]->height << std::endl;

		std::vector<unsigned char> imageData = allTextures[i]->data;

		bool firstP = true;
		count = 0;
		//std::cout << "============" << std::endl;
		int normalH = 0;
		for (int h = forHeight; h < forHeight + allTextures[i]->height; h++)
		{
			int normalW = 0;
			for (int w = forWidth; w < forWidth + allTextures[i]->width; w++)
			{
				int index = 4 * (h * width + w);
				int r = index, g = index + 1, b = index + 2, a = index + 3;
				int imageIndex = 4 * (normalH * allTextures[i]->width + normalW);
				int imR = imageIndex, imG = imageIndex + 1, imB = imageIndex + 2, imA = imageIndex + 3;

				//std::cout << index << "(" << imR << ")" << " ";

				groupData[r] = imageData[imR];
				groupData[g] = imageData[imG];
				groupData[b] = imageData[imB];
				groupData[a] = imageData[imA];

				normalW++;
			}
			normalH++;
		}
		//std::cout << std::endl;
	}

	//Files::SaveImage("Resources/result.png", groupData, width, height);

	//==[ OPENGL ]==//
	{
		this->type = GL_TEXTURE_2D;

		glEnable(GL_TEXTURE_2D);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		if (groupData.size() != 0)
		{
			unsigned char* image = &groupData[0];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			std::cout << "ERROR: TextureGroup not having data!" << std::endl;
		}

		//glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

TextureGroup::~TextureGroup()
{
	//==[ OPENGL ]==//
	{
		if (!glfwTerminate)
			glDeleteTextures(GL_TEXTURE_2D, &texture);
	}
}

inline GLuint TextureGroup::getID() const
{
	//==[ OPENGL ]==//
	return this->texture;
}

void TextureGroup::Enable(const GLint tUnit)
{
	//==[ OPENGL ]==//
	glActiveTexture(GL_TEXTURE0 + tUnit);
	glBindTexture(type, this->texture);
}

void TextureGroup::Disable()
{
	//==[ OPENGL ]==//
	glActiveTexture(0);
	glBindTexture(type, 0);
}
