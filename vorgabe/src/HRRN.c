#include "../lib/HRRN.h"

static queue_object *HRRN_queue = NULL;
static unsigned int current_time = 0;

process *HRRN_tick(process *running_process)
{
    if (running_process) {
        current_time++;
        return running_process;
    }

    // Kein Prozess läuft, wähle den mit höchstem Response Ratio
    queue_object *prev = HRRN_queue;
    queue_object *curr = HRRN_queue->next;
    queue_object *best_prev = HRRN_queue;
    queue_object *best_node = NULL;
    double best_ratio = -1.0;

    while (curr) {
        process *p = (process*)curr->object;
        unsigned int wait = current_time - p->start_time;
        unsigned int duration = p->time_left;
        double ratio = ((double)(wait + duration)) / duration;
        if (ratio > best_ratio) {
            best_ratio = ratio;
            best_prev = prev;
            best_node = curr;
        }
        prev = curr;
        curr = curr->next;
    }

    if (best_node) {
        best_prev->next = best_node->next;
        process *ret = (process*)best_node->object;
        free(best_node);
        current_time++;
        return ret;
    }
    // current_time nicht erhöhen, wenn kein Prozess läuft!
    return NULL;
}

int HRRN_startup()
{
    HRRN_queue = new_queue();
    current_time = 0;
    return HRRN_queue ? 0 : 1;
}

process *HRRN_new_arrival(process *arriving_process, process *running_process)
{
    if (arriving_process)
        queue_add(arriving_process, HRRN_queue);
    // Nicht-preemptiv: laufender Prozess bleibt, neuer Prozess wartet
    return running_process;
}

void HRRN_finish()
{
    free_queue(HRRN_queue);
    HRRN_queue = NULL;
    current_time = 0;
}
