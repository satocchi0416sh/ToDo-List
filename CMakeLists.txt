cmake_minimum_required(VERSION 3.10)
project(ToDo_List)

# オプションの設定：テストビルドを有効にする
option(BUILD_TESTS "Build tests" ON)
option(CODE_COVERAGE "Enable coverage reporting" ON)

# CTestをプロジェクトに追加
include(CTest)
enable_testing()

# wxWidgetsを探す
find_package(wxWidgets REQUIRED COMPONENTS core base richtext)
include(${wxWidgets_USE_FILE})

# ソースファイルを追加
set(SOURCES
    cApp.cpp
    MainWindow.cpp
    Task.cpp
)

# 実行ファイルの設定
add_executable(ToDo_List ${SOURCES})

# wxWidgetsのライブラリをリンク
target_link_libraries(ToDo_List ${wxWidgets_LIBRARIES})

# C++11標準を使用
set_target_properties(ToDo_List PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# テストのビルドを有効にした場合の処理
if(BUILD_TESTS)
    # Google Testのダウンロード・設定
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    # Google Testのビルド設定
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # コードカバレッジのフラグを設定
    if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # カバレッジフラグを設定
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    endif()

    # テスト用の実行ファイル
    add_executable(ToDo_Tests tests/task_tests.cpp Task.cpp)
    
    # テスト実行ファイルにGoogle Testをリンク
    target_link_libraries(ToDo_Tests GTest::gtest_main)
    
    # テストのインクルードディレクトリを追加
    target_include_directories(ToDo_Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    
    # テストを実行可能にする
    include(GoogleTest)
    gtest_discover_tests(ToDo_Tests)
    
    # CTestにテストを直接追加
    add_test(NAME TaskTests COMMAND ToDo_Tests)
endif() 