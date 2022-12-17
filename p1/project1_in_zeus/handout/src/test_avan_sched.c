/* FEEL FREE TO EDIT THIS FILE
 * - test_schedule.c (Trilby VM)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
// Local Includes
#include "vm_support.h" // Gives abort_error, print_warning, print_status, print_debug commands
#include "avan_sched.h" // Your header for the functions you're testing.

int debug_mode = 1; // Hardcodes debug on for the custom print functions

// Local Prototypes
void test_scheduler_create();
void test_create_process();

int main() {
  print_status("Test 1: Testing Scheduler Create");
  //test_scheduler_create();

  test_create_process();

  return 0;
}

void test_create_process(){
  avan_header_t *header = avan_create();

  //process_node_t *avan_new_process(char *command, pid_t pid, int priority, int critical) {
  process_node_t *myProcess1 = avan_new_process("aa", 100, 15, 1);
  process_node_t *myProcess2 = avan_new_process("bb", 200, 7, 0);
  process_node_t *myProcess3 = avan_new_process("cc", 300, 9, 1);
  process_node_t *myProcess4 = avan_new_process("dd", 400, 2, 1);

//aa,bb,cc,dd

//int avan_insert(avan_header_t *header, process_node_t *process) {
  int ifSuccess = avan_insert(header, myProcess1);
  ifSuccess = avan_insert(header, myProcess2);
  ifSuccess = avan_insert(header, myProcess3);
  ifSuccess = avan_insert(header, myProcess4);

  printf("The number of ready queue!!!!!!!!!!!!!!!!!!!!!!! %d\n",header->ready_queue->head->pid);
  MARK("If successfullt insert is %d\n",ifSuccess);


print_avan_debug(header);


printf("The number of ready queue %d\n",header->ready_queue->count);
printf("The number of suspend queue %d\n",header->suspended_queue->count);
printf("The number of terminate queue %d\n",header->terminated_queue->count);


//int avan_quit(avan_header_t *header, process_node_t *node, int exit_code);
 //ifSuccess = avan_quit(header, myProcess3, 6);
// MARK("If successfullt quit is %d\n",ifSuccess);
// print_avan_debug(header);
// printf("The number of ready queue %d\n",header->ready_queue->count);
// printf("The number of suspend queue %d\n",header->suspended_queue->count);
// printf("The number of terminate queue %d\n",header->terminated_queue->count);


  //int avan_terminate(avan_header_t *header, pid_t pid, int exit_code) {     
  ifSuccess = avan_terminate(header, 300, 6); //cc
  
    print_avan_debug(header);
  //  printf("If success terminate is %d\n",ifSuccess);
    avan_cleanup(header);

}



















void test_scheduler_create() {
  print_debug("...Calling avan_create()");
  MARK("I can be used anywhere, even if debug mode is off.\n");
  MARK("I work just like printf! %s %d %lf\n", "Cool!", 42, 3.14);
  avan_header_t *header = avan_create();
  if(header == NULL) {
    abort_error("...avan_create returned NULL!", __FILE__);
  }
  if(header->ready_queue == NULL || header->suspended_queue == NULL || header->terminated_queue == NULL) {
    abort_error("...avan_create returned at least one NULL queue.", __FILE__);
  }
  if(header->ready_queue->count != 0) {
    print_warning("...The count on Ready Queue is not 0!");
  }

  print_status("...Printing the Schedule");
  print_avan_debug(header);
  print_status("...avan_create is looking good so far.");
}
