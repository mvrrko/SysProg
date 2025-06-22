#include "../lib/PRIONP.h"

static queue_object* PRIONP_queue = NULL;

process* PRIONP_tick(process* running_process) {
    // Wenn ein Prozess läuft, läuft er weiter (nicht-preemptiv)
    if (running_process) return running_process;

    // Kein Prozess läuft, wähle den mit höchster Prio aus der Queue
    queue_object *prev = PRIONP_queue;
    queue_object *curr = PRIONP_queue->next;
    queue_object *max_prev = PRIONP_queue;
    queue_object *max_node = curr;
    int found = 0;

    // Suche Prozess mit höchster Prio
    while (curr) {
        process *p = (process*)curr->object;
        process *max_p = max_node ? (process*)max_node->object : NULL;
        if (!max_node || (p->priority > max_p->priority)) {
            max_prev = prev;
            max_node = curr;
            found = 1;
        }
        prev = curr;
        curr = curr->next;
    }

    if (found && max_node) {
        // Entferne max_node aus der Queue
        max_prev->next = max_node->next;
        process *ret = (process*)max_node->object;
        free(max_node);
        return ret;
    }
    return NULL;
}

int PRIONP_startup() {
    PRIONP_queue = new_queue();
    return PRIONP_queue ? 0 : 1;
}

process* PRIONP_new_arrival(process* arriving_process, process* running_process) {
    // Füge neuen Prozess in die Queue ein
    queue_add(arriving_process, PRIONP_queue);
    // Nicht-preemptiv: laufender Prozess bleibt, neuer Prozess wartet
    return running_process;
}

void PRIONP_finish() {
    free_queue(PRIONP_queue);
    PRIONP_queue = NULL;
}
