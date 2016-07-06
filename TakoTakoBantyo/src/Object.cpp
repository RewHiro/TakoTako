#include "Object.h"

Object::Object(const std::string& name) :
update_state_(UpdateState::START),
name_(name),
is_delete_(false)
{

}