#include "engine/ECS/bbAI_System.h"

#include "engine/logic/bbBloatedPool.h"

bbFlag bbEntityAIs_init(bbEntityAIs* entityAIs)
{
    bbVPool_newBloated(&entityAIs->system.pool, sizeof(bbEntityAI),100,100,"ENTITY AI");
    bbList_init(&entityAIs->list,entityAIs->system.pool,NULL,offsetof(bbEntityAI,list_element),NULL);


}
