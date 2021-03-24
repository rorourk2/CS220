#include "node.h"
#include <CUnit/Basic.h>
#include <CUnit/CUCurses.h>
#include <CUnit/Console.h>
#include <CUnit/TestRun.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_NODES 10

extern unsigned long toggle_bit(unsigned long num, unsigned long bit_num);
Node *test_head = NULL;
Node *recovery[NUM_NODES] = {0};

/*
 * Print the address and contents of a Node.
 */
void _printNode(Node *n) {
  if (n != NULL) {
    printf("Node [%p] = {.next=%p, .prev=%p}\n", n, n->next, n->prev);
  } else {
    printf("(nil)\n");
  }
}

/*
 * Print each Node in a linked list of Nodes beginning with n (n may be a node *
 * in middle of list).
 */
void _printNodeList(Node *n) {
  while (n != NULL) {
    _printNode(n);
    n = n->next;
  }
}

/*
 * Generate a linked list of length @count with randomized contents
 */
Node *_gen_list(unsigned int count) {
  Node *head, *tail, *curr;
  head = tail = curr = NULL;
  for (size_t i = 0; i < count; i++) {
    curr = (Node *)malloc(sizeof(Node));
    memset(curr, 0, sizeof(Node));
    if (head == NULL) {
      head = tail = curr;
    } else {
      tail->next = curr;
      curr->prev = tail;
      tail = curr;
    }
  }
  return head;
}

/* The suite initialization function.
 * Allocates and links NUM_NODES nodes on the heap.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite(void) {

  // Generate a linked list for Testing
  test_head = _gen_list(NUM_NODES);

  // Populate the recovery list
  Node *curr = test_head;
  for (size_t i = 0; i < NUM_NODES; i++) {
    recovery[i] = curr;
    if (curr != NULL)
      curr = curr->next;
  }

  // Print the generated linked list
  _printNodeList(test_head);

  return 0;
}

/* The suite cleanup function.
 * Frees allocated nodes on heap.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite(void) {
  for (size_t i = 0; i < NUM_NODES; i++) {
    if (recovery[i] != NULL)
      free(recovery[i]);
  }

  return 0;
}

/*
 * Test student's sort_nodes function.
 */
void testSORT_NODES(void) {
  Node *curr = sort_nodes(test_head);
  Node *next = NULL;

  // Initialize the pointers for traversal
  if (curr != NULL) {
    next = curr->next;
  } else {
    CU_FAIL_FATAL("Fatal failure. Something broke in sort_nodes!");
  }

  // Traverse and test
  while (next != NULL) {
    CU_ASSERT_TRUE_FATAL(curr > next);
    curr = next;
    next = curr->next;
  }
}

/*
 * Test student's toggle function.
 */
void testTOGGLE(void) {
  CU_ASSERT_EQUAL(toggle_bit(0xDEADBEEFDEADBEEF, 0), 0xDEADBEEFDEADBEEE);
  CU_ASSERT_EQUAL(toggle_bit(0xDEADBEEFDEADBEEF, 63), 0x5EADBEEFDEADBEEF);
  CU_ASSERT_EQUAL(toggle_bit(0xDEADBEEFDEADBEEF, 15), 0xDEADBEEFDEAD3EEF);
  CU_ASSERT_EQUAL(toggle_bit(0x0, 0), 0x1);
  CU_ASSERT_EQUAL(toggle_bit(0x0, 63), 0x8000000000000000);
  CU_ASSERT_EQUAL(toggle_bit(0xFFFFFFFFFFFFFFFF, 47), 0xFFFF7FFFFFFFFFFF);
  CU_ASSERT_EQUAL(toggle_bit(0xFFFFFFFFFFFFFFFF, 63), 0x7FFFFFFFFFFFFFFF);
}

int main(int argc, char const *argv[]) {

  // Argparse to find curses or console mode
  char curses = 0;
  char console = 0;
  size_t i;
  for (i = 0; i < argc; i++) {
    if (strcmp(argv[i], "--curses") == 0) {
      curses = 1;
      console = 0;
    }
    if (strcmp(argv[i], "--console") == 0) {
      curses = 0;
      console = 1;
    }
  }

  CU_pSuite pSuite = NULL;

  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  /* add a suite to the registry */
  pSuite = CU_add_suite("Suite_A8", init_suite, clean_suite);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests to the suite */
  if ((NULL == CU_add_test(pSuite, "Testing Sort Nodes function ",
                            testSORT_NODES)) ||
      (NULL == CU_add_test(pSuite, "Testing Toggle function ",
                            testTOGGLE))){
    // Adding one or more tests failed. Cleanup!
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Run tests */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  if (curses == 1) {
    CU_curses_run_tests();
  } else if (console == 1) {
    CU_console_run_tests();
  } else {
    CU_basic_run_tests();
  }
  CU_cleanup_registry();
  return CU_get_error();
}
