cmake_minimum_required(VERSION 2.8)
project (datvexpress)

SET(GCC_COVERAGE_COMPILE_FLAGS "-pedantic -Wall -Wextra")
# -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable (encoder
    encoder.cpp
    DVB-S2/dvb2_bch.cpp
    DVB-S2/dvb2_bbheader.cpp
    DVB-S2/DVB2.cpp
    DVB-S2/dvb2_ldpc_encode.cpp
    DVB-S2/dvb2_ldpc_tables.cpp
    DVB-S2/dvb2_scrambler.cpp
    DVB-S2/DVBS2.cpp
    DVB-S2/dvbs2_interleave.cpp
    DVB-S2/dvbs2_modulator.cpp
    DVB-S2/dvbs2_physical.cpp
    DVB-S2/dvbs2_scrambler.cpp
    DVB-S2/dvbs2_tables.cpp
)

include_directories(DVB-S2)
