#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include "image.h"
#include "world.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct WorkOrder {
  Image *image;
  World *world;
  int start_row_index;
  int end_row_index;
  int samples;
  int max_depth;
} WorkOrder;

typedef struct WorkQueue {
  size_t capacity;
  size_t count;
  WorkOrder *work_orders;

  atomic_size_t next_order;
} WorkQueue;

WorkQueue *work_queue_new(size_t capacity);
bool work_queue_add(WorkQueue *queue, WorkOrder order);
bool work_queue_run(WorkQueue *queue);
void work_queue_free(WorkQueue *queue);

#endif
