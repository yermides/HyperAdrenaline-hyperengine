#include <managers/resource_manager.hpp>

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    INFOLOG("~ResourceManager(), liberando los recursos...");
    freeAllResources();
}
