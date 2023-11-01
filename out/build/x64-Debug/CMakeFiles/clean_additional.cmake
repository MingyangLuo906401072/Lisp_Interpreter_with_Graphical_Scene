# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\sldraw_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\sldraw_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\slisp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\slisp_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\test_gui_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\test_gui_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\test_message_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\test_message_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\unittests_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\unittests_autogen.dir\\ParseCache.txt"
  "sldraw_autogen"
  "slisp_autogen"
  "test_gui_autogen"
  "test_message_autogen"
  "unittests_autogen"
  )
endif()
