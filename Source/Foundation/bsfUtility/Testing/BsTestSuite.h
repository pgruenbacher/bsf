//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include <gtest/gtest.h>
#include "Prerequisites/BsPrerequisitesUtil.h"
#include "BsCorePrerequisites.h"



namespace bs
{

class AppEnvironment : public ::testing::Environment {
 public:

  // Override this to define how to set up the environment.
  virtual void SetUp() override;

  // Override this to define how to tear down the environment.
  virtual void TearDown() override;
};


class Test : public ::testing::Test {
protected:
	// setup once for all tests within the test suite.
	// static void SetUpTestSuite();
	// static void TearDownTestSuite();

  HSceneObject addFlyableCamera();
};

} // namespace bs