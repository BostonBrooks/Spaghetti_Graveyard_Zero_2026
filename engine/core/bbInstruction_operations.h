#ifndef BB_INSTRUCTION_OPERATIONS_H
#define BB_INSTRUCTION_OPERATIONS_H


/** The core is the most convoluted part of the game engine
 * I am breaking down common tasks performed on the core in the hope that they will provide an interface
 * for different implementations of the core.
 *
 *
 * The core pops an instruction off the active_stack, makes a copy, frees the original instruction,
 * and passes a copy to the instruction function.
 *
 *
 * The instruction first sets up the instruction that will undo it and pushes that to the undo_stack,
 * then the instruction creates side_effects.
 *
 * The nature of the undo instruction depends on how the instruction was created. If the instruction was created
 * internally, the instruction allocates space on the undo_stack, sets the data of the undo instruction, then
 * pushes the undo instruction to the undo_stack.
 *
 * If the instruction was created by an input to the core, the instruction allocates some space on the undo_stack
 * and the redo_stack. The instruction then copies the original instruction onto the redo_stack.
 * next the instruction fills out the undo_instruction, adds a reference to the redo_instruction.
 *
 * If the instruction was created by a timed event (I call these actions) The instruction creates it's undo
 * instruction, adds a reference to the now unqueued timed event, and pushes the undo_instruction to the undo_stack.
 *
 * after that, the instruction is prepared to create its own side effects.
 */

/** operations:
    allocate space on undo_stack;
    push undo_instruction to undo_stack

    copy instruction to
*/



#endif // BB_INSTRUCTION_OPERATIONS_H