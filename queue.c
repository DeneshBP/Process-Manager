/* NOTE: I was unable to finish implementing the queue with the scheduler.
 * Because of this I made an array in the scheduler program to handle
 * all the pids. I did this so the whole program could still run.
 * */

#include <stdio.h>

pid_t *queue;
int front = -1, rear = -1;
int queue_size;

void createQueue(int size){
	queue = malloc(sizeof(pid_t) * size);
	queue_size = size;
}

int queueIsEmpty(){
	if (front == -1 && rear == -1) {
		return 1;
	}
	return 0;
}

int queueIsFull(){
    if (rear == queue_size - 1) {
	    return 1;
    }
    return 0;
}

void addToQueue(pid_t pid){
	// Check if queue is full
	if (isQueueFull()) {
		return;
	} else if (isQueueEmpty()) {
		front = rear = 0;
	} else {
		rear++;
	}

	queue[rear] = value;
}

pid_t removeFromQueue(){
	pid_t removed = queue[front];
	
	// Check if queue is empty
	if (isQueueEmpty()) {
		return;
	} else if (front == rear) {
		front = rear = -1;
	} else {
		front++;
	}

	return removed;
}

void moveToBack(pid_t pid) {
	int i, j;
	// Check if queue is empty
	if (isQueueEmpty()) {
		return;
	}

	// Find the index of the item with the specified pid
	for (i = front; i <= rear; i++) {
		if (queue[i] == pid) {
			break;
		}
	}

	// If the item wasn't found, return
	if (i > rear) {
		return;
	}

	// Move the item to the back of the queue
	for (j = i; j < rear; j++) {
		queue[j] = queue[j+1];
	}
	queue[rear] = pid;
}
