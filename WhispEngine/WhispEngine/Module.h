#pragma once
#include "Globals.h"
#include <string>
#include "JSON/json.hpp"

class Module
{
private :
	bool enabled;
public:
	std::string name = "module";

public:

	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init(nlohmann::json &node)
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual bool Save(nlohmann::json &node) 
	{ 
		return true;
	}

};