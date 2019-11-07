#pragma once
class Importer
{
public:
	Importer();
	virtual ~Importer();

	virtual bool Import(const char* path) { return false; };
};

