#pragma once

//mostly from SMFL game development

#include <map>
#include <memory>
#include <string>
#include <assert.h>

template <typename Resource, typename Identifier> class ResourceHolder {
public:
	void load(Identifier id, const std::string& filename);
	void add(Identifier ID, Resource* Resource);
	Resource&         get(Identifier id);        
	const Resource&   get(Identifier id) const;
private:        
	std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};


template <typename Resource, typename Identifier> 
inline void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
	std::unique_ptr<Resource> resource(new Resource());    
	if (!resource->loadFromFile(filename))        
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));    
	assert(inserted.second);
}

template<typename Resource, typename Identifier>
inline void ResourceHolder<Resource, Identifier>::add(Identifier ID, Resource* resource)
{
	auto inserted = mResourceMap.insert(std::make_pair(ID, std::move(std::unique_ptr<Resource>(resource))));
	assert(inserted.second);
}

template <typename Resource, typename Identifier>
inline Resource & ResourceHolder<Resource, Identifier>::get(Identifier id)
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());
	return *found->second;
}

template<typename Resource, typename Identifier>
inline const Resource & ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());
	return *found->second;
}
