file(GLOB STRUCT_SRC "src/*.cc")
add_library(struct SHARED ${STRUCT_SRC})
target_include_directories(struct PUBLIC include)