#################################################
# gz_add_gui_library (<library_name>
#              SOURCES <sources>
#              [PUBLIC_LINK_LIBS <libraries...>]
#              [PRIVATE_LINK_LIBS <libraries...>])
#
# Add a gui library to Ignition Gazebo.
#
# <library_name> Required. Name of the gui library
#
# [SOURCES]: Specify the source files for the system.
#
# [QT_HEADERS]: Qt headers that need to be moc'ed
#
# [PUBLIC_LINK_LIBS]: Specify a list of libraries to be publicly linked.
#
# [PRIVATE_LINK_LIBS]: Specify a list of libraries to be privately linked.
#
function(gz_add_gui_library library_name)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs SOURCES QT_HEADERS PUBLIC_LINK_LIBS PRIVATE_LINK_LIBS)

  cmake_parse_arguments(gz_add_gui_library "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  QT5_WRAP_CPP(${library_name}_headers_MOC ${gz_add_gui_library_QT_HEADERS})
  QT5_ADD_RESOURCES(${library_name}_RCC ${library_name}.qrc)

  add_library(${library_name} SHARED
    ${gz_add_gui_library_SOURCES}
    ${${library_name}_headers_MOC}
    ${${library_name}_RCC}
  )
  target_link_libraries(${library_name}
    PUBLIC
      ${PROJECT_LIBRARY_TARGET_NAME}
      TINYXML2::TINYXML2
      ${gz_add_gui_library_PUBLIC_LINK_LIBS}
    PRIVATE
      ignition-gazebo${PROJECT_VERSION_MAJOR}-gui
      ${gz_add_gui_library_PRIVATE_LINK_LIBS}
  )
endfunction()


#################################################
# gz_add_gui_plugin(<plugin_name>
#              SOURCES <sources>
#              [PUBLIC_LINK_LIBS <libraries...>]
#              [PRIVATE_LINK_LIBS <libraries...>])
#
# Add a gui plugin to Ignition Gazebo.
#
# <plugin_name> Required. Name of the gui plugin.
#
# [SOURCES]: Specify the source files for the system.
#
# [QT_HEADERS]: Qt headers that need to be moc'ed
#
# [TEST_SOURCES]: Source files for unit tests.
#
# [PUBLIC_LINK_LIBS]: Specify a list of libraries to be publicly linked.
#
# [PRIVATE_LINK_LIBS]: Specify a list of libraries to be privately linked.
#
function(gz_add_gui_plugin plugin_name)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs
    SOURCES
    QT_HEADERS
    TEST_SOURCES
    PUBLIC_LINK_LIBS
    PRIVATE_LINK_LIBS
  )

  cmake_parse_arguments(gz_add_gui_plugin "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(MSVC)
    # Warning #4251 is the "dll-interface" warning that tells you when types
    # used by a class are not being exported. These generated source files have
    # private members that don't get exported, so they trigger this warning.
    # However, the warning is not important since those members do not need to
    # be interfaced with.
    set_source_files_properties(${gz_add_gui_plugin_SOURCES} COMPILE_FLAGS "/wd4251")
  endif()

  gz_add_gui_library(${plugin_name}
    SOURCES ${gz_add_gui_plugin_SOURCES}
    QT_HEADERS ${gz_add_gui_plugin_QT_HEADERS}
    PUBLIC_LINK_LIBS ${gz_add_gui_plugin_PUBLIC_LINK_LIBS}
    PRIVATE_LINK_LIBS ${gz_add_gui_plugin_PRIVATE_LINK_LIBS} ignition-plugin${IGN_PLUGIN_VER}::register
  )

  if(gz_add_gui_plugin_TEST_SOURCES)
    # Plugin symbols failing to resolve on Windows:
    # error LNK2001: unresolved external symbol
    if(NOT WIN32)
      ign_build_tests(TYPE UNIT
        SOURCES
          ${gz_add_gui_plugin_TEST_SOURCES}
        LIB_DEPS
          ignition-gazebo${PROJECT_VERSION_MAJOR}-gui
          ${plugin_name}
        INCLUDE_DIRS
          # Used to make internal source file headers visible to the unit tests
          ${CMAKE_CURRENT_SOURCE_DIR}
          # Used to make test-directory headers visible to the unit tests
          ${PROJECT_SOURCE_DIR}
          # Used to make test_config.h visible to the unit tests
          ${PROJECT_BINARY_DIR})
    endif()
  endif()

  install (TARGETS ${plugin_name} DESTINATION ${IGNITION_GAZEBO_GUI_PLUGIN_INSTALL_DIR})
endfunction()

add_subdirectory(modules)

# Plugins
add_subdirectory(align_tool)
add_subdirectory(banana_for_scale)
add_subdirectory(component_inspector)
add_subdirectory(entity_tree)
add_subdirectory(grid_config)
add_subdirectory(joint_position_controller)
add_subdirectory(lights)
add_subdirectory(playback_scrubber)
add_subdirectory(plot_3d)
add_subdirectory(plotting)
add_subdirectory(resource_spawner)
add_subdirectory(scene3d)
add_subdirectory(shapes)
add_subdirectory(tape_measure)
add_subdirectory(transform_control)
add_subdirectory(video_recorder)
add_subdirectory(view_angle)
add_subdirectory(visualize_contacts)
add_subdirectory(visualize_lidar)
