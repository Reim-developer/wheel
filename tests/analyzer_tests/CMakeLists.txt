setup_wheel_test(
    tab_analyzer_test tab_analyzer_test.cxx
    ${CMAKE_SOURCE_DIR}/analyzer/src/tab.cxx
)

target_link_libraries(tab_analyzer_test PRIVATE analyzer)
set_property(TARGET tab_analyzer_test PROPERTY CXX_STANDARD 20)