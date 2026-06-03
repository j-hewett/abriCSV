# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\abriCSV_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\abriCSV_autogen.dir\\ParseCache.txt"
  "abriCSV_autogen"
  )
endif()
