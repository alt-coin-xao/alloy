#add_definitions(-DSTATICLIB)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ../version)

file(GLOB_RECURSE CoreTests CoreTests/*)
file(GLOB_RECURSE CryptoTests Crypto/*)
file(GLOB_RECURSE FunctionalTests FunctionalTests/*)
file(GLOB_RECURSE IntegrationTestLibrary IntegrationTestLib/*)
file(GLOB_RECURSE IntegrationTests IntegrationTests/*)
file(GLOB_RECURSE NodeRpcProxyTests NodeRpcProxyTests/*)
file(GLOB_RECURSE PerformanceTests PerformanceTests/*)
file(GLOB_RECURSE SystemTests System/*)
file(GLOB_RECURSE TestGenerator TestGenerator/*)
file(GLOB_RECURSE TransfersTests TransfersTests/*)
file(GLOB_RECURSE UnitTests UnitTests/*)
file(GLOB_RECURSE TestsCommon Common/*)

source_group("" FILES ${CryptoTests} ${FunctionalTests} ${IntegrationTestLibrary} ${IntegrationTests} ${NodeRpcProxyTests} ${PerformanceTests} ${SystemTests} ${TestGenerator} ${TransfersTests} ${UnitTests})

add_library(IntegrationTestLibrary ${IntegrationTestLibrary})
add_library(TestGenerator ${TestGenerator})
add_library(UnitTestsLib ${UnitTests})
add_library(TestsCommon ${TestsCommon})

add_executable(CoreTests ${CoreTests})
add_executable(CryptoTests ${CryptoTests})
add_executable(IntegrationTests ${IntegrationTests})
add_executable(NodeRpcProxyTests ${NodeRpcProxyTests})
add_executable(PerformanceTests ${PerformanceTests})
add_executable(SystemTests ${SystemTests})
add_executable(TransfersTests ${TransfersTests})
add_executable(UnitTests ${UnitTests})
add_executable(DifficultyTests Difficulty/Difficulty.cpp)
add_executable(HashTargetTests HashTarget.cpp)
add_executable(HashTests Hash/main.cpp)

target_link_libraries(CoreTests
  PRIVATE GTest::gtest_main GTest::gtest
  PUBLIC TestGenerator TestsCommon CryptoNoteCore Serialization System Logging Common Crypto BlockchainExplorer UnitTestsLib Boost::program_options Boost::filesystem Boost::boost Boost::serialization Boost::boost)

target_link_libraries(IntegrationTests
  PRIVATE GTest::gtest_main GTest::gtest
  PUBLIC IntegrationTestLibrary TestsCommon Wallet NodeRpcProxy InProcessNode P2p Rpc Http Transfers Serialization System CryptoNoteCore Logging Common Crypto BlockchainExplorer miniupnpc::miniupnpc Boost::program_options Boost::filesystem Boost::boost)

target_link_libraries(NodeRpcProxyTests
  PRIVATE GTest::gtest_main GTest::gtest
  PUBLIC NodeRpcProxy CryptoNoteCore Rpc Http Serialization System Logging Common Crypto)

target_link_libraries(PerformanceTests
  PRIVATE GTest::gtest_main GTest::gtest
  PUBLIC CryptoNoteCore Serialization Logging Common Crypto Boost::program_options Boost::filesystem Boost::chrono Boost::boost)

target_link_libraries(SystemTests
  PRIVATE GTest::gtest_main GTest::gtest
  PUBLIC System)


if (MSVC)
  target_link_libraries(SystemTests ws2_32)
  target_link_libraries(NodeRpcProxyTests ws2_32)
  target_link_libraries(CoreTests ws2_32)
endif ()

target_link_libraries(TransfersTests
  PRIVATE GTest::gtest_main GTest::gtest
  PUBLIC IntegrationTestLibrary TestsCommon Wallet InProcessNode NodeRpcProxy P2p Rpc Http BlockchainExplorer CryptoNoteCore Serialization System Logging Transfers Common Crypto miniupnpc::miniupnpc Boost::program_options Boost::filesystem Boost::boost)

target_link_libraries(UnitTests
  PRIVATE GTest::gtest_main GTest::gtest
  PUBLIC PaymentGate Wallet TestGenerator InProcessNode NodeRpcProxy Rpc P2p miniupnpc::miniupnpc Http Transfers Serialization System Logging BlockchainExplorer CryptoNoteCore Common Crypto Boost::program_options Boost::filesystem Boost::boost)

target_link_libraries(DifficultyTests
  PRIVATE GTest::gtest_main GTest::gtest
  PUBLIC CryptoNoteCore Serialization Crypto Logging Common Boost::program_options Boost::filesystem Boost::boost)

target_link_libraries(HashTargetTests
  PRIVATE GTest::gtest_main GTest::gtest
  PUBLIC CryptoNoteCore Crypto)

target_link_libraries(HashTests
  PRIVATE GTest::gtest_main GTest::gtest
  PUBLIC Crypto)

if(NOT MSVC)
  set_property(TARGET GTest::gtest_main GTest::gtest IntegrationTestLibrary IntegrationTests TestGenerator UnitTests SystemTests HashTargetTests TransfersTests APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-undef -Wno-sign-compare")
endif()

if(TESTS_WITH_ALL)
add_custom_target(tests ALL
  COMMAND ${CMAKE_CTEST_COMMAND}
  DEPENDS CoreTests CryptoTests IntegrationTests NodeRpcProxyTests PerformanceTests SystemTests TransfersTests UnitTests DifficultyTests HashTargetTests HashTests)
else()
add_custom_target(tests
  COMMAND ${CMAKE_CTEST_COMMAND}
  DEPENDS CoreTests CryptoTests IntegrationTests NodeRpcProxyTests PerformanceTests SystemTests TransfersTests UnitTests DifficultyTests HashTargetTests HashTests)
endif(TESTS_WITH_ALL)

set_property(TARGET
  tests

  IntegrationTestLibrary
  TestGenerator

  CoreTests
  CryptoTests
  IntegrationTests
  NodeRpcProxyTests
  PerformanceTests
  SystemTests
  TransfersTests
  UnitTests

  DifficultyTests
  HashTargetTests
  HashTests
PROPERTY FOLDER "tests")

add_dependencies(IntegrationTestLibrary version)

set_property(TARGET CoreTests PROPERTY OUTPUT_NAME "core_tests")
set_property(TARGET CryptoTests PROPERTY OUTPUT_NAME "crypto_tests")
set_property(TARGET IntegrationTests PROPERTY OUTPUT_NAME "integration_tests")
set_property(TARGET NodeRpcProxyTests PROPERTY OUTPUT_NAME "node_rpc_proxy_tests")
set_property(TARGET PerformanceTests PROPERTY OUTPUT_NAME "performance_tests")
set_property(TARGET SystemTests PROPERTY OUTPUT_NAME "system_tests")
set_property(TARGET TransfersTests PROPERTY OUTPUT_NAME "transfers_tests")
set_property(TARGET UnitTests PROPERTY OUTPUT_NAME "unit_tests")
set_property(TARGET DifficultyTests PROPERTY OUTPUT_NAME "difficulty_tests")
set_property(TARGET HashTargetTests PROPERTY OUTPUT_NAME "hash_target_tests")
set_property(TARGET HashTests PROPERTY OUTPUT_NAME "hash_tests")

add_test(CoreTests core_tests --generate_and_play_test_data)
add_test(CryptoTests crypto_tests ${CMAKE_CURRENT_SOURCE_DIR}/crypto/tests.txt)
add_test(DifficultyTests difficulty_tests ${CMAKE_CURRENT_SOURCE_DIR}/Difficulty/data.txt)
foreach(hash IN ITEMS fast slow tree extra-blake extra-groestl extra-jh extra-skein)
  add_test(hash-${hash} hash_tests ${hash} ${CMAKE_CURRENT_SOURCE_DIR}/Hash/tests-${hash}.txt)
endforeach(hash)
add_test(HashTargetTests hash_target_tests)
add_test(SystemTests system_tests)
## TODO (GCJ) look at these long running tests
add_test(UnitTests unit_tests --gtest_filter=*-BcSTest.*Synchron*:BcSTest.outdatedTxsRemovedOnlyAtFirstStart)
#add_test(UnitTests unit_tests)
