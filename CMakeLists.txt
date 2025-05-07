cmake_minimum_required(VERSION 3.30)
project(VulkanTutorial)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)

#SDL IS BEING STATIC LINKED AS OF NOW. IF IT EVER NEEDS TO BE A SEPARATE DLL, IT HAS TO BE COPIED ONTO BUILD DIR EACH TIME IT IS BUILT
set(SDL_STATIC ON)
set(SDL_SHARED OFF)

add_subdirectory(libs/SDL EXCLUDE_FROM_ALL)

add_executable(VulkanTutorial src/VulkanTutorial/vulkanTutorial.cpp src/VulkanTutorial/vulkanTutorialMain.cpp)
target_link_libraries(VulkanTutorial PRIVATE ${Vulkan_LIBRARIES})
target_include_directories(VulkanTutorial PRIVATE ${Vulkan_INCLUDE_DIRS})
target_include_directories(VulkanTutorial PUBLIC ./include)
target_link_libraries(VulkanTutorial PRIVATE SDL3::SDL3)

add_executable(VulkanTutorialTest
        test/VulkanTutorial/test.cpp)
target_include_directories(VulkanTutorialTest PUBLIC include)
