add_executable(wheel_cli)

target_sources(
    wheel_cli
    PRIVATE
        main.cxx
)

target_link_libraries(
    wheel_cli
    PRIVATE
        wheel_interpreter
        wheel_semantic
        wheel_parser
)

set_target_properties(
    wheel_cli
    PROPERTIES
        OUTPUT_NAME wheel
)

include(GNUInstallDirs)

install(
    TARGETS wheel_cli
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
