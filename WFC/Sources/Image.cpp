#include "Precomp.hpp"
#include "Image.hpp"
#include <filesystem>

//stb_image
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

//stb_image_write
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

Image::Image(const std::string* filename, int sampleSize, bool rotate, bool mirror) :
	m_SampleSize(sampleSize),
	m_SourceImage(nullptr),
	m_SourceChannels(0),
	m_SourceHeight(0),
	m_SourceWidth(0),
	m_ImageAmount(0)
{
	if(LoadImage(filename))
		SampleFromImage(rotate, mirror);
}

Image::~Image()
{
	stbi_image_free(m_SourceImage);

	for (size_t i = 0; i < m_Images.size(); i++)
		delete m_Images[i];
}

bool Image::LoadImage(const std::string* filename)
{
	m_SourceImage = stbi_load(filename->c_str(), &m_SourceWidth, &m_SourceHeight, &m_SourceChannels, 0);
	if (m_SourceImage == nullptr)
	{
		printf("ERROR: Unable to load in image\n");
		return false;
	}

	return true;
}

vec2 Image::CalculateSamplePosition(Side side, int x, int y, bool mirror)
{
	vec2 position(0, 0);

	switch (side)
	{
	case Side::Up:
	{
		position.x = x;
		position.y = y * m_SampleSize;
	}
		break;
	case Side::Left:
	{
		position.x = (m_SampleSize - 1) - y;
		position.y = x * m_SampleSize;
	}
		break;
	case Side::Down:
	{
		//Double mirror & reflect
		//position.x = (m_SampleSize - 1) - y;
		//position.y = m_SampleSize * ((m_SampleSize - 1) - x);

		if (mirror)
		{
			position.x = x;
			position.y = m_SampleSize * ((m_SampleSize - 1) - y);
		}
		else
		{
			position.x = (m_SampleSize - 1) - x;
			position.y = m_SampleSize * ((m_SampleSize - 1) - y);
		}
	}
		break;
	case Side::Right:
	{
		position.x = y;
		position.y = m_SampleSize * ((m_SampleSize - 1) - x);
	}
		break;
	}

	return position;
}

void Image::SampleFromImage(bool rotate, bool mirror)
{
	//Check if samplesize fits within the width & height of the image
	if((m_SourceWidth % m_SampleSize) != 0 || (m_SourceHeight % m_SampleSize) != 0)
	{
		printf("ERROR: The samplesize does not divide with the width or height of the image\n");
		//return false;
	}

	//Allocate sample images
	m_ImageAmount = (m_SourceWidth / m_SampleSize) * (m_SourceHeight / m_SampleSize);
	if (rotate)
		m_ImageAmount = m_ImageAmount * 4; //For each rotation

	for (size_t i = 0; i < m_ImageAmount; i++)
	{
		m_Images.push_back(new unsigned char[m_SampleSize * m_SampleSize * m_SourceChannels]);
	}

	//Sample from source image
	int increment = 1;

	if (rotate)
		increment = 4;

	for (size_t i = 0; i < m_ImageAmount; i += increment)
	{
		//Calculate current image and on the Y axis
		int currentImage = i;
		if (rotate)
			currentImage = i * 0.25f;

		int currentImageY = (m_SampleSize * currentImage) / m_SourceWidth;

		for (size_t y = 0; y < m_SampleSize; y++)
		{
			for (size_t x = 0; x < m_SampleSize; x++)
			{
				//Calculate source position
				vec2 sourcePos(0, 0);
				sourcePos.x = (m_SampleSize * currentImage) + x;
				sourcePos.y = ((m_SampleSize - 1) * m_SourceWidth * currentImageY) + (y * m_SourceWidth);

				//Calculate sample position
				if (rotate)
				{
					vec2 posUp = CalculateSamplePosition(Side::Up, x, y, mirror);
					vec2 posLeft = CalculateSamplePosition(Side::Left, x, y, mirror);
					vec2 posDown = CalculateSamplePosition(Side::Down, x, y, mirror);
					vec2 posRight = CalculateSamplePosition(Side::Right, x, y, mirror);

					for (size_t c = 0; c < m_SourceChannels; c++)
					{
						//Up
						m_Images[i][((posUp.y + posUp.x) * m_SourceChannels) + c] =
							m_SourceImage[((sourcePos.y + sourcePos.x) * m_SourceChannels) + c];

						//Left
						m_Images[i + 1][((posLeft.y + posLeft.x) * m_SourceChannels) + c] =
							m_SourceImage[((sourcePos.y + sourcePos.x) * m_SourceChannels) + c];

						//Down
						m_Images[i + 2][((posDown.y + posDown.x) * m_SourceChannels) + c] =
							m_SourceImage[((sourcePos.y + sourcePos.x) * m_SourceChannels) + c];

						//Right
						m_Images[i + 3][((posRight.y + posRight.x) * m_SourceChannels) + c] =
							m_SourceImage[((sourcePos.y + sourcePos.x) * m_SourceChannels) + c];
					}
				}
				else
				{
					vec2 pos = CalculateSamplePosition(Side::Up, x, y, mirror);

					for (size_t c = 0; c < m_SourceChannels; c++)
					{
						m_Images[i][((pos.y + pos.x) * m_SourceChannels) + c] =
							m_SourceImage[((sourcePos.y + sourcePos.x) * m_SourceChannels) + c];
					}
				}
			}
		}
	}

	WriteImages();
	//return true;
}

void Image::WriteImages()
{
	std::string path = "..//SampledImages";

	//Make directory if not exist
	if (!std::filesystem::create_directory(path.c_str()))
		printf("Unable to create directory SampledImages, might already exist\n");

	if (std::filesystem::is_directory(path.c_str()))
	{
		for (size_t i = 0; i < m_ImageAmount; i++)
		{
			std::string imageName = path + "/" + std::to_string(i) + ".png";
			stbi_write_png(imageName.c_str(), m_SampleSize, m_SampleSize, m_SourceChannels, static_cast<void*>(m_Images[i]), TILESIZE * m_SourceChannels);
		}
	}
}