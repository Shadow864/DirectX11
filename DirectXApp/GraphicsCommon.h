#pragma once
#undef TRANSPARENT

enum class CullFaceMode
{
	FRONT,
	NONE,
	COUNTER
};

enum class BlendMode
{
	NONE = 0,
	MODULATE,
	ADDITIVE,
	TRANSPARENT,
	ALPHA_TO_COVERAGE,
	NO_RENDER_TARGET,
};

enum class FillMode
{
	SOLID = 0,
	WIREFRAME,
};

enum class  DepthStencilMode 
{
	NONE, 
	MIRROR, 
	REFLECTION,
	SHADOW,
	REFLECTION_SHADOW
};

enum class RenderNormal
{
	NONE,
	NORMAL,
	SMOOTH,
};

enum class Technique
{
	NORMAL,
	COLOR,
	STENCIL_ONLY,
};

enum class Scope
{
	GLOBAL = 0,
	OBJECT,
	MATERIAL,
	COUNT,
};


enum class EffectType
{
	BASIC,
	BILLBOARD,
	CYLINDER,
	SPHERE,
	SUBDIVISION,
	EXPLOSION,
	COUNT
};

enum class VertexType
{
	POS_NORMAL,
	POS_SIZE,
	POS_NORMAL_TEXCOORD,
	COUNT
};

enum class TopologyType
{
	POINT_LIST,
	LINE_STRIP,
	TRIANGLE_LIST,
};

