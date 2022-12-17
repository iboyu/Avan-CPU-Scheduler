/* This is the only file you will be editing.
 * - Copyright of Starter Code: Prof. Kevin Andrea, George Mason University.  All Rights Reserved
 * - Copyright of Student Code: You!
 * - Restrictions on Student Code: Do not post your code on any public site (eg. Github).
 * -- Feel free to post your code on a PRIVATE Github and give interviewers access to it.
 * - Date: Aug 2022
 */

/* Fill in your Name and GNumber in the following two comment fields
 * Name: RongLian Yuan
 * GNumber: G01313261
 */

// System Includes
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>
#include <sched.h>
// Local Includes
#include "avan_sched.h"
#include "vm_support.h"
#include "vm_process.h"

/* Feel free to create any helper functions you like! */

/* Malloc four times. Return NULL if malloc failed.
   First time malloc a header points to three queue.
   The rest three times are to malloc three pointers which point to the content in three queue.
   Initialize valuse in those three pointers struct.
*/
avan_header_t *avan_create()
{

  avan_header_t *myHeader = (avan_header_t *)malloc(sizeof(avan_header_t));
  if (myHeader == NULL)
  {
    printf("avan_header_t malloc failed.\n");
    return NULL;
  }

  myHeader->ready_queue = (queue_header_t *)malloc(sizeof(queue_header_t));
  if (myHeader->ready_queue == NULL)
  {
    printf("myHeader -> ready_queue malloc failed.\n");
    return NULL;
  }
  myHeader->ready_queue->count = 0;
  myHeader->ready_queue->head = NULL;

  myHeader->suspended_queue = (queue_header_t *)malloc(sizeof(queue_header_t));
  if (myHeader->suspended_queue == NULL)
  {
    printf("myHeader -> suspended_queue malloc failed.\n");
    return NULL;
  }
  myHeader->suspended_queue->count = 0;
  myHeader->suspended_queue->head = NULL;

  myHeader->terminated_queue = (queue_header_t *)malloc(sizeof(queue_header_t));
  if (myHeader->terminated_queue == NULL)
  {
    printf("myHeader -> terminated_queue malloc failed.\n");
    return NULL;
  }
  myHeader->terminated_queue->count = 0;
  myHeader->terminated_queue->head = NULL;

  return myHeader; // Replace Me with Your Code!
}

/*This is the helper function help me to insert the node into the corresponding queue.
  If the node is Null, fail to insert, return -1. Or we return 0;
*/
int insertTheProcess(queue_header_t *queue, process_node_t *process)
{

  if (process == NULL)
  {
    printf("The process is NULL. Cannot insert. Exit\n");
    return -1;
  }
  process_node_t *currentNode = queue->head;

  if (currentNode == NULL)
  {
    queue->head = process;
  }

  else if (currentNode->next == NULL)
  {
    if (currentNode->pid > process->pid)
    {
      process->next = currentNode;
      queue->head = process;
    }
    else
    {
      queue->head->next = process;
    }
  }
  else
  {

    while (currentNode->next != NULL)
    {
      if ((currentNode->pid) < (process->pid) && (currentNode->next->pid) > (process->pid))
      {
        process->next = currentNode->next;
        currentNode->next = process;
        break;
      }
      else
      {
        currentNode = currentNode->next;
      }
    }
    if (currentNode->next == NULL)
    {
      currentNode->next = process;
    }
  }

  return 0;
}
/*This is the helper function help me to find the node with unique pid.
  If the queue is empty or loop all linked list, we cannot find the node, we return NULL;
  Or we return the node we want to remove;
*/
process_node_t *removeTheProcess(queue_header_t *queue, pid_t pid)
{
  if (queue->head == NULL)
  {
    return NULL;
  }
  process_node_t *removedNode = NULL;
  process_node_t *currentNode = queue->head;

  if (currentNode->pid == pid && currentNode->next == NULL)
  {
    removedNode = currentNode;
    queue->head = NULL;
  }
  else if (currentNode->pid == pid && currentNode->next != NULL)
  {
    removedNode = currentNode;
    queue->head = queue->head->next;
    removedNode->next = NULL;
  }
  else if (currentNode->pid != pid && currentNode->next != NULL)
  {
    process_node_t *nextNode = currentNode->next;
    while (currentNode->next != NULL)
    {
      if (nextNode->pid == pid)
      {
        currentNode->next = nextNode->next;
        nextNode->next = NULL;
        removedNode = nextNode;
        break;
      }
      else
      {
        currentNode = currentNode->next;
        nextNode = nextNode->next;
      }
    }
  }
  return removedNode;
}

