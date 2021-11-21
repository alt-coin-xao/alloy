file(GLOB_RECURSE BlockchainExplorer BlockchainExplorer/*)
file(GLOB_RECURSE Common Common/*)
file(GLOB_RECURSE Crypto Crypto/*)
file(GLOB_RECURSE CryptoNoteCore CryptoNoteCore/* Config/CryptoNoteConfig.h)
file(GLOB_RECURSE CryptoNoteProtocol CryptoNoteProtocol/*)
file(GLOB_RECURSE Daemon Daemon/*)
file(GLOB_RECURSE Http HTTP/*)
file(GLOB_RECURSE InProcessNode InProcessNode/*)
file(GLOB_RECURSE Logging Logging/*)
file(GLOB_RECURSE NodeRpcProxy NodeRpcProxy/*)
file(GLOB_RECURSE P2p P2p/*)
file(GLOB_RECURSE Rpc Rpc/*)
file(GLOB_RECURSE Serialization Serialization/*)
file(GLOB_RECURSE SimpleWallet SimpleWallet/*)
file(GLOB_RECURSE Transfers Transfers/*)
file(GLOB_RECURSE Wallet Wallet/*)
file(GLOB_RECURSE WalletLegacy WalletLegacy/*)
file(GLOB_RECURSE JsonRpcServer JsonRpcServer/*)
file(GLOB_RECURSE PaymentGate PaymentGate/*)
file(GLOB_RECURSE PaymentGateService PaymentGateService/*)
file(GLOB_RECURSE Miner Miner/*)

if(MSVC)
file(GLOB_RECURSE System System/* Platform/Windows/System/*)
###   #file(GLOB_RECURSE System System/* Platform/Windows/System/* Platform/msc/*)
elseif(APPLE)
file(GLOB_RECURSE System System/* Platform/OSX/System/* Platform/Posix/System/*)
else()
file(GLOB_RECURSE System System/* Platform/Linux/System/* Platform/Posix/System/*)
endif()

source_group("" FILES $${Common}  ${Crypto} ${CryptoNoteCore} ${CryptoNoteProtocol} ${Daemon} ${JsonRpcServer} ${Http} ${Logging} ${NodeRpcProxy} ${P2p} ${Rpc} ${Serialization} ${SimpleWallet} ${System} ${Transfers} ${Wallet} ${WalletLegacy})

add_library(BlockchainExplorer ${BlockchainExplorer})
add_library(Common ${Common})
add_library(Crypto ${Crypto})
add_library(Http ${Http})
add_library(InProcessNode ${InProcessNode})
add_library(JsonRpcServer ${JsonRpcServer})
add_library(Logging ${Logging})
add_library(NodeRpcProxy ${NodeRpcProxy})
add_library(PaymentGate ${PaymentGate})
add_library(Serialization ${Serialization})
add_library(System ${System})
add_library(Transfers ${Transfers})
add_library(Wallet ${Wallet} ${WalletLegacy})
add_library(Rpc ${Rpc})

add_library(CryptoNoteCore ${CryptoNoteCore})
target_link_libraries(CryptoNoteCore Boost::boost)
target_include_directories(CryptoNoteCore PUBLIC Boost_INCLUDE_DIR)

add_library(CryptoNoteProtocol ${CryptoNoteProtocol})

add_library(P2p ${P2p})
target_link_libraries(P2p Boost::boost CryptoNoteProtocol)
target_include_directories(P2p PUBLIC Boost_INCLUDE_DIR)

add_dependencies(P2p version)
add_dependencies(Rpc version)
target_link_libraries(P2p CryptoNoteCore Logging miniupnpc::miniupnpc)
target_link_libraries(Rpc CryptoNoteCore Logging P2p)
target_link_libraries(CryptoNoteCore Common Logging Crypto P2p Rpc Http Serialization System RocksDB::rocksdb)
target_link_libraries(Wallet ${CMAKE_THREAD_LIBS_INIT})

add_executable(Daemon ${Daemon})
add_dependencies(Daemon version)
if(MSVC)
  target_link_libraries(Daemon P2p Rpc Serialization System Http Logging CryptoNoteCore Crypto Common miniupnpc::miniupnpc RocksDB::rocksdb Boost::program_options Boost::filesystem Boost::boost Bcrypt)
else()
  target_link_libraries(Daemon P2p Rpc Serialization System Http Logging CryptoNoteCore Crypto Common miniupnpc::miniupnpc RocksDB::rocksdb Boost::program_options Boost::filesystem Boost::boost)
endif()
target_include_directories(Daemon PUBLIC Boost_INCLUDE_DIR)

add_executable(SimpleWallet ${SimpleWallet})
add_dependencies(SimpleWallet version)
target_link_libraries(SimpleWallet Wallet NodeRpcProxy Transfers Rpc P2p miniupnpc::miniupnpc Http Serialization CryptoNoteCore System Logging Common Crypto Boost::program_options Boost::filesystem ${CMAKE_THREAD_LIBS_INIT})

add_executable(PaymentGateService ${PaymentGateService})
target_link_libraries(PaymentGateService PaymentGate JsonRpcServer Wallet NodeRpcProxy Transfers P2p CryptoNoteCore Crypto Rpc Http Serialization System Logging Common InProcessNode BlockchainExplorer miniupnpc::miniupnpc RocksDB::rocksdb Boost::program_options Boost::filesystem ${CMAKE_THREAD_LIBS_INIT})
add_dependencies(PaymentGateService version)

add_executable(Miner ${Miner})
target_link_libraries(Miner CryptoNoteCore Rpc Serialization System Http Logging Common Crypto Boost::program_options Boost::filesystem ${CMAKE_THREAD_LIBS_INIT})

set_property(TARGET Daemon PROPERTY OUTPUT_NAME "alloyd")
set_property(TARGET SimpleWallet PROPERTY OUTPUT_NAME "simplewallet")
set_property(TARGET PaymentGateService PROPERTY OUTPUT_NAME "walletd")
set_property(TARGET Miner PROPERTY OUTPUT_NAME "miner")

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/targets" CACHE PATH "Install to build locastion" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

file(GLOB_RECURSE ALLOYINCLUDE_HDR ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE BLOCKCHAINEXPLORER_HDR BlockchainExplorer/*.h)
file(GLOB_RECURSE COMMON_HDR Common/*.h)
file(GLOB_RECURSE CRYPTONOTECORE_HDR CryptoNoteCore/*.h)
file(GLOB_RECURSE CRYPTONOTEPROTOCOL_HDR CryptoNoteProtocol/*.h)
file(GLOB_RECURSE CRYPTO_HDR Crypto/*.h)
file(GLOB_RECURSE HTTP_HDR HTTP/*.h)
file(GLOB_RECURSE INPROCESSNODE_HDR InProcessNode/*.h)
file(GLOB_RECURSE JSONRPCSERVER_HDR JsonRpcServer/*.h)
file(GLOB_RECURSE LOGGING_HDR Logging/*.h)
file(GLOB_RECURSE NODERPCPROXY_HDR NodeRpcProxy/*.h)
file(GLOB_RECURSE P2P_HDR P2p/*.h)
file(GLOB_RECURSE PAYMENTGATE_HDR PaymentGate/*.h)
file(GLOB_RECURSE RPC_HDR Rpc/*.h)
file(GLOB_RECURSE SERIALIZATION_HDR Serialization/*.h)
file(GLOB_RECURSE SYSTEM_HDR System/*.h)
file(GLOB_RECURSE TRANSFERS_HDR Transfers/*.h)
file(GLOB_RECURSE WALLET_HDR Wallet/*.h)

set_target_properties(Daemon PROPERTIES PUBLIC_HEADER "${ALLOYINCLUDE_HDR}")
set_target_properties(BlockchainExplorer PROPERTIES PUBLIC_HEADER "${BLOCKCHAINEXPLORER_HDR}")
set_target_properties(Common PROPERTIES PUBLIC_HEADER "${COMMON_HDR}")
set_target_properties(Crypto PROPERTIES PUBLIC_HEADER "${CRYPTO_HDR}")
set_target_properties(CryptoNoteCore PROPERTIES PUBLIC_HEADER "${CRYPTONOTECORE_HDR}")
set_target_properties(CryptoNoteProtocol PROPERTIES PUBLIC_HEADER "${CRYPTONOTEPROTOCOL_HDR}")
set_target_properties(Http PROPERTIES PUBLIC_HEADER "${HTTP_HDR}")
set_target_properties(InProcessNode PROPERTIES PUBLIC_HEADER "${INPROCESSNODE_HDR}")
set_target_properties(JsonRpcServer PROPERTIES PUBLIC_HEADER "${JSONRPCSERVER_HDR}")
set_target_properties(Logging PROPERTIES PUBLIC_HEADER "${LOGGING_HDR}")
set_target_properties(NodeRpcProxy PROPERTIES PUBLIC_HEADER "${NODERPCPROXY_HDR}")
set_target_properties(P2p PROPERTIES PUBLIC_HEADER "${P2P_HDR}")
set_target_properties(PaymentGate PROPERTIES PUBLIC_HEADER "${PAYMENTGATE_HDR}")
set_target_properties(Rpc PROPERTIES PUBLIC_HEADER "${RPC_HDR}")
set_target_properties(Serialization PROPERTIES PUBLIC_HEADER "${SERIALIZATION_HDR}")
set_target_properties(System PROPERTIES PUBLIC_HEADER "${SYSTEM_HDR}")
set_target_properties(Transfers PROPERTIES PUBLIC_HEADER "${TRANSFERS_HDR}")
set_target_properties(Wallet PROPERTIES PUBLIC_HEADER "${WALLET_HDR}")

INSTALL(
  TARGETS Daemon SimpleWallet PaymentGateService Miner
  CONFIGURATIONS Release
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/alloy
)

set(ALLOY_NAME_LIST "BlockchainExplorer" "Common" "Crypto" "CryptoNoteCore" "CryptoNoteProtocol" "Http" "InProcessNode" "JsonRpcServer" "Logging" "NodeRpcProxy" "P2p" "PaymentGate" "Rpc" "Serialization" "System" "Transfers" "Wallet")

foreach(ALLOY_LIB_NAME ${ALLOY_NAME_LIST})

INSTALL(
  TARGETS ${ALLOY_LIB_NAME}
  CONFIGURATIONS Release
  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/alloy
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/alloy
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/alloy/${ALLOY_LIB_NAME}
)

endforeach()

INSTALL(
  DIRECTORY Config/
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/alloy/Config
  FILES_MATCHING PATTERN "*.h")

if(MSVC)
elseif(APPLE)
else()
INSTALL(
  DIRECTORY Platform/Linux/System/
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/alloy/System
  FILES_MATCHING PATTERN "*.h")

INSTALL(
  DIRECTORY Platform/Posix/System/
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/alloy/System
  FILES_MATCHING PATTERN "*.h")
endif()
