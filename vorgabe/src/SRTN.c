#include "../lib/SRTN.h"

static queue_object *SRTN_queue = NULL;

process *SRTN_tick(process *running_process)
{
    // Falls kein laufender Prozess: wähle den mit kürzester Restzeit aus der Queue
    if (!running_process) {
        queue_object *prev = SRTN_queue;
        queue_object *curr = SRTN_queue->next;
        queue_object *min_prev = SRTN_queue;
        queue_object *min_node = NULL;
        unsigned int min_time = ~0u; // maximaler Wert

        while (curr) {
            process *p = (process*)curr->object;
            if (p->time_left < min_time) {
                min_time = p->time_left;
                min_prev = prev;
                min_node = curr;
            }
            prev = curr;
            curr = curr->next;
        }

        if (min_node) {
            min_prev->next = min_node->next;
            running_process = (process*)min_node->object;
            free(min_node);
        }
    }

    // Einen Zeitschritt ausführen
    if (running_process) {
        running_process->time_left--;
        if (running_process->time_left == 0) {
            free(running_process);
            running_process = NULL;
        }
    }

    return running_process;
}

int SRTN_startup()
{
    SRTN_queue = new_queue();
    return SRTN_queue ? 0 : 1;
}

process *SRTN_new_arrival(process *arriving_process, process *running_process)
{
    if (arriving_process) {
        // Wenn ein Prozess läuft und der neue Prozess hat weniger Restzeit: preempt!
        if (running_process && arriving_process->time_left < running_process->time_left) {
            // Laufenden Prozess in die Queue zurücklegen
            queue_add(running_process, SRTN_queue);
            running_process = arriving_process;
        } else {
            // Neuer Prozess in die Queue
            queue_add(arriving_process, SRTN_queue);
        }
    }
    return running_process;
}

void SRTN_finish()
{
    free_queue(SRTN_queue);
    SRTN_queue = NULL;
}
