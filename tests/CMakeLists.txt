# Adiciona o GoogleTest via FetchContent
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Coleta automaticamente os arquivos de teste
file(GLOB TEST_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Remove UI/persistence/billing tests moved to mobile project
list(REMOVE_ITEM TEST_SRC 
    "${CMAKE_CURRENT_SOURCE_DIR}/AndroidAuthPersistenceTests.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/StoreUITests.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ThemeTests.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TutorialUITests.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/BillingTests.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/AuthTests.cpp"
)

# Cria o executável de testes
add_executable(ScoundrelTests ${TEST_SRC})

# Optionally add SDL includes/linking when requested by the consumer
if(BUILD_WITH_SDL)
    # Adiciona diretórios de inclusão do SDL2 para os testes que incluem AndroidUI.h
    target_include_directories(ScoundrelTests PRIVATE
        ${SDL2_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
    )

    # Linka com GoogleTest e a biblioteca do jogo
    target_link_libraries(ScoundrelTests PRIVATE 
        ScoundrelCore 
        gtest 
        gtest_main 
        gmock 
        gmock_main
        SDL2-static
        SDL2_mixer
        SDL2_image
    )
else()
    target_link_libraries(ScoundrelTests PRIVATE ScoundrelCore gtest gtest_main gmock gmock_main)
endif()

# Registra o conjunto de testes
add_test(NAME ScoundrelTests COMMAND ScoundrelTests)
