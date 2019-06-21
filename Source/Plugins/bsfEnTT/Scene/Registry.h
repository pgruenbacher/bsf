#pragma once

#include "BsPrerequisites.h"
#include "Utility/BsModule.h"
#include "BsEnTTFwd.h"

namespace bs::ecs {

  void startUp();
  void shutDown();
  Registry* gRegistry();
}  // namespace bs
