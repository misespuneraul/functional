#include "tests.h"
#include "functional.h"
#include "tasks.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

static array_t create_array(int len, int elem_size, void (*destructor)(void *))
{
	array_t list;
	list.elem_size = elem_size;
	list.len = len;
	list.destructor = destructor;
	list.data = malloc(list.len * list.elem_size);
	return list;
}

static void student_destructor(void *elem)
{
	free(((student_t *)elem)->name);
}

static void string_destructor(void *elem)
{
	free(*(char **)elem);
}

static void simple_list_destructor(void *elem)
{
	free(((array_t *)elem)->data);
}

static void read_int(void *elem)
{
	fscanf(stdin, "%d ", (int *)elem);
}

static void read_student(void *elem)
{
	student_t *s = (student_t *)elem;
	s->name = malloc(100);
	fscanf(stdin, "%s %d %f", s->name, &s->age, &s->grade);
}

static void read_string(void *elem)
{
	char **s = (char **)elem;
	*s = malloc(100);
	fscanf(stdin, "%s", *s);
}

static void read_float(void *elem)
{
	fscanf(stdin, "%f ", (float *)elem);
}

static void read_int_list(void *elem)
{
	int len;
	fscanf(stdin, "%d ", &len);
	array_t *array = (array_t *)elem;
	*array = create_array(len, sizeof(int), NULL);

	for_each(read_int, *array);
}

static void write_int(void *elem)
{
	fprintf(stdout, "%d ", *(int *)elem);
}

static void write_boolean(void *elem)
{
	fprintf(stdout, "%s ", *(boolean *)elem ? "true" : "false");
}

static void write_student(void *elem)
{
	student_t *s = (student_t *)elem;
	fprintf(stdout, "%s %d %.1f\n", s->name, s->age, s->grade);
}

static void write_number(void *elem)
{
	number_t *n = (number_t *)elem;
	fprintf(stdout, "%s\n", n->string);
}

static void write_string(void *elem)
{
	fprintf(stdout, "%s ", *(char **)elem);
}

static void write_3_int(void **elems)
{
	fprintf(stdout, "%d %d %d\n",
			*(int *)elems[0], *(int *)elems[1], *(int *)elems[2]);
}

static void write_list(void *elem)
{
	array_t *list = (array_t *)elem;
	for_each(write_int, *list);
	fprintf(stdout, "\n");
}

static void inc_elem(void *elem)
{
	int *i = (int *)elem;
	(*i)++;
}

static void extract_age_from_student(void *new_elem, void *elem)
{
	memcpy(new_elem, &((student_t *)elem)->age, sizeof(int));
}

static void create_student(void *new_elem, void **elems)
{
	student_t *s = (student_t *)new_elem;
	s->name = malloc(100);
	strcpy(s->name, *(char **)elems[0]);
	s->age = *(int *)elems[1];
	s->grade = *(float *)elems[2];
}

static boolean is_even(void *elem)
{
	return *(int *)elem % 2 == 0;
}

static void add(void *acc, void *elem)
{
	*(int *)acc += *(int *)elem;
}

static void minimum_3(void *acc, void **elems)
{
	int min = BIG_ENOUGH;
	for (int i = 0; i < 3; i++) {
		if (*(int *)elems[i] < min)
			min = *(int *)elems[i];
	}

	if (min < *(int *)acc)
		*(int *)acc = min;
}

void test_for_each(void)
{
	// Given
	int len;
	fscanf(stdin, "%d", &len);
	array_t list = create_array(len, sizeof(int), NULL);

	for_each(read_int, list);

	// When
	for_each(inc_elem, list);

	// Then
	for_each(write_int, list);

	// Cleanup
	free(list.data);
}

void test_map(void)
{
	// Given
	int len;
	fscanf(stdin, "%d", &len);
	array_t list = create_array(len, sizeof(student_t), student_destructor);

	for_each(read_student, list);

	// When
	array_t new_list = map(extract_age_from_student, sizeof(int), NULL, list);

	// Then
	for_each(write_int, new_list);

	// Cleanup
	free(new_list.data);
}

void test_filter(void)
{
	// Given
	int len;
	fscanf(stdin, "%d", &len);
	array_t list = create_array(len, sizeof(int), NULL);

	for_each(read_int, list);

	// When
	array_t new_list = filter(is_even, list);

	// Then
	for_each(write_int, new_list);

	// Cleanup
	free(new_list.data);
}

void test_reduce(void)
{
	// Given
	int len;
	fscanf(stdin, "%d", &len);
	array_t list = create_array(len, sizeof(int), NULL);

	for_each(read_int, list);

	// When
	int sum = 0;
	reduce(add, &sum, list);

	// Then
	write_int(&sum);

	// Cleanup
	free(list.data);
}

