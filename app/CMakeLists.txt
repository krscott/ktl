add_executable(c-start-app main.c)

target_link_libraries(c-start-app cstart libkcli.a)

install(TARGETS c-start-app)
