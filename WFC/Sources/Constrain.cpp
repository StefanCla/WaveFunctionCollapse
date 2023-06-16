#include "Precomp.hpp"
#include "Constrain.hpp"
#include "Image.hpp"
#include <fstream>
#include <sstream>
#include <assert.h>

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
						//Top
					case 0: currentImage.x = p; currentImage.y = 0; otherImage.x = p; otherImage.y = m_Image.m_SampleSize - 1;
						break;
						//Left
					case 1: currentImage.x = m_Image.m_SampleSize - 1; currentImage.y = p; otherImage.x = 0; otherImage.y = p;
						break;
						//Bottom
					case 2: currentImage.x = p; currentImage.y = m_Image.m_SampleSize - 1; otherImage.x = p; otherImage.y = 0;
						break;
						//Right
					case 3: currentImage.x = 0; currentImage.y = p; otherImage.x = m_Image.m_SampleSize - 1; otherImage.y = p;
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
//Follow the convention in the file
//Thanks to: https://java2blog.com/read-csv-file-in-cpp/ for the inspiration
void Constrain::BuildWithCSV(const std::string& csvPath)
{
	std::fstream file(csvPath.c_str());

	if (file.is_open())
	{
		std::vector<int> temporaryConstrains;
		std::string fileLine, fileWord;
		while (std::getline(file, fileLine))
		{
			temporaryConstrains.clear();

			//Exclude comments
			if (fileLine[0] == '/')
				continue;

			//Seperate each number by use of comma
			std::stringstream stream(fileLine);
			while (std::getline(stream, fileWord, ','))
			{
				temporaryConstrains.push_back(atoi(fileWord.c_str()));
			}

			//Add offsets to the array 
			if (temporaryConstrains.size() > 1)
			{
				int tileOffSet = 0;
				switch (temporaryConstrains[1])
				{
				case 1: tileOffSet = TILEAMOUNT * 1;	//Left
					break;
				case 2: tileOffSet = TILEAMOUNT * 2;	//Down
					break;
				case 3: tileOffSet = TILEAMOUNT * 3;	//Right
					break;
				default: tileOffSet = 0;				//Up
					break;
				}

				//Add the constrains to the array
				//Start at index 2 as index 0 is reserved for the tile
				//And index 1 is reserved for index 1
				for (size_t i = 2; i < temporaryConstrains.size(); i++)
				{
					if (m_Constrains.size() < (temporaryConstrains[0] + 1))
					{
						std::vector<int> temp;
						temp.push_back(tileOffSet + temporaryConstrains[i]);
						m_Constrains.push_back(temp);
					}
					else
						m_Constrains[temporaryConstrains[0]].push_back(tileOffSet + temporaryConstrains[i]);

				}
			}
			else if (temporaryConstrains.size() == 1) //If a tile has not constrains, just add an empty vector
			{
				m_Constrains.push_back(std::vector<int>());
			}
		}

		if (m_Constrains.size() != TILEAMOUNT)
			assert(false && "Make sure there are as many constrains as there are tiles");
	}
	else
	{
		printf("Unable to open the csv file\n");
		return;
	}
}