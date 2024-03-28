#aux_source_directory(. SRC_LIST)
set(SRC_LIST myrpcapplication.cc myrpcconfig.cc rpcheader.pb.cc rpcprovider.cc myrpcchannel.cc)
add_library(myrpc ${SRC_LIST})
target_link_libraries(myrpc muduo_net muduo_base pthread)