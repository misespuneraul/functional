#ifndef TASKS_H
#define TASKS_H

// Uncomment the following lines to mark the tasks as implemented
#define DONE_FOR_EACH
#define DONE_MAP
#define DONE_FILTER
#define DONE_REDUCE

#define DONE_FOR_EACH_MULTIPLE
#define DONE_MAP_MULTIPLE
#define DONE_REDUCE_MULTIPLE

#define DONE_REVERSE
#define DONE_CREATE_NUMBER_ARRAY
#define DONE_GET_PASSING_STUDENTS_NAMES
#define DONE_CHECK_SUM_BIGGER
#define DONE_GET_EVEN_INDEXED_STRINGS
#define DONE_GENERATE_SQUARE_MATRIX

#include "functional.h"

typedef struct {
	int integer_part;
	int fractional_part;
	char *string;
} number_t;

typedef struct student_t {
	char *name;
	int age;
	float grade;
} student_t;

/**
 * @brief Reverse a given list of integers.
 * The received list will be freed by the caller.
 *
 * @param list The list to reverse.
 * @return The reversed list.
 */
array_t reverse(array_t list);

/**
 * @brief Create a list of numbers from two lists of integers.
 * Numbers are represented as a struct(number_t) containing an integer
 * part, a fractional part and a string representation (remember the
 * destructor).
 * The received lists will be freed by the callee.
 *
 * @param integer_part The list of integer parts.
 * @param fractional_part The list of fractional parts.
 * @return The list of numbers.
 */
array_t create_number_array(array_t integer_part, array_t fractional_part);

/**
 * @brief Get the names of the students that passed the course.
 * To pass the course, the student must have a grade greater or
 * equal to 5.0.
 * The received list will be freed by the callee.
 *
 * @param list The list of students (student_t).
 * @return The list of names of the students that passed the course.
 */
array_t get_passing_students_names(array_t list);

/**
 * @brief Check if the sum of the integers in the list is greater
 * or equal to the given sum.
 * The received lists will be freed by the callee.
 *
 * @param list_list The list of lists of integers.
 * @param int_list The list of sums to check against.
 * @return list of booleans, where each element is true if the sum
 * of the corresponding list is greater or equal to the sum in int_list.
 */
array_t check_bigger_sum(array_t list_list, array_t int_list);

/**
 * @brief Get the strings at even indexes from the given list.
 * The received list will be freed by the callee.
 * @param list The list of strings.
 * @return The list of strings at even indexes.
 */
array_t get_even_indexed_strings(array_t list);

/**
 * @brief Generate a square matrix of size n x n.
 * The matrix is represented as a list of lists of integers.
 * The values on each line with be in increasing order starting
 * from line index + 1.
 * Don't forget the destructor.
 *
 * @param n The size of the matrix.
 * @return The generated matrix.
 */
array_t generate_square_matrix(int n);

#endif // TASKS_H
