#pragma once
class Highmap
{


public:

	struct HeightMapType
	{
		float x, y, z;
	};

	Highmap();
	~Highmap();

	bool Load(const std::string& filename);


	UINT GetWidth() const { return m_Width; }
	UINT GetHeight() const { return m_Height; }

	float GetValue(UINT index) const  { return m_Values[index].y; }
	float GetValue(UINT x, UINT y)  const { return m_Values[x + y * m_Width].y; }

	HeightMapType GetNormal(UINT x, UINT y) const;

private:

	UINT m_Width;
	UINT m_Height;

	HeightMapType* m_Values;
};

