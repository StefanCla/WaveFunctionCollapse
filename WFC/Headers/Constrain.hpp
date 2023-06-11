#pragma once
#include <string>
#include <vector>

class Image;

class Constrain
{
public:
	Constrain(Image& image);
	~Constrain();

	//Build constrains by comparing a row of pixels on each side
	void BuildWithImage();

	//Import constrains from a csv file
	void BuildWithCSV(const std::string* csvPath);

	std::vector<std::vector<int>> m_Constrains;

private:
	Image& m_Image;
};