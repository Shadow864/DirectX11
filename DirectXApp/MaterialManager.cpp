#include "stdafx.h"
#include "MaterialManager.h"

#include "Material.h"
#include "Texture.h"
#include "BasicMaterial.h"
#include "BillboardMaterial.h"
#include "CylinderMaterial.h"
#include "SphereMaterial.h"
#include "SubdivisionMaterial.h"
#include "ExplosionMaterial.h"

MaterialManager::MaterialManager()
{

	///////////////////// Default ///////////////////
	Default = std::make_shared<BasicMaterial>();

	
	///////////////////// Water ///////////////////
	std::shared_ptr<ITexture> water_texture = std::make_shared<Texture>(L"Textures/water2.dds");
	water_texture->SetScale(30, 30);

	Water = std::make_shared<BasicMaterial>();
	Water->m_RenderState.BlendMode.Set(BlendMode::TRANSPARENT, Scope::MATERIAL);
	Water->m_DiffuseMap		= water_texture;
	Water->m_Diffuse.w		= 0.4f;

	///////////////////// Terrain ///////////////////
	std::shared_ptr<ITexture> terrain_texture = std::make_shared<Texture>(L"Textures/grass.dds");
	terrain_texture->SetScale(20, 20);

	Terrain = std::make_shared<BasicMaterial>();
	Terrain->m_DiffuseMap	= terrain_texture;
	Terrain->m_Specular.w = 0;

	///////////////////// Fire ///////////////////
	std::shared_ptr<ITexture> fire_texture = std::make_shared<AnimateTexture>(L"Textures/FireAnim/Fire", 120);

	Fire = std::make_shared<BasicMaterial>();
	Fire->m_DiffuseMap		= fire_texture;


	///////////////////// Bolt ///////////////////
	std::shared_ptr<ITexture> bolt_texture = std::make_shared<AnimateTexture>(L"Textures/BoltAnim/Bolt", 3);

	Bolt = std::make_shared<BasicMaterial>();
	Bolt->m_RenderState.BlendMode.Set(BlendMode::TRANSPARENT, Scope::MATERIAL);
	Bolt->m_RenderState.CullMode.Set(CullFaceMode::NONE, Scope::MATERIAL);
	Bolt->m_Diffuse.w = 0.9;
	Bolt->m_DiffuseMap = bolt_texture;

	///////////////////// WoodenBox ///////////////////
	std::shared_ptr<ITexture> woodenbox_texture = std::make_shared<Texture>(L"Textures/WoodCrate01.dds");

	WoodenBox = std::make_shared<BasicMaterial>();
	WoodenBox->m_DiffuseMap		= woodenbox_texture;

	///////////////////// WireFence ///////////////////
	std::shared_ptr<ITexture> wirefence_texture = std::make_shared<Texture>(L"Textures/WireFence.dds");
	WireFence = std::make_shared<BasicMaterial>();
	WireFence->m_RenderState.CullMode.Set(CullFaceMode::NONE, Scope::MATERIAL);
	WireFence->m_RenderState.BlendMode.Set(BlendMode::ALPHA_TO_COVERAGE, Scope::MATERIAL);
	WireFence->m_DiffuseMap		= wirefence_texture;

	///////////////////// BrickFloor ///////////////////
	std::shared_ptr<ITexture> brick_texture = std::make_shared<Texture>(L"Textures/darkbrickdxt1.dds");
	brick_texture->SetScale(10, 10);

	BrickFloor = std::make_shared<BasicMaterial>();
	BrickFloor->m_RenderState.CullMode.Set(CullFaceMode::FRONT, Scope::MATERIAL);
	BrickFloor->m_DiffuseMap = brick_texture;
	
	///////////////////// Mirror ///////////////////
	std::shared_ptr<ITexture> mirror_texture = std::make_shared<Texture>(L"Textures/ice.dds");

	Mirror = std::make_shared<BasicMaterial>();
	Mirror->m_RenderState.BlendMode.Set(BlendMode::TRANSPARENT, Scope::MATERIAL);
	Mirror->m_Diffuse.w = 0.35;
	Mirror->m_Specular = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 100);
	Mirror->m_DiffuseMap = mirror_texture;

	///////////////////// Shadow ///////////////////
	Shadow = std::make_shared<BasicMaterial>();
	Shadow->m_RenderState.BlendMode.Set(BlendMode::TRANSPARENT, Scope::MATERIAL);
	Shadow->m_RenderState.DepthStencilMode.Set(DepthStencilMode::SHADOW, Scope::MATERIAL);
	Shadow->m_RenderState.DepthStencilRef.Set(0, Scope::MATERIAL);
	Shadow->m_Diffuse.x = 0;
	Shadow->m_Diffuse.y = 0;
	Shadow->m_Diffuse.z = 0;
	Shadow->m_Diffuse.w = 0.9;


	Shadow->m_Ambient.x = 0;
	Shadow->m_Ambient.y = 0;
	Shadow->m_Ambient.z = 0;


	Shadow->m_Specular.x = 0;
	Shadow->m_Specular.y = 0;
	Shadow->m_Specular.z = 0;
	Shadow->m_Specular.w = 0;

	Skull = std::make_shared<BasicMaterial>();
	Skull->m_Ambient = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.0f);
	Skull->m_Diffuse = DirectX::XMFLOAT4(.3f, 0.3f, 0.3f, 1.0f);
	Skull->m_Specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 100.0f);

	std::shared_ptr<ITexture> wall_texture = std::make_shared<Texture>(L"Textures/brick01.dds");

	Wall = std::make_shared<BasicMaterial>();
	Wall->m_RenderState.CullMode.Set(CullFaceMode::FRONT, Scope::MATERIAL);
	Wall->m_DiffuseMap = wall_texture;

	std::shared_ptr<ITexture> tree_texture = std::make_shared<Texture>(L"Textures/tree1.dds");
	Tree = std::make_shared<BillboardMaterial>();
	Tree->m_EffectType = EffectType::BILLBOARD;
	Tree->m_RenderState.CullMode.Set(CullFaceMode::NONE, Scope::MATERIAL);
	Tree->m_RenderState.BlendMode.Set(BlendMode::TRANSPARENT, Scope::MATERIAL);
	Tree->m_DiffuseMap = tree_texture;

	Cylinder = std::make_shared<CylinderMaterial>();
	Cylinder->m_EffectType = EffectType::CYLINDER;
	Cylinder->m_DiffuseMap = fire_texture;

	Sphere = std::make_shared<SphereMaterial>();
	Sphere->m_EffectType = EffectType::SPHERE;
	Sphere->m_DiffuseMap = fire_texture;

	Subdivision = std::make_shared<SubdivisionMaterial>();
	Subdivision->m_EffectType = EffectType::SUBDIVISION;
	Subdivision->m_DiffuseMap = fire_texture;

	Explosion = std::make_shared<ExplosionMaterial>();
	Explosion->m_EffectType = EffectType::EXPLOSION;
	Explosion->m_DiffuseMap = fire_texture;
	
}

MaterialManager::~MaterialManager()
{

}
