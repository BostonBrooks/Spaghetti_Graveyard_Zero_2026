cmake_minimum_required(VERSION 4.1)
project(Spaghetti_Graveyard_Zero_2026 C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../games/game0)

if (CSFML3)
    add_definitions(-DCSFML3=1)
endif (CSFML3)


add_executable(Server engine/server/bbServer.c
        engine/network/bbNetworkPacket.c
        engine/threadsafe/bbThreadedPool.c
        engine/threadsafe/bbThreadedQueue.c
        engine/logic/bbBloatedPool.c
        engine/logic/bbVPool.h)
target_compile_definitions(Server PRIVATE
        DEFINE_SERVER)
target_include_directories(Server  PRIVATE "./")
target_link_libraries(Server PRIVATE
        csfml-graphics
        csfml-window
        csfml-system
        csfml-network
)



add_executable(SkellyChase test/bbTest_SkellyChase.c
        engine/ECS/ECS.c
        engine/ECS/spawn_entity.c
        engine/ECS/bbServerEntities.c
        engine/ECS/bbGraphicsSystem.c
        engine/graphics/bbFonts.c
        engine/graphics/bbSprites.c
        engine/graphics/bbTextures.c
        engine/graphics/bbColours.c
        engine/graphics/bbCompositions.c
        engine/graphics/bbGraphicsApp.c
        engine/graphics/bbAnimations.c
        engine/core/bbInstruction.c
        engine/core/bbCoreInbox.c
        engine/core/bbCoreInputs.c
        engine/core/bbCoreDiscard.c
        engine/core/bbCoreInboxInput.c
        engine/core/bbCore.c
        engine/core/bbAction.c
        engine/geometry/bbCoordinates.c
        engine/geometry/bbViewportCoords.c
        engine/data/bbIncludes.c
        engine/viewport/bbViewportApp.c
        engine/viewport/bbDrawables.c
        engine/viewport/bbMapIcons.c
        engine/viewport/bbViewport.c
        engine/viewport/bbUnits.c
        engine/interthread/bbClock.c
        engine/network/bbNetworkTime.c
        engine/network/bbNetworkPacket.c
        engine/network/bbNetworkApp.c
        engine/network/bbNetwork.c
        engine/logic/bbList_getNth.c
        engine/logic/bbDictionary.c
        engine/logic/bbTree.c
        engine/logic/bbList_map.c
        engine/logic/bbBloatedPool.c
        engine/logic/bbList_sort.c
        engine/logic/bbList.c
        engine/logic/bbList_iterate.c
        engine/logic/bbNestedList.c
        engine/userinterface/bbInput.c
        engine/userinterface/bbMouseFunctions.c
        engine/userinterface/bbUI_Inbox.c
        engine/userinterface/bbWidgetFunctions.c
        engine/userinterface/bbMouse.c
        engine/userinterface/bbWidgets.c
        engine/userinterface/bbUIApp.c
        engine/threadsafe/bbThreadedQueue.c
        engine/threadsafe/bbThreadedPool.c
        engine/threadsafe/bbThreadedQueue_search.c
        games/game0/maps/skellychase/mouse/mouse.c
        games/game0/maps/skellychase/drawfunctions/drawfunctions.c
        games/game0/maps/skellychase/widgets/widgets.c
        games/game0/maps/skellychase/widgets/spawn_widgets.c
        games/game0/maps/skellychase/core/core.c
        games/game0/maps/skellychase/core/instructions.c
        games/game0/maps/skellychase/core/core_inputs.c
        engine/groundsurface/bbHillShading.c
        engine/groundsurface/bbGroundSurface.c
        engine/geometry/bbGroundCoords.c
)

target_compile_definitions(SkellyChase PRIVATE
        DEFINE_SKELLYCHASE)

target_include_directories(SkellyChase  PRIVATE "./"
                            PRIVATE "./games/game0/maps/skellychase/")

target_link_libraries(SkellyChase PRIVATE
        csfml-graphics
        csfml-window
        csfml-system
        csfml-network
        m
)

add_executable(Test_Pool test/bbTest_Pool.c
        engine/logic/bbBloatedPool.c)
target_compile_definitions(Test_Pool PRIVATE
        DEFINE_TEST_POOL)
target_include_directories(Test_Pool PRIVATE "./")

add_executable(Test_ECS  test/bbTest_ECS.c
        engine/ECS/ECS.c
        engine/core/bbInstruction.c
        engine/core/bbCoreInputs.c
        engine/core/bbCoreDiscard.c
        engine/core/bbCore.c
        engine/core/bbCoreInbox.c
        engine/core/bbCoreInboxInput.c
        engine/core/bbAction.c
        engine/logic/bbList.c
        engine/logic/bbList_sort.c
        engine/logic/bbBloatedPool.c
        engine/threadsafe/bbThreadedQueue.c
        engine/threadsafe/bbThreadedPool.c
        engine/network/bbNetwork.c
        engine/network/bbNetworkTime.c
        engine/network/bbNetworkPacket.c
        engine/network/bbNetworkApp.c
        engine/threadsafe/bbThreadedQueue_search.c
        engine/logic/bbList_getNth.c)
target_compile_definitions(Test_ECS PRIVATE DEFINE_TEST_ECS)
target_include_directories(Test_ECS PRIVATE "./")
target_link_libraries(Test_ECS PRIVATE
        csfml-graphics
        csfml-window
        csfml-system
        csfml-network
)