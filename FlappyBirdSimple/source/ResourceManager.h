#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <variant>
#include "renderer/Texture.h"
#include "renderer/Shader.h"

class ResourceManagerContext 
{
public:
	ResourceManagerContext();
	~ResourceManagerContext();
private:
	friend class ResourceManager;

	std::unordered_map<std::string, std::variant<std::shared_ptr<Shader>, std::shared_ptr<Texture>>> m_Resources;
};

class ResourceManager 
{
public:
	static void Init();
	static void Shutdown();
	template<typename TResource>
	inline static TResource& GetResource(std::string name) // FIX: Added & to return a reference
	{
		using VariantType = typename decltype(s_ResourceContext->m_Resources)::mapped_type;
		using TargetPtrType = std::shared_ptr<TResource>;

		static_assert(std::is_constructible_v<VariantType, TargetPtrType>,
			"The type you passed is not a valid type for the ResourceManager map!");

		auto it = s_ResourceContext->m_Resources.find(name);
		if (it != s_ResourceContext->m_Resources.end())
		{
			if (std::holds_alternative<TargetPtrType>(it->second))
			{
				TargetPtrType& resource = std::get<TargetPtrType>(it->second);
				return *resource; // Returns a direct reference to the object inside the shared_ptr
			}
		}

		// CRITICAL: Prevent undefined behavior if a resource name doesn't exist
		throw std::runtime_error("Resource not found: " + name);
	}

	template<typename TResource, typename... Args>
	inline static void LoadResource(std::string name, Args&&... args) 
	{
		using VariantType = typename decltype(s_ResourceContext->m_Resources)::mapped_type; // std::variant<std::shared_ptr<Shader>, ...>

		using TargetPtrType = std::shared_ptr<TResource>;
		static_assert(std::is_constructible_v<VariantType, TargetPtrType>,
			"std::shared_ptr<TResource> is not a valid type for this map!");

		static_assert(std::is_constructible_v<VariantType, TargetPtrType>,
			"The type you passed is not a valid type for the ResourceManager map!");

		s_ResourceContext->m_Resources.insert({name, std::make_shared<TResource>(std::forward<Args>(args)...)});
	}
private:
	inline static std::unique_ptr<ResourceManagerContext> s_ResourceContext;
};