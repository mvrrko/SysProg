#include "../lib/LCFSPR.h"

static queue_object* LCFSPR_queue = NULL;
static process* preempted_process = NULL;

process* LCFSPR_tick(process* running_process) {
    // Wenn ein Prozess läuft, führe einen Zeitschritt aus
    if (running_process) {
        running_process->time_left--;
        // Wenn der Prozess fertig ist, hole den nächsten vom Stack
        if (running_process->time_left == 0) {
            free(running_process);
            running_process = queue_poll_tail(LCFSPR_queue);
        }
        return running_process;
    }
    // Kein Prozess läuft, hole den letzten aus der Queue
    return queue_poll_tail(LCFSPR_queue);
}

int LCFSPR_startup() {
    LCFSPR_queue = new_queue();
    return LCFSPR_queue ? 0 : 1;
}

process* LCFSPR_new_arrival(process* arriving_process, process* running_process) {
    if (arriving_process) {
        // Wenn ein Prozess läuft, wird er unterbrochen und kommt zurück auf den Stack
        if (running_process) {
            queue_add(running_process, LCFSPR_queue);
        }
        // Der neue Prozess wird ganz vorne auf den Stack gelegt
        queue_add(arriving_process, LCFSPR_queue);
        // Der neue Prozess soll sofort laufen
        return queue_poll_tail(LCFSPR_queue);
    }
    // Kein neuer Prozess, der aktuelle läuft weiter
    return running_process;
}

void LCFSPR_finish() {
    free_queue(LCFSPR_queue);
    LCFSPR_queue = NULL;
}
