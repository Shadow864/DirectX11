#include "stdafx.h"
#include "Highmap.h"


Highmap::Highmap()
:m_Values(nullptr)
{
}


Highmap::~Highmap()
{
	delete[] m_Values;
	m_Values = nullptr;
}

bool Highmap::Load(const std::string& file_name)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;

	// Open the height map file in binary.
	error = fopen_s(&filePtr, file_name.c_str(), "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Save the dimensions of the terrain.
	m_Width = bitmapInfoHeader.biWidth;
	m_Height = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = m_Width * m_Height * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Create the structure to hold the height map data.
	m_Values = new HeightMapType[m_Width * m_Height];
	if (!m_Values)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	float max = 0;

	// Read the image data into the height map.
	for (j = 0; j < m_Height; j++)
	{
		for (i = 0; i < m_Width; i++)
		{
			height = bitmapImage[k];

			max = max(height, max);

			index = (m_Height * j) + i;

			m_Values[index].x = (float)i;
			m_Values[index].y = (float)height / 255;
			m_Values[index].z = (float)j;

			k += 3;
		}
	}

	

	delete[] bitmapImage;

	return true;
}

Highmap::HeightMapType Highmap::GetNormal(UINT x, UINT y) const
{

	float sx = GetValue(x < m_Width - 1 ? x + 1 : x, y) - GetValue(x ? x - 1 : x, y);
	if (x == 0 || x == m_Width - 1)
		sx *= 2;

	float sy = GetValue(x, y<m_Height - 1 ? y + 1 : y) - GetValue(x, y ? y - 1 : y);
	if (y == 0 || y == m_Height - 1)
		sy *= 2;

	HeightMapType normal;

	normal.x = -sx;
	normal.y = 0.2;
	normal.z = sy;

	return normal;
}