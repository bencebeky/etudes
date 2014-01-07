/*******************************************************************
Copyright 2013, 2014 Bence Béky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************/

/*******************************************************************
A friend of mine got this question at a job interview with Microsoft:
How do you tell if a linked list has a loop in it? They told me about
two possible algorithms. In this program, I implemented those,
together with list creation, printing, and freeing, all from scratch.
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// This is the struct for a single link in a chain.
// Every node has a label, which is not used 
// in the loop testing algorithms.
struct link {
  struct link *next;
  int label;
};

// Create a list, possibly of length zero,
// which dead ends with a NULL pointer.
struct link *create_list_dead() {
  int i;
  struct link *head, *node;
  srand(time(NULL));
  // Maybe the list has zero nodes.
  if (random() > 0.6*RAND_MAX)
    return NULL;
  i=0;
  // Create the first node, the head.
  head = malloc(sizeof (void*));
  head->label = i;
  node = head;
  // Create more nodes with Poisson distribution,
  // label them incrementally.
  while(random() < 0.6*RAND_MAX) {
    i++;
    node->next = malloc(sizeof (void*));
    node = node->next;
    node->label = i;
  }
  // The last node has a null pointer.
  node->next = NULL;
  return head;
}

// Create a list of length at least one,
// where one node points back to a previous one,
// possibly the head or itself.
struct link *create_list_loop() {
  int i;
  struct link *head, *node, *joint;
  // Create the head node.
  i=0;
  head = malloc(sizeof (void*));
  head->label = i;
  node = head;
  srand(time(NULL));
  // Possibly create some nodes before we get to the one
  // to which we will loop back.
  while(random() < 0.6*RAND_MAX) {
    i++;
    node->next = malloc(sizeof (void*));
    node = node->next;
    node->label = i;
  }
  // Call the one we will loop back the joint.
  joint = node;
  // Possibly create more nodes after.
  while(random() < 0.6*RAND_MAX) {
    i++;
    node->next = malloc(sizeof (void*));
    node = node->next;
    node->label = i;
  }
  // Link the last one back to the joint.
  node->next = joint;
  return head;
}

// Print all nodes in the list, detecting a loop 
// using the incremental labels (this is cheating).
void print_list(struct link *head) {
  int i;
  struct link *node;
  // If there are no nodes, print nothing.
  if (head==NULL) return;
  // Start from the head, keep track of labels.
  node = head;
  i = node->label;
  // Print each node until we reach the end or a lower label.
  // Note that if node->next is NULL, then C does not
  // evaluate the next expression (after &&), which would be invalid.
  while((node->next != NULL) && (i < node->next->label)) {
    printf ("print: node %d\n", i);
    node = node->next;
    i = node->label;
  }
  // If we stopped because we reached a null pointer, this is a dead end list.
  if (node->next == NULL)
    printf ("print: last node %d\n", i);
  // Otherwise the list has a loop.
  else
    printf ("print: last node %d linking back to node %d\n", i, node->next->label);
}

// This is one algorithm to determine if the list loops back to itself
// without looking at the labels. I define two walkers: one makes one
// step in each iteration, two makes two steps. If they meet again, that
// must be because they got in a loop. If two reaches the end, the list
// has an end. Note that two is always ahead of one, therefore one cannot
// reach the end of the list (if there is such) before two.
int isloop_twowalker(struct link *head) {
  struct link *one, *two;
  // If the list is of length zero, it does not have a loop.
  if(head==NULL) return 0;
  // Otherwise, start walker from the head.
  one = head;
  two = head;
  while(1) {
    // Two makes two steps, checking if we reached the end
    // or ran into one after each step.
    two = two->next;
    if (two==NULL) return 0;
    printf ("walkers %d %d\n", one->label, two->label);
    if (one==two) return 1;
    two = two->next;
    if (two==NULL) return 0;
    printf ("walkers %d %d\n", one->label, two->label);
    if (one==two) return 1;
    // One makes one step. Note that if two just got back to joint,
    // and one has not yet reached it, then it is possible
    // that this is the first time they meet.
    one = one->next;
    if (one==NULL) return 0;
    printf ("walkers %d %d\n", one->label, two->label);
    if (one==two) return 1;
  }
}

// This is another algorithm to determin if the list loops back to itself
// without looking at the labels. As we go along, we reverse each link.
// If we hit the end, the list has an end. However, if it loops back,
// we get rerouted at the joint towards the head. Therefore if we reach
// the head, that means there was a loop. This is a pretty fast algorithm,
// the downside is that it is invasive.
int isloop_reverse(struct link *head) {
  struct link *node, *prev, *next;
  // If the list is of length zero, it does not have a loop.
  if (head==NULL) return 0;
  // Otherwise, start from the head.
  prev = head;
  node = head->next;
  while(1) {
    // If we are at the end, the list has an end,
    // if we got back to the head, it has a loop.
    if (node==NULL) return 0;
    if (node==head) return 1;
    next = node->next;
    // node used to point to next, make it point to prev, and step forward.
    node->next = prev;
    printf ("node %d now points to node %d\n", node->label, prev->label);
    prev = node;
    node = next;
  }
}

// This algorithm frees up every node in the list. Note that it does not work
// if there was a loop (it segfaults), also it does not work if the list
// had a dead end and isloop_reverse messed up with all the links.
void free_list(struct link *head) {
  struct link *node, *next;
  node = head;
  while(node!=NULL) {
    next = node->next;
    printf ("free %d\n", node->label);
    free(node);
    node = next;
  }
}

// This is the main function.
int main() {
  struct link *head;
  // First we create a list that deadends, print it, and test it
  // with both algorithms.
  head = create_list_dead();
  print_list(head);
  if (isloop_twowalker(head))
    printf("twowalker: loop\n");
  else
    printf("twowalker: not loop\n");
  if (isloop_reverse(head))
    printf("reverse: loop\n");
  else
    printf("reverse: not loop\n");
  // Free does not work after isloop_reverse, which is pretty invasive.
  //free_list(head);
  // Then we create a list that loops back onto itself, print it,
  // and test it with both algorithms.
  head = create_list_loop();
  print_list(head);
  if (isloop_twowalker(head))
    printf("twowalker: loop\n");
  else
    printf("twowalker: not loop\n");
  if (isloop_reverse(head))
    printf("reverse: loop\n");
  else
    printf("reverse: not loop\n");
  // Cannot easily free looped list, we'll just let
  // the garbage collectors deal with it.
  return 0;
}
