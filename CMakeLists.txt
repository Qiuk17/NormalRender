CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
PROJECT(PA3 CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PATH(OPENGL_INCLUDE_SUBDIR
        NAMES gl.h
        PATHS ${OPENGL_INCLUDE_DIR}
        PATH_SUFFIXES GL OpenGL Headers
        NO_DEFAULT_PATH)

FIND_PACKAGE(GLUT REQUIRED)
FIND_PATH(GLUT_INCLUDE_SUBDIR
        NAMES glut.h
        PATHS ${GLUT_INCLUDE_DIR}
        PATH_SUFFIXES GL GLUT Headers
        NO_DEFAULT_PATH)

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
             src/scene.cpp
             src/camera.cpp
             src/presenter.cpp
             src/detector.cpp
             src/parser.cpp
             src/main.cpp)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "-g -Wno-deprecated-declarations")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES} ${INCLUDES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} vecmath image ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE include ${OPENGL_INCLUDE_SUBDIR} ${GLUT_INCLUDE_SUBDIR})
