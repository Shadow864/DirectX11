#pragma once
#include "ModelObject.h"

class Terrain : public ModelObject
{
public:
	Terrain(const std::string& file_name);

	//virtual bool IsDynamic	(){ return true; }
	virtual void Update(float delta) override;

private:

};
