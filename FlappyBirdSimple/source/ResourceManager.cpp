#include "ResourceManager.h"

void ResourceManager::Init()
{
	s_ResourceContext = std::make_unique<ResourceManagerContext>();
}

void ResourceManager::Shutdown()
{

}

ResourceManagerContext::ResourceManagerContext()
{

}

ResourceManagerContext::~ResourceManagerContext()
{

}
