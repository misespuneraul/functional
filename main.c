#include "tests.h"
#include "tasks.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

test_options get_test_case(char *input)
{
	// Determine option based on input
	if (!strcmp(input, "filter"))
		return FILTER;
	else if (!strcmp(input, "map"))
		return MAP;
	else if (!strcmp(input, "for_each"))
		return FOR_EACH;
	else if (!strcmp(input, "reduce"))
		return REDUCE;
	else if (!strcmp(input, "for_each_multi"))
		return FOR_EACH_MULTI;
	else if (!strcmp(input, "map_multi"))
		return MAP_MULTI;
	else if (!strcmp(input, "reduce_multi"))
		return REDUCE_MULTI;
	else if (!strcmp(input, "reverse"))
		return REVERSE;
	else if (!strcmp(input, "create_number_array"))
		return CREATE_NUMBER_ARRAY;
	else if (!strcmp(input, "get_passing_students_names"))
		return GET_PASSING_STUDENTS_NAMES;
	else if (!strcmp(input, "check_bigger_sum"))
		return CHECK_BIGGER_SUM;
	else if (!strcmp(input, "get_even_indexed_strings"))
		return GET_EVEN_INDEXED_STRINGS;
	else if (!strcmp(input, "generate_square_matrix"))
		return GENERATE_SQUARE_MATRIX;

	return ERROR;
}

int main(void) {
	char *input = malloc(100);
	// Read at least 100 characters or until we hit \n
	fscanf(stdin, "%100[^\n]s", input);

	test_options option = get_test_case(input);

	// Switch based on option
	switch (option) {
	// Task 1
	case FILTER:
		test_filter();
		break;
	case MAP:
		test_map();
		break;
	case FOR_EACH:
		test_for_each();
		break;
	case REDUCE:
		test_reduce();
		break;
	// Task 2
	case MAP_MULTI:
		test_map_multiple();
		break;
	case FOR_EACH_MULTI:
		test_for_each_multiple();
		break;
	case REDUCE_MULTI:
		test_reduce_multiple();
		break;
	// Task 3
	case REVERSE:
		test_reverse();
		break;
	case CREATE_NUMBER_ARRAY:
		test_create_number_array();
		break;
	case GET_PASSING_STUDENTS_NAMES:
		test_get_passing_students_names();
		break;
	case CHECK_BIGGER_SUM:
		test_check_bigger_sum();
		break;
	case GET_EVEN_INDEXED_STRINGS:
		test_get_even_indexed_strings();
		break;
	case GENERATE_SQUARE_MATRIX:
		test_generate_square_matrix();
		break;
	default:
		printf("Invalid option\n");
		return 1;
	}

	free(input);

	return 0;
}
