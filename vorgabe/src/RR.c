#include "../lib/RR.h"

static queue_object *RR_queue = NULL;
static int quantum = 1;
static int quantum_counter = 0;

process *RR_tick(process *running_process)
{
    // Wenn kein Prozess läuft, hole den nächsten aus der Queue und setze Quantum-Zähler zurück
    if (!running_process) {
        running_process = queue_poll(RR_queue);
        quantum_counter = 0;
    }

    if (running_process) {
        running_process->time_left--;
        quantum_counter++;

        // Prozess fertig?
        if (running_process->time_left == 0) {
            free(running_process);
            running_process = NULL;
            quantum_counter = 0;
        }
        // Quantum abgelaufen?
        else if (quantum_counter == quantum) {
            queue_add(running_process, RR_queue);
            running_process = NULL;
            quantum_counter = 0;
        }
    }

    return running_process;
}

int RR_startup(int q)
{
    RR_queue = new_queue();
    quantum = q > 0 ? q : 1;
    quantum_counter = 0;
    return RR_queue ? 0 : 1;
}

process *RR_new_arrival(process *arriving_process, process *running_process)
{
    if (arriving_process) {
        queue_add(arriving_process, RR_queue);
    }
    return running_process;
}

void RR_finish()
{
    free_queue(RR_queue);
    RR_queue = NULL;
    quantum = 1;
    quantum_counter = 0;
}
