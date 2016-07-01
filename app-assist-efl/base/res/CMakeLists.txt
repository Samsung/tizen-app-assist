CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# install edj
ADD_CUSTOM_TARGET(app-assist-efl.edj
		COMMAND edje_cc -id ${CMAKE_CURRENT_SOURCE_DIR}/images ${EDJE_CFLAGS}
		${CMAKE_CURRENT_SOURCE_DIR}/edje/app-assist-efl.edc app-assist-efl.edj
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/edje/app-assist-efl.edc
)
ADD_DEPENDENCIES(${PROJECT_NAME} app-assist-efl.edj)
INSTALL(FILES app-assist-efl.edj DESTINATION ${APP_ASSIST_EDJ_PATH})
