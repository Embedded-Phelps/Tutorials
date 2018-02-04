/**
 * @file dlinkedlist_unittest.c
 * @brief The source file for the unit testing of the doubly linked list
 *
 * @author Shuting Guo
 * @date 01/30/2018
 * @version 1.0
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>


#include "dlinkedlist.h"

#define SKIP_TEST (skip())

void test_get_list_container(void ** state)
{
  dll_t list = {
    .data = 1,
    .node.next = NULL,
    .node.prev = NULL,
  };
  assert_ptr_equal(&list, GET_LIST_CONTAINER(&list.node, dll_t, node));
}

void test_dll_destroy(void ** state)
{
  dll_t * pList = NULL;
  /* CASE 1: Destroy a empty linked list */
  assert_null(dll_destroy(&pList));

  /* CASE 2: Destroy a linked list with only one node */
  dll_insert_at_beginning(&pList, 0);
  assert_null(dll_destroy(&pList));

  /* CASE 3: Destroy a linked list with more than one node */
  dll_insert_at_beginning(&pList, 0);
  dll_insert_at_beginning(&pList, 0);
  assert_null(dll_destroy(&pList));
}

void test_dll_insert_at_beginning(void ** state)
{
  dll_t *pList = NULL, *old_pList = NULL;
  /* CASE 1: Linked list does not have a node */
  dll_insert_at_beginning(&pList, 0);
  assert_true(pList->data == 0);
  assert_null((void*)pList->node.prev); // would the void cast be a problem?
  assert_null((void*)pList->node.next);

  /* CASE 2: Linked list is not empty */
  old_pList = pList;
  dll_insert_at_beginning(&pList, 1);
  assert_true(pList->data == 1);
  assert_null((void*)pList->node.prev);
  assert_ptr_equal((void*)pList->node.next, (void*)&old_pList->node);
  assert_ptr_equal((void*)old_pList->node.prev, (void*)&pList->node);

  // Deallocate memory
  dll_destroy(&pList);
}

void test_dll_insert_at_end(void ** state)
{
  dll_t *pList = NULL;
  /* CASE 1: Linked list does not have a node */
  dll_insert_at_end(&pList, 0);
  assert_true(pList->data == 0);
  assert_null((void*)pList->node.prev); // would the void cast be a problem?
  assert_null((void*)pList->node.next);

  /* CASE 2: Linked list is not empty */
  dll_insert_at_end(&pList,1);
  assert_non_null((void*)pList->node.next);
  assert_true(GET_LIST_CONTAINER(pList->node.next, dll_t, node)->data == 1);
  assert_ptr_equal((void*)pList->node.next->prev, (void*)&pList->node);
  assert_null((void*)pList->node.next->next);

  /* Deallocate memory */
  dll_destroy(&pList);
}

void test_dll_insert_at_position(void ** state)
{
  dll_t * pList = NULL;
  /* CASE 1: NULL Pointer and position is not 0 */
  assert_null(dll_insert_at_position(&pList, 0, 2));

  /* CASE 2: NULL Pointer and position is 0 */
  dll_insert_at_position(&pList, 0, 0);
  assert_true(pList->data == 0);
  assert_null(pList->node.prev);
  assert_null(pList->node.next);

  /* CASE 3: Non-NULL list insert at position 0 */
  dll_node_t * temp_node = &pList->node;
  dll_insert_at_position(&pList, 1, 0);
  assert_true(pList->data == 1);
  assert_null(pList->node.prev);
  assert_ptr_equal(pList->node.next, temp_node);

  /* CASE 4: Insert at a middle position */
  dll_insert_at_position(&pList, 2, 1);
  assert_true(GET_LIST_CONTAINER(pList->node.next, dll_t, node)->data == 2);
  assert_ptr_equal(&pList->node, pList->node.next->prev);
  assert_ptr_equal(pList->node.next->next, temp_node);
  assert_ptr_equal(pList->node.next, temp_node->prev);

  /* CASE 5: Insert at the end */
  dll_insert_at_position(&pList, 3, dll_size(&pList));
  assert_true(GET_LIST_CONTAINER(temp_node->next, dll_t, node)->data == 3);
  assert_ptr_equal(temp_node, temp_node->next->prev);
  assert_null(temp_node->next->next);

  /* CASE 6: Insert position out of range */
  dll_insert_at_position(&pList, 4, 100);
  assert_true(dll_size(&pList) == 4);

  // Deallocate memory
  dll_destroy(&pList);
}

