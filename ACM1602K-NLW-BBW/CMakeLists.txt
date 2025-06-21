add_library(acm1602k_lib ACM1602K-NLW-BBW.c)

target_include_directories(acm1602k_lib 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)