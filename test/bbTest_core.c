///This test shows that actions can be performed in order up until the current time.
///Then, the state can be reversed, and new actions added to the list.
///After that the new actions can be performed along with the existing actions
#include<unistd.h>
#include "engine/core/bbCore.h"

#include "engine/core/bbCoreInputs.h"
#include "engine/core/bbLocalMessage.h"
#include "engine/logic/bbString.h"
#include "engine/core/bbLocalMessageInputs.h"
#include "engine/data/bbHome.h"

thread_local char* thread;
U64 test_time = 0;
bbHome home;
bbHandle no_handle = {0};

//TODO rewind core until just after time was set to action.act_tick
//Look for bbInstruction_unsetTime
//with instruction->data.integer.integer == i;

int main (void)
{

    //bbCore core;
    bbCore_init(&home.core.core);

    bbStr_putStr(home.core.quote, "Abera Kedabera",KEY_LENGTH);
    home.core.core.simulation_time = 117;
    home.core.test_int = 69;

    U32 collision = 0;
    bbPrintf("Play:\n");


    char key[KEY_LENGTH];

    for (I32 i = 0; i < 9;i++)
    {
        bbCoreInput_setTime(&home.core.core, i, bbInstructionSource_input, no_handle);
        bbCore_react(&home.core.core);


        sprintf(key, " (%d)",i);

        bbAction_setQuote(&home.core.core,
                            0,
                            collision++,
                            0,
                            i,
                            key);

        bbCoreInput_checkActions(&home.core.core,i,bbInstructionSource_input, no_handle);
        bbCore_react(&home.core.core);



    }


    bbPrintf("Add more actions:\n");
    sprintf(key, "(%d.1)",5);
    printf( "new action: (%d.1)\n",5);

    bbAction_setQuote(&home.core.core,
                        0,
                        collision++,
                        0,
                        5,
                        key);

    sprintf(key, "(%d.2)",5);
    printf( "new action: (%d.2)\n",5);

    bbAction_setQuote(&home.core.core,
                        0,
                        collision++,
                        0,
                        5,
                        key);


    bbCoreInput_checkActions(&home.core.core,10,bbInstructionSource_input, no_handle);
    bbCore_react(&home.core.core);

    sprintf(key, "(%d.1)",3);
    printf( "new action: (%d.1)\n",3);

    bbAction_setQuote(&home.core.core,
                        0,
                        collision++,
                        0,
                        3,
                        key);

    sprintf(key, "(%d.2)",3);
    printf( "new action: (%d.1)\n",3);

    bbAction_setQuote(&home.core.core,
                        0,
                        collision++,
                        0,
                        3,
                        key);


    bbCoreInput_checkActions(&home.core.core,10,bbInstructionSource_input, no_handle);
    bbCore_react(&home.core.core);



}
