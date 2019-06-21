#pragma once

#include "BsEnTTFwd.h"

namespace bs::ecs {


struct ParentChild {
	EntityType parent{entt::null};
	EntityType child{entt::null};
	EntityType nextSibling{entt::null};
	EntityType prevSibling{entt::null};
};

}