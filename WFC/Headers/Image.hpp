#pragma once
#include <string> //Maybe string_view?
#include <vector>

struct vec2
{
	vec2();
	vec2(int x, int y) :
		x(x),
		y(y)
	{}

	int x;
	int y;
};

enum class Side
{
	Up,
	Left,
	Down,
	Right
};

class Image
{
public:
	Image(const std::string* filename, int sampleSize, bool rotate = false, bool mirror = false);
	~Image();


private:
	//Load a source image into the project
	bool LoadImage(const std::string* filename);

	//Samples part of the image to be used in contrain building
	void SampleFromImage(bool rotate = false, bool mirror = false);

	//Transforms the x and y coordinate depending on the rotation
	vec2 CalculateSamplePosition(Side side, int x, int y, bool mirror = false);

	//Write sampled images to file
	void WriteImages();

public:
	unsigned char* m_SourceImage;
	std::vector<unsigned char*> m_Images;

	int m_SourceChannels;
	int m_SourceWidth;
	int m_SourceHeight;
	int m_SampleSize;
	int m_ImageAmount;
};