void test_dll_delete_from_beginning(void ** state)
{
  dll_t *pList = NULL;
  /* CASE 1: NULL pointer */
  assert_null(dll_delete_from_beginning(&pList));

  /* CASE 2: The liked list has more than one node */
  dll_insert_at_beginning(&pList, 0);
  dll_insert_at_end(&pList, 1);
  dll_delete_from_beginning(&pList);
  assert_true(pList->data == 1);
  assert_null(pList->node.prev);
  assert_null(pList->node.next);
  dll_delete_from_beginning(&pList);

  /* CASE 3: The linked list has only one node */
  assert_null(dll_delete_from_beginning(&pList));
}

void test_dll_delete_from_end(void ** state)
{
  /* CASE 1: NULL pointer */
  dll_t *pList = NULL;
  assert_null(dll_delete_from_end(&pList));

  /* CASE 2: The linked list has more than one node */
  dll_insert_at_beginning(&pList, 0);
  dll_insert_at_end(&pList, 1);
  dll_delete_from_end(&pList);
  assert_true(pList->data == 0);
  assert_null(pList->node.next);
  assert_null(pList->node.prev);

  /* CASE 3: The liked list has only one node */
  assert_null(dll_delete_from_end(&pList));
}

void test_dll_delete_from_position(void ** state)
{
  dll_t *pList = NULL;
  /* CASE 1: NULL pointer */
  assert_null(dll_delete_from_position(&pList, 0));

  /* CASE 2: The list has one node, delete position non zero */
  dll_insert_at_beginning(&pList, 0);
  dll_delete_from_position(&pList, 3);
  assert_true(dll_size(&pList) == 1);

  /* CASE 3: The list has one node, delete position 0 */
  assert_null(dll_delete_from_position(&pList, 0));

  /* CASE 4: The list has more than one nodes, delete the head */
  dll_insert_at_beginning(&pList, 0);
  dll_insert_at_end(&pList, 1);
  dll_insert_at_end(&pList, 2);
  dll_insert_at_end(&pList, 3);
  dll_insert_at_end(&pList, 4);
  dll_delete_from_position(&pList, 0);
  assert_true(pList->data == 1);
  assert_null(pList->node.prev);
  assert_true(dll_size(&pList) == 4);

  /* CASE 5: The list has more than one node, delete a middle node */
  dll_delete_from_position(&pList, 1);
  assert_true(GET_LIST_CONTAINER(pList->node.next, dll_t, node)->data == 3);
  assert_ptr_equal(pList->node.next->prev, &pList->node);
  assert_true(dll_size(&pList) == 3);

  /* CASE 6: The list has more than one node, delete the end node */
  dll_delete_from_position(&pList, dll_size(&pList)-1);
  assert_true(dll_size(&pList) == 2);
  assert_null(pList->node.next->next);

  /* CASE 7: The list has more than one node, delete position out of range */
  dll_delete_from_position(&pList, 100);
  assert_true(dll_size(&pList) == 2);

  // Deallocate memory
  dll_destroy(&pList);
}

void test_dll_size(void ** state)
{
  /* CASE 1: The linked list has no node */
  dll_t *pList = NULL;
  assert_true(dll_size(&pList)==0);

  /* CASE 2: The linked list has nodes */
  dll_insert_at_beginning(&pList, 0);
  dll_insert_at_end(&pList, 1);
  assert_true(dll_size(&pList)==2);

  // Deallocate memory
  dll_destroy(&pList);
}

int main(int argc, char ** argv)
{
  const struct CMUnitTest tests[] =
  {
    cmocka_unit_test(test_get_list_container),
    cmocka_unit_test(test_dll_destroy),
    cmocka_unit_test(test_dll_insert_at_beginning),
    cmocka_unit_test(test_dll_insert_at_end),
    cmocka_unit_test(test_dll_insert_at_position),
    cmocka_unit_test(test_dll_delete_from_beginning),
    cmocka_unit_test(test_dll_delete_from_end),
    cmocka_unit_test(test_dll_delete_from_position),
    cmocka_unit_test(test_dll_size),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