/* Adds a process into the appropriate singly linked list.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int avan_insert(avan_header_t *header, process_node_t *process)
{
  if (process == NULL)
  {
    printf("The process is NULL. Exit.\n");
    return -1;
  }
  process->flags = 0x00000001 << 0;

  int ifSucess = insertTheProcess(header->ready_queue, process);
  header->ready_queue->count++;
  return ifSucess;
}

/*We use the helper function removeTheProcess to find the node that we want to remove.
  If the return node is Null, it means the queue doesn't have the node with that pid.
  After find it, we use helper function to insert that into the  corresponding queue.
*/
int avan_suspend(avan_header_t *header, pid_t pid)
{

  if (header->ready_queue->head == NULL)
  {
    printf("The ready queue is empty. Exit.\n");
    return -1;
  }
  process_node_t *removedNode = removeTheProcess(header->ready_queue, pid);

  int ifSucess = -1;
  if (removedNode == NULL)
  {
    printf("Sorry, cannot find the process you want to suspend. Exit.\n");
    return -1;
  }
  else
  {
    removedNode->flags = 0x00000001 << 1;
    ifSucess = insertTheProcess(header->suspended_queue, removedNode);
    header->ready_queue->count--;
    header->suspended_queue->count++;
  }
  return ifSucess;
}

/*  The algorithm here is similar to the suspend function.
 */
int avan_resume(avan_header_t *header, pid_t pid)
{

  if (header->suspended_queue->head == NULL)
  {
    printf("The suspend queue is empty. Exit.\n");
    return -1;
  }

  process_node_t *removedNode = removeTheProcess(header->suspended_queue, pid);
  int ifSucess = -1;
  if (removedNode == NULL)
  {
    printf("Sorry, cannot find the process you want to resume. Exit.\n");
    return -1;
  }
  else
  {
    removedNode->flags = 0x00000001 << 0;

    ifSucess = insertTheProcess(header->ready_queue, removedNode);

    header->suspended_queue->count--;
    header->ready_queue->count++;
  }
  return ifSucess;
}

/*We transfer to decimal exit_code to binary and make records which bits does binary has "1".
  Basically we do it to make sure "light up" the bit with "1".
  We do "|" bitwise shift with mask.
  Since there are already four bits from 0 to 3. We need to add 4 to index, to make sure it shift from bit 4.
*/
int avan_quit(avan_header_t *header, process_node_t *node, int exit_code)
{

  if (node == NULL)
  {
    printf("The node is NULL, cannot insert to terminated quque. Exit\n");
    return -1;
  }
  if (exit_code < 0)
  {
    printf("The exit_code is negative. Exit\n");
    return -1;
  }
  node->flags = 0x00000001 << 2;
  int index = 0;
  int myRemainder;
  int mask = 0x00000001;
  while (exit_code > 0)
  {
    myRemainder = exit_code % 2;
    exit_code = exit_code / 2;
    if (myRemainder == 1)
    {
      mask = 0x00000001 << (index + 4);
      node->flags = node->flags | mask;
    }
    index++;
  }
  int ifSucess = insertTheProcess(header->terminated_queue, node);
  header->terminated_queue->count++;

  return ifSucess;
}

