# CMakeList.txt: VulkanTest03 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # 禁用编译器扩展

# 查找 Vulkan 包
find_package(Vulkan REQUIRED)
get_filename_component(Vulkan_LIBRARY_DIR ${Vulkan_LIBRARIES} DIRECTORY)

include_directories(${Vulkan_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/third_party/glm)
include_directories(${CMAKE_SOURCE_DIR}/third_party/glfw-3.4.bin.WIN64/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/stb-master)
include_directories(${CMAKE_SOURCE_DIR}/third_party/tinyobjloader-release)

link_directories(${CMAKE_SOURCE_DIR}/third_party/glfw-3.4.bin.WIN64/lib-vc2022)
link_directories(${Vulkan_LIBRARY_DIR})

# 将源代码添加到此项目的可执行文件。
add_executable (VulkanTest03 "VulkanTest03.cpp" "VulkanTest03.h")

target_link_libraries(${PROJECT_NAME} vulkan-1.lib)
target_link_libraries(${PROJECT_NAME} glfw3.lib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VulkanTest03 PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
