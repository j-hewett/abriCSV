add_library(sqlite3 STATIC sqlite3.c)

target_include_directories(sqlite3 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_definitions(sqlite3 PUBLIC
    SQLITE_THREADSAFE=1
    SQLITE_DQS=0
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sqlite3 PRIVATE -w)
elseif (MSVC)
    target_compile_options(sqlite3 PRIVATE /w)
endif()

add_library(SQLite::SQLite3 ALIAS sqlite3)