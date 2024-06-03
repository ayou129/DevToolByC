# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\DevTool_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\DevTool_autogen.dir\\ParseCache.txt"
  "DevTool_autogen"
  )
endif()
