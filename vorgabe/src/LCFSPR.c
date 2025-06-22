#include "../lib/LCFSPR.h"

static queue_object* LCFSPR_queue = NULL;

process* LCFSPR_tick(process* running_process) {
    if (running_process) {
        running_process->time_left--;
        if (running_process->time_left == 0) {
            // Prozess ist fertig, hole den letzten aus der Queue
            return queue_poll_tail(LCFSPR_queue);
        }
        // Prozess läuft weiter
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
        // NICHT sofort starten! Das macht tick im nächsten Schritt.
    }
    // Immer den aktuellen Prozess weitergeben
    return running_process;
}

void LCFSPR_finish() {
    if (!LCFSPR_queue) return;
    queue_object *curr = LCFSPR_queue->next;
    while (curr) {
        queue_object *next = curr->next;
        free(curr);
        curr = next;
    }
    free(LCFSPR_queue);
    LCFSPR_queue = NULL;
}
