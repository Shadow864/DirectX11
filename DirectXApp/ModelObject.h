#pragma once
#include "MeshData.h"
#include <memory>


class GameTimer;
class EffectsManager;
class GraphicsDevice;

class ITexture;

class Material;
struct ID3D11ShaderResourceView;

struct ID3D11DeviceContext;
struct ID3D11Device;
struct ID3D11Buffer;



class IInitializable
{
public:
	virtual void Initialize		(ID3D11Device* device) = 0;
};

class ILoadable
{
public:
	virtual bool LoadContent		(ID3D11Device* device) = 0;
	virtual bool UnloadContent		(ID3D11Device* device) = 0;
};

class IRenderable
{
public:
	virtual void Render(GraphicsDevice* device) = 0;
};

class IUpdateable
{
public:
	virtual void Update			(float delta) = 0;
};

__declspec(align(32))
class ModelObject : public IRenderable, public IUpdateable, public IInitializable, public ILoadable
{
	
public:
	ModelObject();

	virtual ~ModelObject(void);

	virtual void					Render				(GraphicsDevice* device) override;
	virtual void					Initialize			(ID3D11Device* device) override;
	virtual bool					LoadContent			(ID3D11Device* device) override;
	virtual bool					UnloadContent		(ID3D11Device* device) override;

	virtual void					Update(float delta) override;

	std::shared_ptr<Material>		GetMaterial()	const	{ return m_Material; }
	void							SetMaterial(std::shared_ptr<Material> material);



	std::shared_ptr<MeshData>		GetMeshData			() const	{ return m_Data;  }
	ID3D11Buffer*					GetVerticesBuffer	() const	{ return m_VerticesBuffer; }
	ID3D11Buffer*					GetIndicesBuffer	() const	{ return m_IndicesBuffer; }
	
	void	SetPosition	(float x, float y, float z);
	void	SetRotation	(float roll, float pitch, float yaw);
	void	SetScale	(float x, float y, float z);

	DirectX::XMMATRIX GetWorldMatrix() const { return m_WorldMatrix;  }


	void	SetReflection(DirectX::XMMATRIX reflection);

	void	SetCastShadow(bool cast) { m_CastShadow = cast; }
	bool	CanCastShadow() const { return m_CastShadow;  }


protected:

	void	RecalculateWorldMatrix();
protected:

	ID3D11Buffer*				m_VerticesBuffer;
	ID3D11Buffer*				m_IndicesBuffer;

	DirectX::XMVECTOR			m_Translation;
	DirectX::XMVECTOR			m_Scale;
	DirectX::XMVECTOR			m_Rotation;

	float						m_Pith;
	float						m_Roll;
	float						m_Yaw;

	DirectX::XMMATRIX			m_WorldMatrix;
	
	std::shared_ptr<MeshData>	m_Data;
	std::shared_ptr<Material>	m_Material;
public:


	bool						m_CastShadow;
};


class Triangle : public ModelObject
{
public:
	Triangle();
};

class Pyramid: public ModelObject
{
public: 
	Pyramid();
};

class SierpinskiTriangle : public ModelObject
{
public:
	SierpinskiTriangle(UINT level); 
};

class SierpinskiPyramid : public ModelObject
{

public:
	SierpinskiPyramid(UINT level); 
};

class Plane : public ModelObject
{
public:
	Plane();
};

class Grid : public ModelObject
{
public:
	Grid(float width, float depth, UINT m, UINT n);
};

class Cylinder : public ModelObject
{
public:
	Cylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount);
};

class Geosphere : public ModelObject
{
public:
	Geosphere(float radius, UINT numSubdivisions);
};

class Box : public ModelObject
{
public:
	Box();
};


class Wave : public ModelObject
{
public:
	Wave(float width, float depth, UINT m, UINT n);

	//virtual bool IsDynamic	(){ return true; }
	virtual void Update(float delta) override;

protected:
	void				UpdateMesh();

private:

	float				GetHeight(UINT x, UINT z, UINT m, UINT n, float time) const;
	float				GetHeight(float x, float z, float time) const;

	DirectX::XMFLOAT3	GetNormal(UINT x, UINT z, UINT m, UINT n, float time) const;
	DirectX::XMFLOAT3	GetNormal(float x, float z, float time) const;

private:
	float m_Time;
	float m_Width;
	float m_Depth;
	UINT  m_M;
	UINT  m_N;
};

class TestObject: public ModelObject
{
public:
	TestObject(const std::string& file_name);
	virtual void Update(float delta) override;
};

class MirrorObject : public Grid
{
public:
	MirrorObject(float width, float depth, UINT m, UINT n);

	virtual void Render(GraphicsDevice* device) override;

	void AddObject(ModelObject* object) { m_Objects.push_back(object); }
	std::vector < ModelObject*>  m_Objects;
};


class Billboard : public ModelObject
{
public:
	Billboard();
};


class Circle : public ModelObject
{
public:
	Circle();
};

class Point : public ModelObject
{
public:
	Point();
};