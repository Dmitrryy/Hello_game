#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <cassert>

template <typename Resource, typename Key>
class ResourceHolder
{
public:

	void load(Key _id, const std::string& _fname);

	template<typename SecondParam>
	void load(Key _id, const std::string& _fname, SecondParam&& _sparam);

	Resource&       get(Key _id);
	const Resource& get(Key _id) const;

	void clear() { m_data.clear(); }

private:

	std::map < Key, std::unique_ptr <Resource> > m_data;
};

#include "ResourceHolder.inl"