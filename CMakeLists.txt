cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(sealjs VERSION 0.0.1 LANGUAGES CXX)

include(NodeJS.cmake)
nodejs_init()

add_nodejs_module(sealjs addon.cc myobject.cc sealcontext.cc keygenerator.cc encryptor.cc encoder.cc plaintext.cc ciphertext.cc evaluator.cc decryptor.cc)

# Import SEAL
find_package(SEAL 3.0.0 EXACT REQUIRED)

# Link SEAL
target_link_libraries(sealjs SEAL::seal)
target_include_directories(sealjs PUBLIC "node_modules/node-addon-api")
