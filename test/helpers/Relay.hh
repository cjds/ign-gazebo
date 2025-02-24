/*
 * Copyright (C) 2020 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#ifndef IGNITION_GAZEBO_TEST_HELPERS_RELAY_HH_
#define IGNITION_GAZEBO_TEST_HELPERS_RELAY_HH_

#include <gtest/gtest.h>

#include <ignition/gazebo/test_config.hh>

#include "../plugins/MockSystem.hh"

namespace ignition
{
namespace gazebo
{
namespace test
{
/// \brief Helper class to be used in internal tests. It allows registering
/// callbacks that will be called during the update cycle. It works as a system
/// without the need for test writers to do the plugin loading themselves.
///
/// ## Usage
///
///  // Instantiate a relay system
///  test::Relay testSystem;
///
///  // Register callbacks, for example:
///  testSystem.OnPostUpdate([&](const gazebo::UpdateInfo &,
///    const gazebo::EntityComponentManager &_ecm)
///    {
///      // Add expectations here
///    }
///
///  // Add the system to a server before running it
///  server.AddSystem(testSystem.systemPtr);
///
class Relay
{
  /// \brief Constructor
  public: Relay()
  {
    this->systemPtr = std::make_shared<MockSystem>();
    EXPECT_NE(nullptr, this->systemPtr);
  }

  /// \brief Wrapper around system's pre-update callback
  /// \param[in] _cb Function to be called every pre-update
  public: Relay &OnPreUpdate(MockSystem::CallbackType _cb)
  {
    this->systemPtr->preUpdateCallback = std::move(_cb);
    return *this;
  }

  /// \brief Wrapper around system's update callback
  /// \param[in] _cb Function to be called every update
  public: Relay &OnUpdate(MockSystem::CallbackType _cb)
  {
    this->systemPtr->updateCallback = std::move(_cb);
    return *this;
  }

  /// \brief Wrapper around system's post-update callback
  /// \param[in] _cb Function to be called every post-update
  public: Relay &OnPostUpdate(MockSystem::CallbackTypeConst _cb)
  {
    this->systemPtr->postUpdateCallback = std::move(_cb);
    return *this;
  }

  /// \brief Pointer to system
  public: std::shared_ptr<MockSystem> systemPtr{nullptr};
};
}
}
}
#endif
