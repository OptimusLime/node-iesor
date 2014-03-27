#set(JSON_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(JSON_HDRS
	src/autolink.h
	src/json_batchallocator.h
	src/config.h
	src/features.h
	src/forwards.h
	src/reader.h
	src/value.h
	src/writer.h
	
	src/json_tool.h
	
	src/json_internalarray.inl
	src/json_internalmap.inl
	src/json_valueiterator.inl	
)
set(JSON_SRCS
	src/json_reader.cpp
	src/json_value.cpp
	src/json_writer.cpp
)

set(JSON_General_HDRS
	json.h
)
include_directories( ../ )

if(JSON_BUILD_SHARED)
	add_library(JSON_shared SHARED
		${JSON_General_HDRS}
		${JSON_HDRS}
		${JSON_SRCS}
		)
	set_target_properties(JSON_shared PROPERTIES
		OUTPUT_NAME "JSON"
		CLEAN_DIRECT_OUTPUT 1
		VERSION ${JSON_VERSION}
	)
endif()

if(JSON_BUILD_STATIC)
	add_library(JSON STATIC
		${JSON_General_HDRS}
		${JSON_HDRS}
		${JSON_SRCS}
	)
	set_target_properties(JSON PROPERTIES
		CLEAN_DIRECT_OUTPUT 1
		VERSION ${JSON_VERSION}
	)
endif()

# These are used to create visual studio folders.
source_group(Src FILES ${JSON_SRCS} ${JSON_HDRS})
source_group(Include FILES ${JSON_General_HDRS})

#Think about adding in json installation for linux systems