/*Since we don't know the terminate node from ready queue or suspend queue.
  The only thing is to create two node and "try" to remove them from both queue.
  If both return "NULL", which means both queues don't have the node corresponding with the pid. We return -1, otherwise, return 0;
*/
int avan_terminate(avan_header_t *header, pid_t pid, int exit_code)
{

  if (header->ready_queue->head == NULL && header->suspended_queue->head == NULL)
  {
    printf("The ready queue and the suspend queue are empty. Cannot terminate. Exit\n");
    return -1;
  }

  process_node_t *removedFromReadyQueue = removeTheProcess(header->ready_queue, pid);
  process_node_t *removedFromSuspendQueue = removeTheProcess(header->suspended_queue, pid);
  if (removedFromReadyQueue == NULL && removedFromSuspendQueue == NULL)
  {
    printf("Both ready queue and suspend cannot find the process of the pid. Cannot terminate. Exit\n");
    return -1;
  }

  if (exit_code < 0)
  {
    printf("The exit_code is negative. Exit\n");
    return -1;
  }

  int successForReady = -1;
  int successForSuspend = -1;
  int finalReturn;
  /*If terminate node removed from ready queue.
    After we find it, we do a bitwise operator.
    We transfer to decimal exit_code to binary and make records which bits does binary has "1".
    Basically we do it to make sure "light up" the bit with "1".
    We do "|" bitwise shift with mask.
    Since there are already four bits from 0 to 3. We need to add 4 to index, to make sure it shift from bit 4.
  */
  if (removedFromReadyQueue != NULL)
  {
    successForReady = 0;
    removedFromReadyQueue->flags = 0x00000001 << 2;
    int index = 0;
    int myRemainder;
    int mask = 0x00000001;
    while (exit_code > 0)
    {
      myRemainder = exit_code % 2;
      exit_code = exit_code / 2;
      if (myRemainder == 1)
      {
        mask = 0x00000001 << (index + 4);
        removedFromReadyQueue->flags = removedFromReadyQueue->flags | mask;
      }
      index++;
    }
    successForReady = insertTheProcess(header->terminated_queue, removedFromReadyQueue);
    header->ready_queue->count--;
    header->terminated_queue->count++;
  }
  /* If terminate node removes from suspend_queue, same algorithm as above.
   */
  if (removedFromSuspendQueue != NULL)
  {
    successForSuspend = 0;
    removedFromSuspendQueue->flags = 0x00000001 << 2;
    int index = 0;
    int myRemainder;
    int mask = 0x00000001;
    while (exit_code > 0)
    {
      myRemainder = exit_code % 2;
      exit_code = exit_code / 2;
      if (myRemainder == 1)
      {
        mask = 0x00000001 << (index + 4);
        removedFromSuspendQueue->flags = removedFromSuspendQueue->flags | mask;
      }
      index++;
    }
    successForSuspend = insertTheProcess(header->terminated_queue, removedFromSuspendQueue);
    header->suspended_queue->count--;
    header->terminated_queue->count++;
  }

  if (successForReady == -1 && successForSuspend == -1)
  {
    printf("cannot find it in ready queue or suspend queue.\n");
    finalReturn = -1;
  }
  else
  {
    finalReturn = exit_code;
  }

  return finalReturn;
}

/* Create a new process_node_t with the given information.
 * - Malloc and copy the command string, don't just assign it!
 * Follow the specification for this function.
 * Returns the process_node_t on success or a NULL on any error.
 */
process_node_t *avan_new_process(char *command, pid_t pid, int priority, int critical)
{

  process_node_t *myProcess = (process_node_t *)malloc(sizeof(process_node_t));
  if (myProcess == NULL)
  {
    printf("Avan_new_process malloc failed.\n");
    return NULL;
  }

  myProcess->flags = 0x00000001 << 0;
  myProcess->priority = priority;

  myProcess->pid = pid;
  myProcess->next = NULL;

  if (critical == 1)
  {
    myProcess->flags = (0x00000001) | (0x00000001 << 3);
  }
  else if (critical == 0)
  {
    myProcess->flags = 0x00000001 << 0;
  }
  else
  {
    printf("The critical should only be 0 or 1.\n");
    return NULL;
  }
  myProcess->skips = 0;
  strcpy(myProcess->cmd, command);

  return myProcess; // Replace Me with Your Code!
}

