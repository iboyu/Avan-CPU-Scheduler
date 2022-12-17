/* This is the only file you will be editing.
 * - Copyright of Starter Code: Prof. Kevin Andrea, George Mason University.  All Rights Reserved
 * - Copyright of Student Code: You!  
 * - Restrictions on Student Code: Do not post your code on any public site (eg. Github).
 * -- Feel free to post your code on a PRIVATE Github and give interviewers access to it.
 * - Date: Aug 2022
 */

/* Fill in your Name and GNumber in the following two comment fields
 * Name:
 * GNumber:
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

/* Initialize the avan_header_t Struct
 * Follow the specification for this function.
 * Returns a pointer to the new avan_header_t or NULL on any error.
 */
avan_header_t *avan_create() {
  return NULL; // Replace Me with Your Code!
}

/* Adds a process into the appropriate singly linked list.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int avan_insert(avan_header_t *header, process_node_t *process) {
  return -1; // Replace Me with Your Code!
}

/* Move the process with matching pid from Ready to Suspended queue.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error (such as process not found).
 */
int avan_suspend(avan_header_t *header, pid_t pid) {
  return -1; // Replace Me with Your Code!
}

/* Move the process with matching pid from Suspended to Ready queue.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error (such as process not found).
 */
int avan_resume(avan_header_t *header, pid_t pid) {
  return -1; // Replace Me with Your Code!
}

/* Insert the process in the Terminated Queue and add the Exit Code to it.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int avan_quit(avan_header_t *header, process_node_t *node, int exit_code) {
  return -1; // Replace Me with Your Code!
}

/* Move the process with matching pid from Ready to Terminated and add the Exit Code to it.
 * Follow the specification for this function.
 * Returns its exit code (from flags) on success or a -1 on any error.
 */
int avan_terminate(avan_header_t *header, pid_t pid, int exit_code) {
  return -1; // Replace Me with Your Code!
}

/* Create a new process_node_t with the given information.
 * - Malloc and copy the command string, don't just assign it!
 * Follow the specification for this function.
 * Returns the process_node_t on success or a NULL on any error.
 */
process_node_t *avan_new_process(char *command, pid_t pid, int priority, int critical) {
  return NULL; // Replace Me with Your Code!
}

/* Schedule the next process to run from Ready Queue.
 * Follow the specification for this function.
 * Returns the process selected or NULL if none available or on any errors.
 */
process_node_t *avan_select(avan_header_t *header) {
  return NULL; // Replace Me with Your Code!
}

/* Returns the number of items in a given queue_header_t
 * Follow the specification for this function.
 * Returns the number of processes in the list or -1 on any errors.
 */
int avan_get_size(queue_header_t *ll) {
  return -1; // Replace Me with Your Code!
}

/* Frees all allocated memory in the avan_header_tr */
void avan_cleanup(avan_header_t *header) {
  return; // Replace Me with Your Code!
}
