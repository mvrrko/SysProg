#include "../lib/MLF.h"

#define MLF_LEVELS 4

static queue_object *MLF_queues[MLF_LEVELS] = {NULL, NULL, NULL, NULL};
static int quantums[MLF_LEVELS] = {1, 2, 6, -1}; // (i+1)! für i=0..2, -1 für Level 3 (kein Quantum)
static int running_level = 0;
static int quantum_counter = 0;

static void promote_to_next_level(process *p) {
    if (running_level < MLF_LEVELS - 1) {
        queue_add(p, MLF_queues[running_level + 1]);
    } else {
        // Im letzten Level: wie LCFS, also vorne einfügen
        queue_add(p, MLF_queues[MLF_LEVELS - 1]);
    }
}

process *MLF_tick(process *running_process)
{
    // Falls kein Prozess läuft, suche das nächste Level mit Prozessen
    if (!running_process) {
        for (int i = 0; i < MLF_LEVELS; i++) {
            if (MLF_queues[i]->next) {
                running_level = i;
                quantum_counter = 0;
                if (i < MLF_LEVELS - 1) {
                    running_process = queue_poll(MLF_queues[i]);
                } else {
                    running_process = queue_poll_tail(MLF_queues[i]);
                }
                break;
            }
        }
    }

    if (running_process) {
        running_process->time_left--;
        quantum_counter++;

        // Level 3: kein Quantum, Prozess läuft bis fertig
        if (running_level == MLF_LEVELS - 1) {
            if (running_process->time_left == 0) {
                running_process = NULL;
            }
        } else {
            // Quantum abgelaufen oder Prozess fertig?
            if (running_process->time_left == 0) {
                running_process = NULL;
                quantum_counter = 0;
            } else if (quantum_counter == quantums[running_level]) {
                promote_to_next_level(running_process);
                running_process = NULL;
                quantum_counter = 0;
            }
        }
    }

    return running_process;
}

int MLF_startup()
{
    for (int i = 0; i < MLF_LEVELS; i++) {
        MLF_queues[i] = new_queue();
        if (!MLF_queues[i]) return 1;
    }
    running_level = 0;
    quantum_counter = 0;
    return 0;
}

process *MLF_new_arrival(process *arriving_process, process *running_process)
{
    if (arriving_process) {
        queue_add(arriving_process, MLF_queues[0]);
    }
    return running_process;
}

void MLF_finish()
{
    for (int i = 0; i < MLF_LEVELS; i++) {
        free_queue(MLF_queues[i]);
        MLF_queues[i] = NULL;
    }
    running_level = 0;
    quantum_counter = 0;
}
