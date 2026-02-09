cmake_minimum_required(VERSION 4.1)
project(Spaghetti_Graveyard_Zero_2026 C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../games/game0)

add_executable(Spaghetti_Graveyard_Zero_2026 test/incremental_test.c
        engine/data/bbIncludes.c
        engine/logic/bbBloatedPool.c
        engine/logic/bbDictionary.c
        engine/threadsafe/bbThreadedPool.c
        engine/threadsafe/bbThreadedQueue.c
        engine/threadsafe/bbThreadedQueue_search.c
        engine/logic/bbTree.c
        engine/graphics/bbTextures.c
        engine/graphics/bbSprites.c
        engine/graphics/bbAnimations.c
        engine/graphics/bbColours.c
        engine/graphics/bbFonts.c
        engine/graphics/bbCompositions.c
        engine/graphics/bbGraphicsApp.c
        games/game0/maps/map0/drawfunctions/drawfunctions.c
        engine/userinterface/bbWidgets.c
        engine/userinterface/bbWidgetFunctions.c
        games/game0/maps/map0/widgets/widgets.c
        engine/geometry/bbCoordinates.c
        engine/userinterface/bbInput.c
        engine/userinterface/bbMouse.c
        engine/userinterface/bbMouseFunctions.c
        games/game0/maps/map0/mouse/mouse.c)

target_include_directories(Spaghetti_Graveyard_Zero_2026  PRIVATE "./")

target_link_libraries(Spaghetti_Graveyard_Zero_2026 PRIVATE
        csfml-graphics
        csfml-window
        csfml-system
        csfml-network
)

add_executable(Server engine/server/bbServer.c
        engine/data/bbIncludes.c
        engine/logic/bbBloatedPool.c
        engine/logic/bbDictionary.c
        engine/threadsafe/bbThreadedPool.c
        engine/threadsafe/bbThreadedQueue.c
        engine/threadsafe/bbThreadedQueue_search.c
        engine/network/bbNetwork.c
        engine/network/bbNetworkApp.c
        engine/network/bbNetworkClock.c
        engine/network/bbNetworkPacket.c
        engine/network/bbNetworkTime.c
        engine/logic/bbList.c
        engine/logic/bbList_getNth.c
        engine/logic/bbList_iterate.c
        engine/logic/bbList_map.c
        engine/logic/bbList_sort.c
)


target_include_directories(Server  PRIVATE "./")

target_link_libraries(Server PRIVATE
        csfml-graphics
        csfml-window
        csfml-system
        csfml-network
)


add_executable(test_sort test/bbList_testSort.c
        engine/logic/bbList.c
        engine/logic/bbBloatedPool.c
)

target_include_directories(test_sort  PRIVATE "./")

