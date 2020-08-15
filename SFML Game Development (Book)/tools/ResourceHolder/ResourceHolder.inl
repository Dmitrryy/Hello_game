
template <typename Resource, typename Key>
void ResourceHolder<Resource, Key>::load(Key _id, const std::string& _fname)
{
	auto  resource = std::make_unique<Resource>();
	if (!resource->loadFromFile(_fname))
		throw std::runtime_error("ResourceHolder::load - Failed to load from file: " + _fname);

	auto inserted = m_data.insert(std::make_pair(_id, std::move(resource)));
	assert(inserted.second);
}

template <typename Resource, typename Key>
template <typename SecondParam>
void ResourceHolder<Resource, Key>::load(Key _id, const std::string& _fname, SecondParam&& _sparam)
{
	auto  resource = std::make_unique<Resource>();
	if (!resource->loadFromFile(_fname, _sparam))
		throw std::runtime_error("ResourceHolder::load - Failed to load from file: " + _fname);

	auto inserted = m_data.insert(std::make_pair(_id, std::move(resource)));
	assert(inserted.second);
}

template <typename Resource, typename Key>
Resource& ResourceHolder<Resource, Key>::get(Key _id)
{
	auto res = m_data.find(_id);
	if (res == m_data.end()) { throw std::out_of_range("illegal id"); }

	return *(res->second);
}

template <typename Resource, typename Key>
const Resource& ResourceHolder<Resource, Key>::get(Key _id) const
{
	auto res = m_data.find(_id);
	if (res == m_data.end()) { throw std::out_of_range("illegal id"); }

	return *(res->second);
}