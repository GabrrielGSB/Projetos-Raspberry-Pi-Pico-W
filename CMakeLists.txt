#============================================================
#        CONFIGURAÇÃO PADRÃO DO PROJETO E AMBIENTE
    # Generated Cmake Pico project file
    cmake_minimum_required(VERSION 3.13)

    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # Initialise pico_sdk from installed location
    # (note this can come from environment, CMake cache etc)

    # == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
    if(WIN32)
        set(USERHOME $ENV{USERPROFILE})
    else()
        set(USERHOME $ENV{HOME})
    endif()
    set(sdkVersion 2.2.0)
    set(toolchainVersion 14_2_Rel1)
    set(picotoolVersion 2.2.0-a4)
    set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
    if (EXISTS ${picoVscode})
        include(${picoVscode})
    endif()

    set(PICO_BOARD pico_w CACHE STRING "Board type")

    # Pull in Raspberry Pi Pico SDK (must be before project)
    include(pico_sdk_import.cmake)

    project(Teste C CXX ASM)

    # Initialise the Raspberry Pi Pico SDK
    pico_sdk_init()
#============================================================

#============================================================
#             ADIÇÃO DE BIBLIOTECAS EXTERNAS

    # Configuração da LIB arduinoStyle
        add_library(arduinoStyle libs/arduinoStyle.c)

        target_link_libraries(arduinoStyle 
                               pico_stdlib
                               hardware_pwm)

        target_include_directories(arduinoStyle PUBLIC libs)

    # Configuração da LIB bitDogLabRef
        add_library(bitDogLabRef libs/bitDogLabRef.c)

        target_link_libraries(bitDogLabRef 
                               pico_stdlib
                               hardware_pwm
                               hardware_adc
                               arduinoStyle)

        target_include_directories(bitDogLabRef PUBLIC libs)

    # Configuração da LIB matrizLED
        add_library(matrizLED libs/subLibs/matrizLED.c)

        target_link_libraries(matrizLED 
                               pico_stdlib
                               bitDogLabRef)                   

        target_include_directories(matrizLED PUBLIC libs/subLibs)

#============================================================                    

#============================================================
#   CONFIGURAÇÃO DO EXECUTÁVEL E LINKAGEM DAS BIBLIOTECAS
    add_executable(Teste Teste.c )

    pico_set_program_name(Teste "Teste")
    pico_set_program_version(Teste "0.1")

    # Modify the below lines to enable/disable output over UART/USB
    pico_enable_stdio_uart(Teste 0)
    pico_enable_stdio_usb(Teste 1)

    # Add the standard library to the build
    target_link_libraries(Teste 
                        pico_stdlib
                        hardware_pwm
                        hardware_adc
                        arduinoStyle
                        bitDogLabRef)


    # Add the standard include files to the build
    target_include_directories(Teste PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}
    )

    # Add any user requested libraries
    target_link_libraries(Teste  )

    pico_add_extra_outputs(Teste)
#============================================================

