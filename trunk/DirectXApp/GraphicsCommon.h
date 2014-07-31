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
	COUNT
};