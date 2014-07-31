#include "stdafx.h"
#include "Terrain.h"
#include "GeometryGenerator.h"
#include "Highmap.h"

Terrain::Terrain(const std::string& filename)
:ModelObject()
{
	//Update(10);

	std::unique_ptr<Highmap> map = std::make_unique<Highmap>();

	map->Load(filename);

	GeometryGenerator gen;

	gen.CreateFromHightMap(*m_Data, map.get());
	//GeometryGenerator gen;

	//gen.ComputeNormals(*m_Data);

}

void Terrain::Update(float delta)
{
	__super::Update(delta);

}