

# find grpc
message("researching for grpc")
set(gRPC_INCLUDE_DIR ${GRPC_ROOT}/include)
find_library(gRPC_ADDRESS_SORTING_LIBRARY      libaddress_sorting.a ${GRPC_ROOT}/lib NO_DEFAULT_PATH)
find_library(gRPC_GPR_LIBRARY                  libgpr.a ${GRPC_ROOT}/lib NO_DEFAULT_PATH)
find_library(gRPC_GRPCPP_LIBRARY               libgrpcpp.a ${GRPC_ROOT}/lib NO_DEFAULT_PATH)
find_library(gRPC_GRPCPP_ERROR_DETAILS_LIBRARY libgrpcpp_error_details.a ${GRPC_ROOT}/lib NO_DEFAULT_PATH)
find_library(gRPC_GRPCPP_REFLECTION_LIBRARY    libgrpcpp_reflection.a ${GRPC_ROOT}/lib NO_DEFAULT_PATH)
find_library(gRPC_GRPCPP_UNSECURE_LIBRARY      libgrpcpp_unsecure.a ${GRPC_ROOT}/lib NO_DEFAULT_PATH)
find_library(gRPC_GRPC_LIBRARY                 libgrpc.a ${GRPC_ROOT}/lib NO_DEFAULT_PATH)
find_library(gRPC_GRPCPP_CHANNELZ_LIBRARY      libgrpcpp_channelz.a ${GRPC_ROOT}/lib NO_DEFAULT_PATH)
find_library(gRPC_GRPC_CRONET_LIBRARY          libgrpc_cronet.a ${GRPC_ROOT}/lib NO_DEFAULT_PATH)
find_library(gRPC_GRPC_UNSECURE_LIBRARY        libgrpc_unsecure.a ${GRPC_ROOT}/lib NO_DEFAULT_PATH)

set(GRPC_STATIC_LIBRARY
        ${gRPC_ADDRESS_SORTING_LIBRARY}
        ${gRPC_GPR_LIBRARY}
        ${gRPC_GRPCPP_LIBRARY}
        ${gRPC_GRPCPP_ERROR_DETAILS_LIBRARY}
        ${gRPC_GRPCPP_REFLECTION_LIBRARY}
        ${gRPC_GRPCPP_UNSECURE_LIBRARY}
        ${gRPC_GRPC_LIBRARY}
        ${gRPC_GRPCPP_CHANNELZ_LIBRARY}
        ${gRPC_GRPC_CRONET_LIBRARY}
        ${gRPC_GRPC_UNSECURE_LIBRARY}
        ${zlib}
        )

find_library(gRPC_GRPCPP_UNSECURE_LIBRARY_SHARED grpcpp_unsecure ${GRPC_ROOT}/lib NO_DEFAULT_PATH)

message(STATUS "gRPC_GPR_LIBRARY : " ${gRPC_GPR_LIBRARY})
message(STATUS "gRPC_GRPCPP_LIBRARY : " ${gRPC_GRPCPP_LIBRARY})
message(STATUS "gRPC_GRPCPP_ERROR_DETAILS_LIBRARY : " ${gRPC_GRPCPP_ERROR_DETAILS_LIBRARY})
message(STATUS "gRPC_GRPCPP_REFLECTION_LIBRARY : " ${gRPC_GRPCPP_REFLECTION_LIBRARY})
message(STATUS "gRPC_GRPCPP_UNSECURE_LIBRARY : " ${gRPC_GRPCPP_UNSECURE_LIBRARY})
message(STATUS "gRPC_GRPC_LIBRARY : " ${gRPC_GRPC_LIBRARY})
message(STATUS "gRPC_GRPCPP_CHANNELZ_LIBRARY : " ${gRPC_GRPCPP_CHANNELZ_LIBRARY})
message(STATUS "gRPC_GRPC_CRONET_LIBRARY : " ${gRPC_GRPC_CRONET_LIBRARY})
message(STATUS "gRPC_GRPC_UNSECURE_LIBRARY : " ${gRPC_GRPC_UNSECURE_LIBRARY})

message(STATUS "gRPC_GRPCPP_UNSECURE_LIBRARY_SHARED : " ${gRPC_GRPCPP_UNSECURE_LIBRARY_SHARED})

foreach (Camel
        gRPC_INCLUDE_DIR
        gRPC_GRPCPP_UNSECURE_LIBRARY
        )
    string(TOUPPER ${Camel} UPPER)
    set(${UPPER} ${${Camel}})
endforeach ()