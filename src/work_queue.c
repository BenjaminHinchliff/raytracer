#include "work_queue.h"
#include "trace.h"

#include <pthread.h>
#include <stdlib.h>

bool raytrace_work(WorkQueue *work_queue, uint32_t *state) {
  size_t next_order = work_queue->next_order++;
  if (next_order >= work_queue->count) {
    return false;
  }

  WorkOrder *order = &work_queue->work_orders[next_order];

  trace_rows(order->world, state, order->start_row_index, order->end_row_index,
             order->samples, order->max_depth, order->image);

  return true;
}

bool work_queue_add(WorkQueue *queue, WorkOrder order) {
  if (queue->count == queue->capacity) {
    return false;
  }

  queue->work_orders[queue->count++] = order;
  return true;
}

WorkQueue *work_queue_new(size_t capacity) {
  WorkQueue *queue = malloc(sizeof(WorkQueue));
  queue->capacity = capacity;
  queue->count = 0;
  queue->work_orders = malloc(sizeof(WorkOrder) * capacity);
  queue->next_order = 0;
  return queue;
}

typedef struct RunnerContext {
  WorkQueue *queue;
  uint32_t state[4]; // rand not thread safe so needs to be passed in
} RunnerContext;

void *work_runner(void *ptr) {
  RunnerContext *ctx = (RunnerContext *)ptr;

  // initialize rng
  while (raytrace_work(ctx->queue, ctx->state))
    /* pass */;

  return NULL;
}

bool work_queue_run(WorkQueue *queue) {
  const int num_threads = 8;
  pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
  RunnerContext *contexts = malloc(sizeof(RunnerContext) * num_threads);
  for (int i = 0; i < num_threads; i++) {

    contexts[i] = (RunnerContext){
        .queue = queue,
        .state = {rand(), rand(), rand(), rand()},
    };
    pthread_create(&threads[i], NULL, work_runner, (void *)&contexts[i]);
  }

  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }
  free(threads);
  free(contexts);
  return true;
}

void work_queue_free(WorkQueue *queue) {
  free(queue->work_orders);
  free(queue);
}
