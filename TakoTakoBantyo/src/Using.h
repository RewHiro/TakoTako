#pragma once

#include <unordered_map>

class GameObject;
using GameObjectSP = std::shared_ptr < GameObject >const;
using GAMEOBJ_MAP_ITR = std::unordered_multimap<std::string, GameObjectSP>::iterator;