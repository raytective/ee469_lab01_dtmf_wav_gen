cmake_minimum_required(VERSION 3.22)
project(ee469_lab01_dtmf_wav_gen C)

set(CMAKE_C_STANDARD 99)

add_executable(ee469_lab01_dtmf_wav_gen main.c)

target_link_libraries(ee469_lab01_dtmf_wav_gen m)
