#include "../lib/PRIONP.h"

static queue_object* PRIONP_queue = NULL;

process* PRIONP_tick(process* running_process) {
    // Wenn ein Prozess läuft, läuft er weiter (nicht-preemptiv)
    if (running_process) return running_process;

    // Kein Prozess läuft, wähle den mit höchster Prio aus der Queue, der schon angekommen ist
    queue_object *prev = PRIONP_queue;
    queue_object *curr = PRIONP_queue->next;
    queue_object *max_prev = PRIONP_queue;
    queue_object *max_node = NULL;
    int max_priority = -1;

    while (curr) {
        process *p = (process*)curr->object;
        if (p->start_time <= 0 && (int)p->priority > max_priority) {
            max_priority = p->priority;
            max_prev = prev;
            max_node = curr;
        }
        prev = curr;
        curr = curr->next;
    }

    if (max_node) {
        max_prev->next = max_node->next;
        process *ret = (process*)max_node->object;
        free(max_node); // Nur das Queue-Element freigeben!
        return ret;
    }
    return NULL;
}

int PRIONP_startup() {
    PRIONP_queue = new_queue();
    return PRIONP_queue ? 0 : 1;
}

process* PRIONP_new_arrival(process* arriving_process, process* running_process) {
    if (arriving_process)
        queue_add(arriving_process, PRIONP_queue);
    return running_process;
}

void PRIONP_finish() {
    if (!PRIONP_queue) return;
    queue_object *curr = PRIONP_queue->next;
    while (curr) {
        queue_object *next = curr->next;
        // free(curr->object);   // <--- ENTFERNEN!
        free(curr);
        curr = next;
    }
    free(PRIONP_queue);
    PRIONP_queue = NULL;
}