#pragma once

#include "boost/noncopyable.hpp"

#include "Using.h"

class ObjectFactory : private boost::noncopyable
{
public:

	static ObjectFactory& getInstance()
	{
		static ObjectFactory instance;
		return instance;
	}

private:

	ObjectFactory() = default;
};