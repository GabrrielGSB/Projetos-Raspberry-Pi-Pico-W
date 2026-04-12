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
    # Configuração da displayLIB ssd1306
        add_library(ssd1306_i2c libs/subLibs/displayLibs/ssd1306_i2c.c)

        target_link_libraries(ssd1306_i2c 
                               pico_stdlib 
                               hardware_i2c)

        target_include_directories(ssd1306_i2c PUBLIC libs/subLibs/displayLibs)
    # Configuração da LIB arduinoStyle
        add_library(arduinoStyle libs/arduinoStyle.c)

        target_link_libraries(arduinoStyle 
                               pico_stdlib
                               hardware_clocks
                               hardware_adc
                               hardware_dma
                               hardware_pwm)

        target_include_directories(arduinoStyle PUBLIC libs)
        
    # Configuração da LIB soms
        add_library(soms libs/subLibs/soms.c)

        target_link_libraries(soms
                               arduinoStyle)    

        target_include_directories(soms PUBLIC libs/subLibs)
    # Configuração da LIB interrupcoes
        add_library(interrupcoes libs/interrupcoes.c)

        target_link_libraries(interrupcoes 
                               arduinoStyle
                               hardware_i2c
                               pico_cyw43_arch_lwip_threadsafe_background)

        target_include_directories(interrupcoes PUBLIC libs)

    # Configuração da LIB comunicacao
        add_library(comunicacao libs/comunicacao.c)

        target_link_libraries(comunicacao
                               pico_stdlib 
                               interrupcoes
                               hardware_i2c)

        target_include_directories(comunicacao PUBLIC libs)
    
    # Configuração da LIB displaySSD1306
        add_library(displaySSD1306 libs/subLibs/displaySSD1306.c)

        target_link_libraries(displaySSD1306
                               comunicacao 
                               ssd1306_i2c)

        target_include_directories(displaySSD1306 PUBLIC libs/subLibs)
    # Configuração da LIB microfone
        add_library(microfone libs/subLibs/microfone.c)

        target_link_libraries(microfone 
                               arduinoStyle)

        target_include_directories(microfone PUBLIC libs/subLibs)

    # Configuração da LIB joystick
        add_library(joystick libs/subLibs/joystick.c)

        target_link_libraries(joystick 
                               hardware_adc
                               arduinoStyle)

        target_include_directories(joystick PUBLIC libs/subLibs) 

    # Configuração da LIB botoes
        add_library(botoes libs/subLibs/botoes.c)

        target_link_libraries(botoes 
                               interrupcoes
                               arduinoStyle)

        target_include_directories(botoes PUBLIC libs/subLibs)
    # Configuração da LIB matrizLED
        add_library(matrizLED libs/subLibs/matrizLED.c)

        target_link_libraries(matrizLED 
                               pico_stdlib
                               hardware_pio)

        target_include_directories(matrizLED PUBLIC libs/subLibs)

        pico_generate_pio_header(matrizLED ${CMAKE_CURRENT_LIST_DIR}/ws2818b.pio)
    
    # Configuração da LIB HTTP
        add_library(HTTP libs/subLibs/HTTP.c)

        target_link_libraries(HTTP 
                               arduinoStyle
                               matrizLED
                               comunicacao 
                               pico_stdlib 
                               pico_cyw43_arch_lwip_threadsafe_background)

        target_include_directories(HTTP PUBLIC libs/subLibs)

    # Configuração da LIB telasDisplaySSD1306
        add_library(telasDisplaySSD1306 telasDisplay/telasDisplaySSD1306.c)

        target_link_libraries(telasDisplaySSD1306
                               arduinoStyle
                               interrupcoes
                               displaySSD1306)    

        target_include_directories(telasDisplaySSD1306 PUBLIC telasDisplay)


    # Configuração da LIB bitDogLabRef
        add_library(bitDogLabRef libs/bitDogLabRef.c)

        target_link_libraries(bitDogLabRef 
                               hardware_adc
                               arduinoStyle
                               displaySSD1306
                               comunicacao
                               matrizLED
                               joystick
                               microfone
                               soms
                               botoes)

        target_include_directories(bitDogLabRef PUBLIC libs)
   
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
                           interrupcoes
                           comunicacao
                           bitDogLabRef
                           HTTP
                           telasDisplaySSD1306
                           pico_multicore
                           pico_cyw43_arch_lwip_threadsafe_background)


    # Add the standard include files to the build
    target_include_directories(Teste PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}
    )

    # Add any user requested libraries
    target_link_libraries(Teste  )

    pico_add_extra_outputs(Teste)
#============================================================

