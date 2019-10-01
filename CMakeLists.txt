CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
PROJECT(PA1 CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

ADD_SUBDIRECTORY(deps/vecmath)
ADD_SUBDIRECTORY(deps/image)

INCLUDE_DIRECTORIES(deps/vecmath/include)
INCLUDE_DIRECTORIES(deps/image/include)
INCLUDE_DIRECTORIES(include)

SET(SOURCES  
             src/ray.cpp
             src/color.cpp
             src/light.cpp
             src/collision.cpp
             src/entities.cpp
             src/materials.cpp
             src/manager.cpp
             src/camera.cpp
             src/presenter.cpp
             src/detector.cpp
             src/main.cpp)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES} ${INCLUDES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} vecmath image)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE include)
