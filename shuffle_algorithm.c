/*
 * See:
 *   http://benpfaff.org/writings/clc/shuffle.html
 */

#include <stdlib.h>

void
shuffle_int_array (int *array, int size)
{
  int index;
  int rand_index;
  int temp;

  for (index = 0; index < size - 1; index++)
  {
    rand_index = index + rand () / (RAND_MAX / (size - index) + 1);
    temp = array[rand_index];
    array[rand_index] = array[index];
    array[index] = temp;
  }
}

