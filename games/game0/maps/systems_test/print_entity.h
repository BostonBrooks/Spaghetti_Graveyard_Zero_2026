#ifndef PRINT_ENTITY_NAME
#define PRINT_ENTITY_NAME

#define bbEntity_print(NAME){\
    bbECS_entity* entity;\
    bbHandle_getComponent(&home.ECS.ECS->system,(bbComponent**)&entity,NAME);\
    if (entity == NULL) {\
    bbDebug("Entity not found, (%d,%d,%d)\n", NAME.system.index,NAME.system.system,NAME.system.generation);\
    } else {\
        bbDebug("Entity name: %s, (%d,%d,%d) \n", entity->key,NAME.system.index,NAME.system.system,NAME.system.generation);\
    }\
    }


#endif //PRINT_ENTITY_NAME