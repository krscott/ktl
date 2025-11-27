add_library(ktl ktl.c)

target_include_directories(ktl PUBLIC ${PROJECT_SOURCE_DIR}/include)
# target_sources(ktllib PRIVATE file.c)

install(TARGETS ktl)
