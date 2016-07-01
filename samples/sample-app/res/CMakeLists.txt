ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/autoscroll.edj
	COMMAND edje_cc 
	${CMAKE_CURRENT_SOURCE_DIR}/edje/autoscroll.edc
	${CMAKE_BINARY_DIR}/autoscroll.edj
	oDEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/autoscroll.edc
)


SET(EDJDIR "${CMAKE_APP_RES_DIR}/edje")
SET(IMGDIR "${CMAKE_APP_RES_DIR}/images")

# ADD_CUSTOM_TARGET(button_view_edj_build DEPENDS ${CMAKE_BINARY_DIR}/button_view.edj)
ADD_CUSTOM_TARGET(autoscroll_edj_build DEPENDS ${CMAKE_BINARY_DIR}/autoscroll.edj)

ADD_DEPENDENCIES(${PROJECT_NAME} button_view_edj_build)
ADD_DEPENDENCIES(${PROJECT_NAME} autoscroll_edj_build)

ADD_SUBDIRECTORY(po)

# INSTALL(FILES ${CMAKE_BINARY_DIR}/button_view.edj DESTINATION ${EDJDIR})
INSTALL(FILES ${CMAKE_BINARY_DIR}/autoscroll.edj DESTINATION ${EDJDIR})