/* Schedule the next process to run from Ready Queue.
 * Follow the specification for this function.
 * Returns the process selected or NULL if none available or on any errors.
 *
 *
 * Iteratre the ready queue, and:
 * 1. find process with critical flag, mark it as bestNode, else,
 * 2. find starving process, mark it as bestNode, else,
 * 3. find process with lowest priority num.
 * After find the best Node, remove it from ready queue, make it's skips -> 0, make it next->NULL
 * Set all other process in the queue, skip++.
 */
process_node_t *avan_select(avan_header_t *header)
{
  process_node_t *head = header->ready_queue->head;
  if (head == NULL)
  {
    printf("The ready is empty, cannot select!\n");
    return NULL;
  }
  process_node_t *bestNode = NULL;
  int ifCritical = 0;
  int ifStarve = 0;
  int mask = 0x00001000;

  while (head != NULL)
  {
    if ((head->flags & mask) == mask)
    {
      bestNode = head;
      ifCritical = 1;
      break;
    }
    head = head->next;
  }
  if (ifCritical == 0)
  {
    head = header->ready_queue->head;
    while (head != NULL)
    {
      if (head->skips >= MAX_SKIPS)
      {
        bestNode = head;
        ifStarve = 1;
        break;
      }
      head = head->next;
    }
  }
  if (ifCritical == 0 && ifStarve == 0)
  {
    head = header->ready_queue->head;
    bestNode = head;
    while (head != NULL)
    {
      if (head->priority < bestNode->priority)
      {
        bestNode = head;
      }
      head = head->next;
    }
  }

  /*remove the bestNode from the ready queue with three conditions:
    1. only one node in list and the first node is the node we want to remove
    2. more than one node in list and the first node is the node we want to remove
    3. more than one node in list and the first node is not the node we want to remove
   */
  process_node_t *preNode = header->ready_queue->head;
  if (preNode == bestNode && preNode->next == NULL)
  {
    header->ready_queue->head = NULL;
    bestNode->skips = 0;
    header->ready_queue->count--;
  }
  if (preNode == bestNode && preNode->next != NULL)
  {
    header->ready_queue->head = header->ready_queue->head->next;
    bestNode->skips = 0;
    bestNode->next = NULL;
    header->ready_queue->count--;
  }
  if (preNode != bestNode)
  {
    while (preNode != NULL)
    {
      if (preNode->next == bestNode)
      {
        preNode->next = bestNode->next;
        bestNode->skips = 0;
        bestNode->next = NULL;
        break;
      }
      preNode = preNode->next;
    }
    header->ready_queue->count--;
  }

/*Add the rest of other nodes skips++;
*/
  head = header->ready_queue->head;
  while (head != NULL)
  {
    head->skips++;
    head = head->next;
  }

  return bestNode; // Replace Me with Your Code!
}

/* Returns the number of items in a given queue_header_t
 * Follow the specification for this function.
 * Returns the number of processes in the list or -1 on any errors.
 */
int avan_get_size(queue_header_t *ll)
{
  if (ll == NULL)
  {
    printf("The queue is empty, I cannot find the number of processes. Exit\n");
    return -1;
  }
  return ll->count; // Replace Me with Your Code!
}

/*This is the helper function help me to free the link list using recurision.
 */
void deleteNode(process_node_t *process)
{
  if (process->next != NULL)
  {
    deleteNode(process->next);
  }
  free(process);
}

/*Frees all allocated memory in the avan_header_tr
  1. Free link list if there is something in the list.
  2. Free the queue.
  3. Free the header.
*/

void avan_cleanup(avan_header_t *header)
{
  if (header->ready_queue->head != NULL)
  {
    deleteNode(header->ready_queue->head);
  }
  if (header->suspended_queue->head != NULL)
  {
    deleteNode(header->suspended_queue->head);
  }
  if (header->terminated_queue->head != NULL)
  {
    deleteNode(header->terminated_queue->head);
  }

  free(header->ready_queue);
  free(header->suspended_queue);
  free(header->terminated_queue);
  free(header);

  return; 
}
