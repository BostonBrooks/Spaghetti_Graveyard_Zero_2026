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

    for (I32 i = 0; i < 15;i++)
    {
        bbCoreInput_setTime(&home.core.core, i, bbInstructionSource_input, no_handle);
        bbCore_react(&home.core.core);

        I32 time_plus_rand = i + rand() % 3 + 1;


        sprintf(key, "created: %d, acted: %d",i, time_plus_rand);

        bbAction_setQuote(&home.core.core,
                            0,
                            collision++,
                            0,
                            time_plus_rand,
                            key);

        bbCoreInput_checkActions(&home.core.core,i,bbInstructionSource_input, no_handle);
        bbCore_react(&home.core.core);



    }


    //bbCoreInput_setTestInt(&home.core.core, 7, bbInstructionSource_input, no_handle);
    //bbCore_react(&home.core.core);



    bbPrintf("Rewind:\n");
    bbCore_rewindUntilTime(&home.core.core, 3);



    bbPrintf("Add more actions:\n");
    sprintf(key, "new action: - acted: %d",7);

    bbAction_setQuote(&home.core.core,
                        0,
                        collision++,
                        0,
                        7,
                        key);

    sprintf(key, "new action: i = %d",8);

    bbAction_setQuote(&home.core.core,
                        0,
                        collision++,
                        0,
                        8,
                        key);

    sprintf(key, "new action: i = %d",8);

    bbAction_setQuote(&home.core.core,
                        0,
                        collision++,
                        0,
                        8,
                        key);

    bbPrintf("Fast Forward:\n");
    bbCore_react(&home.core.core);


}
