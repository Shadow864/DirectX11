#pragma once
#include <map>
#include <memory>
#include <string>

class Material;

class MaterialManager
{

//typedef std::map<std::string, Material*> MaterialVec;

public:
	MaterialManager();
	~MaterialManager();

	//std::unique_ptr<Material> GetMaterial(const std::string& name);

public:

	std::shared_ptr<Material> Default;
	std::shared_ptr<Material> Water;
	std::shared_ptr<Material> Terrain;

	std::shared_ptr<Material> Fire;
	std::shared_ptr<Material> Bolt;
	std::shared_ptr<Material> WoodenBox;
	std::shared_ptr<Material> WireFence;
	std::shared_ptr<Material> BrickFloor;
	std::shared_ptr<Material> Skull;
	std::shared_ptr<Material> Wall;

	std::shared_ptr<Material> Shadow;
	std::shared_ptr<Material> Mirror;

	std::shared_ptr<Material> Tree;

	std::shared_ptr<Material> Cylinder;
	std::shared_ptr<Material> Sphere;

	std::shared_ptr<Material> Subdivision;
	//MaterialVec	m_Material;
};

