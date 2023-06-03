#include "Precomp.hpp"
#include "Constrain.hpp"
#include "Image.hpp"

Constrain::Constrain(Image& image) :
	m_Image(image)
{}

Constrain::~Constrain()
{}

//Loops over each pixel of a side and compares it with another
//If both rows are identical, they are added to the constrain vector
void Constrain::BuildWithImage()
{
	for (size_t i = 0; i < m_Image.m_ImageAmount; i++) //Loop over each image
	{
		std::vector<int> temporaryConstrains;

		for (size_t s = 0; s < 4; s++) //Loop over each side of the image
		{
			for (size_t j = 0; j < m_Image.m_ImageAmount; j++) //Loop over each other image
			{
				bool matching = true;

				for (size_t p = 0; p < m_Image.m_SampleSize; p++) //Loop over each pixel of a side
				{
					vec2 currentImage(0, 0);
					vec2 otherImage(0, 0);

					switch (s)
					{
					case 0: currentImage.x = p; currentImage.y = 0; otherImage.x = p; otherImage.y = m_Image.m_SampleSize - 1; //Top
						break;
					case 1: currentImage.x = m_Image.m_SampleSize - 1; currentImage.y = p; otherImage.x = 0; otherImage.y = p; //Left
						break;
					case 2: currentImage.x = p; currentImage.y = m_Image.m_SampleSize - 1; otherImage.x = p; otherImage.y = 0; //Bottom
						break;
					case 3: currentImage.x = 0; currentImage.y = p; otherImage.x = m_Image.m_SampleSize - 1; otherImage.y = p; //Right
						break;
					}

					currentImage.y *= m_Image.m_SampleSize;
					otherImage.y *= m_Image.m_SampleSize;

					for (size_t c = 0; c < m_Image.m_SourceChannels; c++) //Loop over each channel color
					{
						if (m_Image.m_Images[i][((currentImage.x + currentImage.y) * m_Image.m_SourceChannels) + c] !=
							m_Image.m_Images[j][((otherImage.x + otherImage.y) * m_Image.m_SourceChannels) + c])
							matching = false;
					}
				}

				if (matching)
					temporaryConstrains.push_back((s * m_Image.m_ImageAmount) + j);
			}
		}

		m_Constrains.push_back(temporaryConstrains);
	}
}

//Load in a csv file that contains the constrains for each image
void Constrain::BuildWithCSV(const std::string* csvPath)
{}