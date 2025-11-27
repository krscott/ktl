add_executable(ktl-app main.c)

target_link_libraries(ktl-app libkcli.a)

install(TARGETS ktl-app)
