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

private:
	std::vector<std::vector<int>> m_Constrains;
	Image& m_Image;
};