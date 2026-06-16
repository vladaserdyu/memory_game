# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\MemoryGame_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MemoryGame_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\tests_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\tests_autogen.dir\\ParseCache.txt"
  "MemoryGame_autogen"
  "tests_autogen"
  )
endif()
