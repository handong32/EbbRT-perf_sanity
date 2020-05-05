project("perf_sanity-ebbrt" CXX)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g3")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -flto -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall -Werror")

include_directories(${PROJECT_SRC_DIR})
add_executable(perf_sanity.elf src/perf_sanity.cc)
add_executable(multicore_perf.elf src/multicore_perf.cc src/MulticorePerf.cc)

add_custom_command(TARGET perf_sanity.elf POST_BUILD
  COMMAND objcopy -O elf32-i386 perf_sanity.elf perf_sanity.elf32 )

add_custom_command(TARGET multicore_perf.elf POST_BUILD
  COMMAND objcopy -O elf32-i386 multicore_perf.elf multicore_perf.elf32 )
