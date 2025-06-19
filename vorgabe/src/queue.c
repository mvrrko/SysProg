#include "../lib/queue.h"
#include <stdlib.h>

int queue_add(void *new_object, queue_object *queue)
{
    if (!queue || !new_object) return 1;
    queue_object *new_node = (queue_object *)malloc(sizeof(queue_object));
    if (!new_node) return 1;
    new_node->object = new_object;
    new_node->next = queue->next;
    queue->next = new_node;
    return 0;
}

void *queue_poll(queue_object *queue)
{
    if (!queue || !queue->next) return NULL;
    queue_object *prev = queue;
    queue_object *curr = queue->next;
    while (curr->next) {
        prev = curr;
        curr = curr->next;
    }
    void *obj = curr->object;
    prev->next = NULL;
    free(curr);
    return obj;
}

queue_object *new_queue()
{
    queue_object *head = (queue_object *)malloc(sizeof(queue_object));
    if (!head) return NULL;
    head->object = NULL;
    head->next = NULL;
    return head;
}

void free_queue(queue_object *queue)
{
    if (!queue) return;
    queue_object *curr = queue->next;
    while (curr) {
        queue_object *next = curr->next;
        free(curr);
        curr = next;
    }
    free(queue);
}

void *queue_peek(queue_object *queue)
{
    if (!queue || !queue->next) return NULL;
    queue_object *curr = queue->next;
    while (curr->next) {
        curr = curr->next;
    }
    return curr->object;
}

void *queue_poll_tail(queue_object *queue)
{
    if (!queue || !queue->next) return NULL;
    queue_object *first = queue->next;
    queue->next = first->next;
    void *obj = first->object;
    free(first);
    return obj;
}