void test_for_each_multiple(void)
{
	// Given
	const int num_lists = 3;

	int len;
	fscanf(stdin, "%d", &len);
	array_t lists[num_lists];

	for (int i = 0; i < num_lists; i++) {
		lists[i] = create_array(len, sizeof(int), NULL);
		for_each(read_int, lists[i]);
	}

	// When
	for_each_multiple(write_3_int, num_lists, lists[0], lists[1], lists[2]);

	// Cleanup
	for (int i = 0; i < num_lists; i++)
		free(lists[i].data);
}

void test_map_multiple(void)
{
	// Given
	int len;
	fscanf(stdin, "%d", &len);
	array_t name_list = create_array(len, sizeof(char *), string_destructor);
	array_t age_list = create_array(len, sizeof(int), NULL);
	array_t grade_list = create_array(len, sizeof(float), NULL);

	for_each(read_string, name_list);
	for_each(read_int, age_list);
	for_each(read_float, grade_list);

	// When
	array_t new_list = map_multiple(create_student,
									sizeof(student_t),
									student_destructor, 3,
									name_list, age_list,
									grade_list);

	// Then
	for_each(write_student, new_list);

	// Cleanup
	for_each(student_destructor, new_list);
	free(new_list.data);
}

void test_reduce_multiple(void)
{
	// Given
	const int num_lists = 3;

	int len;
	fscanf(stdin, "%d", &len);
	array_t lists[num_lists];

	for (int i = 0; i < num_lists; i++) {
		lists[i] = create_array(len, sizeof(int), NULL);
		for_each(read_int, lists[i]);
	}

	// When
	int min = BIG_ENOUGH;
	reduce_multiple(minimum_3, &min, num_lists, lists[0], lists[1], lists[2]);

	// Then
	write_int(&min);

	// Cleanup
	for (int i = 0; i < num_lists; i++)
		free(lists[i].data);
}

void test_reverse(void)
{
	// Given
	int len;
	fscanf(stdin, "%d", &len);
	array_t list = create_array(len, sizeof(int), NULL);

	for_each(read_int, list);

	// When
	array_t new_list = reverse(list);

	// Then
	for_each(write_int, new_list);

	// Cleanup
	free(list.data);
	free(new_list.data);
}

void test_create_number_array(void)
{
	// Given
	int len;
	fscanf(stdin, "%d", &len);
	array_t integer_part = create_array(len, sizeof(int), NULL);
	array_t fractional_part = create_array(len, sizeof(int), NULL);

	for_each(read_int, integer_part);
	for_each(read_int, fractional_part);

	// When
	array_t new_list = create_number_array(integer_part, fractional_part);

	// Then
	for_each(write_number, new_list);

	// Cleanup
	for_each(new_list.destructor, new_list);
	free(new_list.data);
}

void test_get_passing_students_names(void)
{
	// Given
	int len;
	fscanf(stdin, "%d", &len);
	array_t list = create_array(len, sizeof(student_t), student_destructor);

	for_each(read_student, list);

	// When
	array_t new_list = get_passing_students_names(list);

	// Then
	for_each(write_string, new_list);

	// Cleanup
	for_each(string_destructor, new_list);
	free(new_list.data);
}

void test_check_bigger_sum(void)
{
	// Given
	int len;
	fscanf(stdin, "%d", &len);
	array_t list_list = create_array(len,
									 sizeof(array_t),
									 simple_list_destructor);
	for_each(read_int_list, list_list);

	array_t int_list = create_array(len, sizeof(int), NULL);
	for_each(read_int, int_list);

	// When
	array_t new_list = check_bigger_sum(list_list, int_list);

	// Then
	for_each(write_boolean, new_list);

	// Cleanup
	free(new_list.data);
}

void test_get_even_indexed_strings(void)
{
	// Given
	int len;
	fscanf(stdin, "%d", &len);
	array_t list = create_array(len, sizeof(char *), string_destructor);
	for_each(read_string, list);

	// When
	array_t new_list = get_even_indexed_strings(list);

	// Then
	for_each(write_string, new_list);

	// Cleanup
	for_each(string_destructor, new_list);
	free(new_list.data);
}

void test_generate_square_matrix(void)
{
	// Given
	int n;
	fscanf(stdin, "%d", &n);

	// When
	array_t list = generate_square_matrix(n);

	// Then
	for_each(write_list, list);

	// Cleanup
	for_each(list.destructor, list);
	free(list.data);
}
