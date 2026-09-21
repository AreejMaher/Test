# Set the minimum version of CMake that can be used
cmake_minimum_required(VERSION 3.20)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

# Set target definition variables required by the toolchain file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/target.cmake)

# Set the toolchain file for cross-compilation
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/gcc-arm-none-eabi.cmake)

# Project definition
project(Test)

# Enable CMake support for ASM and C/CPP languages
enable_language(C ASM)

set(PROJECT_NAME Test)

# Create an executable object type
add_executable(${PROJECT_NAME})

# Tell the compiler you are using a Cortex-M4 with a Floating Point Unit (FPU)
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    ARM_MATH_CM4
    __FPU_PRESENT=1
)

# Include additional CMake files for flags, components, and files
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/flags.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/components.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/files.cmake)

# Add sources to executable/library
target_sources(${PROJECT_NAME} PRIVATE
    # --- Filtering: biquad DF2T (matches ssvep_dsp.c's notch + bandpass) ---
    "CMSIS-DSP/Source/FilteringFunctions/arm_biquad_cascade_df2T_f32.c"
    "CMSIS-DSP/Source/FilteringFunctions/arm_biquad_cascade_df2T_init_f32.c"

    # --- Transform: real FFT (power spectrum) ---
    "CMSIS-DSP/Source/TransformFunctions/arm_rfft_fast_f32.c"
    "CMSIS-DSP/Source/TransformFunctions/arm_rfft_fast_init_f32.c"
    "CMSIS-DSP/Source/TransformFunctions/arm_cfft_f32.c"
    "CMSIS-DSP/Source/TransformFunctions/arm_cfft_radix8_f32.c"
    "CMSIS-DSP/Source/TransformFunctions/arm_bitreversal2.c"

    # --- Shared tables ---
    "CMSIS-DSP/Source/CommonTables/arm_common_tables.c"
    "CMSIS-DSP/Source/CommonTables/arm_const_structs.c"
)

# Add include directories (unchanged from your original — these were correct)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS-DSP/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS-DSP/PrivateInclude
)

# Compiler options
target_compile_options(${PROJECT_NAME} PRIVATE
  # User defined compiler options
)

# Add additional defined symbols
target_compile_definitions(${PROJECT_NAME} PRIVATE
  # Add additional defined symbols here
)

# Linker options
target_link_options(${PROJECT_NAME} PRIVATE 
  # User defined linker options
)

# Add additional linker search directories
target_link_directories(${PROJECT_NAME} PRIVATE
  # Add additional linker search directories here
)

# Add additional linker libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
  # Add additional linker libraries here
)