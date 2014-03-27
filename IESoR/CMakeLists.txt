#set(IESOR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(IESOR_World_SRCS
	Simulation/iesorWorld.cpp
)
set(IESOR_World_HDRS
	Simulation/iesorWorld.h
)

set(IESOR_Body_SRCS
	Network/iesorBody.cpp
	Network/network.cpp
)
set(IESOR_Body_HDRS
	Network/iesorBody.h
	Network/network.h
)

set(IESOR_Init_SRCS
	Simulation/Initialize/initializeWorld.cpp
)
set(IESOR_Init_HDRS
	Simulation/Initialize/initializeWorld.h
)

set(IESOR_Record_SRCS
	Simulation/Record/recordWorld.cpp
)
set(IESOR_Record_HDRS
	Simulation/Record/recordWorld.h
)

set(IESOR_Construction_SRCS
	Construction/Bodies/outputsToBodies.cpp
	Construction/GPU/gpuBodyBuilder.cpp
)
set(IESOR_Construction_HDRS
	Construction/Bodies/outputsToBodies.h
	Construction/GPU/gpuBodyBuilder.h
)

set(IESOR_General_HDRS
	iesor.h
)

#need to include json source files as well
include_directories( ../ ${JSON_SOURCE_DIR})



if(IESOR_BUILD_SHARED)
	add_library(IESoR_shared SHARED
		${IESOR_General_HDRS}
		${IESOR_World_HDRS}
		${IESOR_World_SRCS}
		${IESOR_Body_HDRS}
		${IESOR_Body_SRCS}
		${IESOR_Record_HDRS}
		${IESOR_Record_SRCS}
		${IESOR_Init_HDRS}
		${IESOR_Init_SRCS}
		${IESOR_Construction_HDRS}
		${IESOR_Construction_SRCS}
		
		)
		
		
	#need to link the JSON library in
	target_link_libraries (IESoR_shared)
	
	set_target_properties(IESoR_shared PROPERTIES
		OUTPUT_NAME "IESoR"
		CLEAN_DIRECT_OUTPUT 1
		VERSION ${IESOR_VERSION}
	)
	
	
endif()

if(IESOR_BUILD_STATIC)
	add_library(IESoR STATIC
		${IESOR_General_HDRS}
		${IESOR_World_HDRS}
		${IESOR_World_SRCS}
		${IESOR_Body_HDRS}
		${IESOR_Body_SRCS}
		${IESOR_Record_HDRS}
		${IESOR_Record_SRCS}
		${IESOR_Init_HDRS}
		${IESOR_Init_SRCS}
		${IESOR_Construction_HDRS}
		${IESOR_Construction_SRCS}
	)
	
	#need to link the JSON library in
	target_link_libraries (IESoR)
	
	set_target_properties(IESoR PROPERTIES
		CLEAN_DIRECT_OUTPUT 1
		VERSION ${IESOR_VERSION}
	)
	
	
endif()

# These are used to create visual studio folders.
source_group(Simulation FILES ${IESOR_World_SRCS} ${IESOR_World_HDRS})
source_group(Simulation\\Initialize FILES ${IESOR_Init_SRCS} ${IESOR_Init_HDRS})
source_group(Simulation\\Record FILES ${IESOR_Record_SRCS} ${IESOR_Record_HDRS})
source_group(Construction FILES ${IESOR_Construction_SRCS} ${IESOR_Construction_HDRS})
source_group(Network FILES ${IESOR_Body_SRCS} ${IESOR_Body_HDRS})
source_group(Include FILES ${IESOR_General_HDRS})

#Think about adding in iesor installation for linux systems









