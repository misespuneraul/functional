#ifndef TESTS_H
#define TESTS_H

typedef enum test_options {
	FILTER,
	MAP,
	FOR_EACH,
	REDUCE,
	FOR_EACH_MULTI,
	MAP_MULTI,
	REDUCE_MULTI,
	REVERSE,
	CREATE_NUMBER_ARRAY,
	GET_PASSING_STUDENTS_NAMES,
	CHECK_BIGGER_SUM,
	GET_EVEN_INDEXED_STRINGS,
	GENERATE_SQUARE_MATRIX,
	ERROR = -1,
} test_options;

/**
 * @brief Tests for_each implementation
 * Read input from file and print output to file
 *
 */
void test_for_each(void);

/**
 * @brief Tests map implementation
 * Read input from file and print output to file
 *
 */
void test_map(void);

/**
 * @brief Tests filter implementation
 * Read input from file and print output to file
 *
 */
void test_filter(void);

/**
 * @brief Tests reduce implementation
 * Read input from file and print output to file
 *
 */
void test_reduce(void);

/**
 * @brief Tests for_each_multiple implementation
 * Read input from file and print output to file
 *
 */
void test_for_each_multiple(void);

/**
 * @brief Tests map_multiple implementation
 * Read input from file and print output to file
 *
 */
void test_map_multiple(void);

/**
 * @brief Tests filter_multiple implementation
 * Read input from file and print output to file
 *
 */
void test_reduce_multiple(void);

/**
 * @brief Tests reverse task implementation
 * Read input from file and print output to file
 *
 */
void test_reverse(void);

/**
 * @brief Tests create_number_array task implementation
 * Read input from file and print output to file
 *
 */
void test_create_number_array(void);

/**
 * @brief Tests get passing students task implementation
 * Read input from file and print output to file
 *
 */
void test_get_passing_students_names(void);

/**
 * @brief Tests check bigger sum task implementation
 * Read input from file and print output to file
 *
 */
void test_check_bigger_sum(void);

/**
 * @brief Tests get even indexed strings task implementation
 * Read input from file and print output to file
 *
*/
void test_get_even_indexed_strings(void);

/**
 * @brief Tests generate square matrix task implementation
 * Read input from file and print output to file
 *
*/
void test_generate_square_matrix(void);

#endif // TESTS_H
