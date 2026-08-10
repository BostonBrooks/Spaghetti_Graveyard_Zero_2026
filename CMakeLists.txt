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


add_executable(Test_Pool test/bbTest_Pool.c
        engine/logic/bbBloatedPool.c)
target_compile_definitions(Test_Pool PRIVATE
        DEFINE_TEST_POOL)
target_include_directories(Test_Pool PRIVATE "./")


add_executable(Core_Test  games/game0/maps/core_test/core_test.c
        engine/test_string/bbTestString.c

        engine/core/bbAction.c
        engine/core/bbCore.c
        engine/core/bbCoreDiscard.c
        engine/core/bbCoreInbox.c
        engine/core/bbCoreInboxInput.c
        engine/core/bbCoreInputs.c
        engine/core/bbInstruction.c

        engine/logic/bbList.c
        engine/logic/bbNestedList.c
        engine/logic/bbList_sort.c
        engine/logic/bbList_map.c
        engine/logic/bbList_iterate.c
        engine/logic/bbList_getNth.c

        engine/logic/bbTree.c
        engine/logic/bbBloatedPool.c
        engine/logic/bbDictionary.c

        engine/threadsafe/bbThreadedQueue_search.c
        engine/threadsafe/bbThreadedQueue.c
        engine/threadsafe/bbThreadedPool.c
)

target_compile_definitions(Core_Test PRIVATE DEFINE_TEST_CORE
        PRIVATE BB_HOME_H)
target_include_directories(Core_Test PRIVATE "./"
        PRIVATE "./games/game0/maps/core_test/")
target_link_libraries(Core_Test PRIVATE
        csfml-graphics
        csfml-window
        csfml-system
        csfml-network
)


add_executable(ECS_Test  games/game0/maps/ECS_test/ECS_test.c
        engine/test_string/bbTestString.c
        games/game0/maps/ECS_test/virtual_instructions/instructions.c
        games/game0/maps/ECS_test/core/core_inbox.c

        engine/ECS/ECS.c
        engine/ECS/server_entities/bbServerEntities.c


        engine/core/bbAction.c
        engine/core/bbCore.c
        engine/core/bbCoreDiscard.c
        engine/core/bbCoreInbox.c
        engine/core/bbCoreInboxInput.c
        engine/core/bbCoreInputs.c
        engine/core/bbInstruction.c

        engine/logic/bbList.c
        engine/logic/bbNestedList.c
        engine/logic/bbList_sort.c
        engine/logic/bbList_map.c
        engine/logic/bbList_iterate.c
        engine/logic/bbList_getNth.c

        engine/logic/bbTree.c
        engine/logic/bbBloatedPool.c
        engine/logic/bbDictionary.c

        engine/threadsafe/bbThreadedQueue_search.c
        engine/threadsafe/bbThreadedQueue.c
        engine/threadsafe/bbThreadedPool.c
)

target_compile_definitions(ECS_Test  PRIVATE DEFINE_TEST_ECS
        PRIVATE BB_HOME_H)
target_include_directories(ECS_Test  PRIVATE "./"
        PRIVATE "./games/game0/maps/ECS_test/")
target_link_libraries(ECS_Test PRIVATE
        csfml-graphics
        csfml-window
        csfml-system
        csfml-network
)

add_executable(Systems_Test games/game0/maps/systems_test/System_test.c

        games/game0/maps/systems_test/core/netpause_button.c
        games/game0/maps/systems_test/core/unfreeze_button.c
        games/game0/maps/systems_test/graphics_system/graphics_system.c
        games/game0/maps/systems_test/mouse/mouse.c
        games/game0/maps/systems_test/drawfunctions/drawfunctions.c
        games/game0/maps/systems_test/widgets/widgets.c
        games/game0/maps/systems_test/widgets/spawn_widgets.c
        games/game0/maps/systems_test/core/core.c
        games/game0/maps/systems_test/core/instructions.c
        games/game0/maps/systems_test/core/core_inputs.c
        games/game0/maps/systems_test/core/core_inbox.c
        games/game0/maps/systems_test/AI_system/AI_System.c
        games/game0/maps/systems_test/entity_spawner/spawner.c
        games/game0/maps/systems_test/moveables/moveables.c
        games/game0/maps/systems_test/AI_system/ai_instructions.c

        engine/test_string/bbTestString.c

        engine/ECS/ECS.c
        engine/ECS/server_entities/bbServerEntities.c
        engine/ECS/spawn_entity.c
        engine/ECS/moveables/bbMoveables.c
        engine/ECS/graphics_system/bbGraphicsSystem.c
        engine/ECS/bbEntitySpawner.c
        engine/ECS/AI_system/bbAI_System.c
        engine/ECS/bbEntitySpawner.c

        engine/core/bbAction.c
        engine/core/bbCore.c
        engine/core/bbCoreDiscard.c
        engine/core/bbCoreInbox.c
        engine/core/bbCoreInboxInput.c
        engine/core/bbCoreInputs.c
        engine/core/bbInstruction.c

        engine/logic/bbList.c
        engine/logic/bbNestedList.c
        engine/logic/bbList_sort.c
        engine/logic/bbList_map.c
        engine/logic/bbList_iterate.c
        engine/logic/bbList_getNth.c

        engine/logic/bbTree.c
        engine/logic/bbBloatedPool.c
        engine/logic/bbDictionary.c

        engine/threadsafe/bbThreadedQueue_search.c
        engine/threadsafe/bbThreadedQueue.c
        engine/threadsafe/bbThreadedPool.c

        engine/network/bbNetworkTime.c
        engine/network/bbNetworkPacket.c
        engine/network/bbNetworkApp.c
        engine/network/bbNetwork.c

        engine/interthread/bbClock.c

        engine/userinterface/bbInput.c
        engine/userinterface/bbMouse.c
        engine/userinterface/bbMouseFunctions.c
        engine/userinterface/bbUI_Inbox.c
        engine/userinterface/bbUIApp.c
        engine/userinterface/bbWidgetFunctions.c
        engine/userinterface/bbWidgets.c

        engine/viewport/bbDrawables.c
        engine/viewport/bbMapIcons.c
        engine/viewport/bbUnits.c
        engine/viewport/bbViewport.c
        engine/viewport/bbViewportApp.c

        engine/geometry/bbCoordinates.c
        engine/geometry/bbGroundCoords.c
        engine/geometry/bbViewportCoords.c

        engine/groundsurface/bbHillShading.c
        engine/groundsurface/bbGroundSurface.c

        engine/graphics/bbTextures.c
        engine/graphics/bbSprites.c
        engine/graphics/bbGraphicsApp.c
        engine/graphics/bbFonts.c
        engine/graphics/bbCompositions.c
        engine/graphics/bbColours.c
        engine/graphics/bbAnimations.c
)

target_compile_definitions(Systems_Test  PRIVATE DEFINE_TEST_SYSTEM)
target_include_directories(Systems_Test  PRIVATE "./"
        PRIVATE "games/game0/maps/systems_test/")
target_link_libraries(Systems_Test PRIVATE
        csfml-graphics
        csfml-window
        csfml-system
        csfml-network
        m
)