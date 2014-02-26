#include <stdio.h>
#include <stdlib.h>

/* Queue class, of size capacity,
 * front is where we dequeue,
 * with count elements currently in queue,
 * stored in array[].
 * Why do we not store front and back?
 * Because they could both take size values,
 * but queue has size*size non-empty and one empty states,
 * so we would need an additional flag
 * to tell if front==back is empty or full. */
struct queue {
  int size;
  int front;
  int count;
  int *array;
};

void queueInit(struct queue * q, int n) {
  q->size = n;
  q->front = 0;
  q->count = 0;
  q->array = malloc(n*sizeof(int));
}

void enqueue(struct queue * q, int element) {
  if (q->count == q->size) {
    fprintf(stderr, "Error: queue overflow.\n");
    exit(-1);
  }
  q->array[(q->front + q->count) % q->size] = element;
  q->count++;
}

int dequeue(struct queue * q) {
  if (q->count == 0) {
    fprintf(stderr, "Error: queue underflow.\n");
    exit(-1);
  }
  int element = q->array[q->front];
  q->front = (q->front+1) % q->size;
  q->count--;
  return element;
}

int main() {
  struct queue q;
  queueInit(&q, 4);
  enqueue(&q, 1);
  printf("%d\n", dequeue(&q));
  enqueue(&q, 2);
  printf("%d\n", dequeue(&q));
  enqueue(&q, 3);
  enqueue(&q, 4);
  enqueue(&q, 5);
  printf("%d\n", dequeue(&q));
  printf("%d\n", dequeue(&q));
  printf("%d\n", dequeue(&q));
  enqueue(&q, 5);
  enqueue(&q, 6);
  enqueue(&q, 7);
  printf("%d\n", dequeue(&q));
  enqueue(&q, 8);
  printf("%d\n", dequeue(&q));
  printf("%d\n", dequeue(&q));
  enqueue(&q, 9);
  printf("%d\n", dequeue(&q));
  printf("%d\n", dequeue(&q));
  return 0;
}
