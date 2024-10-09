#include "functional.h"
#include "tasks.h"
#include "tests.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// structura care retine un string si indexul sau
typedef struct indexed_string_t {
	char *str;
	int index;
} indexed_string_t;

// creare array
array_t create_array(int len, int elem_size, void (*destructor)(void *))
{
	array_t list;
	list.elem_size = elem_size;
	list.len = len;
	list.destructor = destructor;
	list.data = malloc(list.len * list.elem_size);
	return list;
}

void get_reverse(void *list, void *element) {
	char *first = (char *)(*(array_t *)list).data;
	int size = (*(array_t *)list).elem_size;
	char *next = (char *)(*(array_t *)list).data + size;

	// shiftarea elementelor din lista rezultat cu o pozitie
	memmove(next, first, ((*(array_t *)list).len) * size);

	// copierea elementului in lista rezultat
	memcpy((*(array_t *)list).data, element, size);

	// incrementarea lungimii listei
	(*(array_t *)list).len++;
}

void get_reverse_indexed(void *list, void *element) {
	// functia se bazeaza pe get_reverse
	char *first = (char *)(*(array_t *)list).data;
	int size = (*(array_t *)list).elem_size;
	char *next = (char *)(*(array_t *)list).data + size;

	// shiftarea elementelor cu o pozitie
	memmove(next, first, ((*(array_t *)list).len) * size);

	// destinatia este un array de tip indexed string
	indexed_string_t *current = (indexed_string_t *)(*(array_t *)list).data;
	current->str = malloc(100);

	// copierea stringului
	strcpy(current->str, *(char **)element);

	// salvarea in index a lungimii curente a listei de indexed strings
	current->index = (int)(*(array_t *)list).len;
	(*(array_t *)list).len++;
}

void get_reverse_indexed_even(void *list, void *element) {
	// singura diferenta pentru cazul acesta este ca
	// incrementarea lungimii listei se face inainte, nu dupa, pentru ca
	// indecsii sa si mentina paritatea corecta
	(*(array_t *)list).len++;
	char *first = (char *)(*(array_t *)list).data;
	int size = (*(array_t *)list).elem_size;
	char *next = (char *)(*(array_t *)list).data + size;

	// mutarea se face cu lungimea - 1 in acest caz
	memmove(next, first, ((*(array_t *)list).len - 1) * size);
	indexed_string_t *current = (indexed_string_t *)(*(array_t *)list).data;
	current->str = malloc(100);
	strcpy(current->str, *(char **)element);
	current->index = (int)(*(array_t *)list).len;
}

// functie pentru testare care printeaza un indexed string
void print_indexed(void *elem) {
	char *my_string = ((indexed_string_t *)elem)->str;
	printf("%d - %s\n", ((indexed_string_t *)elem)->index, my_string);
}

// destructor pentru elemente de tip indexed string
void indexed_destructor(void *elem)
{
	free(((indexed_string_t *)elem)->str);
}

void crt_n(void *new_number, void **elems)
{
	number_t *num = (number_t *)new_number;
	num->string = malloc(201);

	// copiaza direct campurile de tip int din structura
	num->fractional_part = *(int *)elems[0];
	num->integer_part = *(int *)elems[1];

	// foloseste scrierea cu snprintf pt a completa campul de tip string
	snprintf(num->string, 201, "%d.%d", *(int *)elems[0], *(int *)elems[1]);
}

// destructor de elemente tip number t
void num_d(void *elem)
{
	free(((number_t *)elem)->string);
}

// destructor de stringuri care verifica daca acestea exista inainte
void string_destructor(void *elem)
{
	if (*(char **)elem)
		free(*(char **)elem);
}

// verifica daca un student are nota de trecere
boolean passing(void *elem) {
	return ((student_t *)elem)->grade >= 5;
}

// verifica daca un string are indexul par
boolean even_index(void *elem) {
	return ((int)((indexed_string_t *)elem)->index % 2 == 0);
}

// retine doar numele unui student
void first_name_basis(void *only_names, void *student_list) {
	*(char **)only_names = (char *)malloc(100);
	strcpy(*(char **)only_names, ((student_t *)student_list)->name);
}

// first_name_basis, dar aplicat pe indexed string (retine doar stringul)
void strings_only(void *only_strings, void *string_list) {
	*(char **)only_strings = (char *)malloc(100);
	strcpy(*(char **)only_strings, ((indexed_string_t *)string_list)->str);
}

// suma elementelor unei liste
void add(void *acc, void *elem)
{
	*(int *)acc += *(int *)elem;
}

void list_to_bool(void *result, void **elems) {
	// verifica daca suma elementelor unei liste este mai mare decat target-ul
	boolean *num = (boolean *)result;
	int target = *(int *)elems[1];
	int sum = 0;
	array_t *list = (array_t *)elems[0];
	reduce(add, &sum, *list);

	// salveaza rezultatul intr-o lista de tip boolean
	if (sum >= target)
		*num = true;
	else
		*num = false;
}

array_t reverse(array_t list) {
	// creaza o lista noua
	array_t *new_list = malloc(sizeof(array_t));
	new_list->len = 0;
	new_list->elem_size = list.elem_size;
	new_list->destructor = list.destructor;
	new_list->data = malloc(list.elem_size * list.len);

	// aplica functia de reverse
	reduce(get_reverse, (void *)new_list, list);
	array_t result = *new_list;

	// elibereaza rezultatul
	free(new_list);
	return result;
}

array_t create_number_array(array_t integer_part, array_t fractional_part) {
	// aplica direct functia de create_num
	int size = sizeof(number_t);
	array_t res;
	res = map_multiple(crt_n, size, num_d, 2, integer_part, fractional_part);
	return res;
}

array_t get_passing_students_names(array_t list) {
	// aplica filtrul passing, apoi functia de retinere doar stringuri
	array_t filtered = filter(passing, list);
	return map(first_name_basis, sizeof(char *), string_destructor, filtered);
}

array_t check_bigger_sum(array_t list_list, array_t int_list) {
	// aplica direct functia de list_to_bool
	int size = sizeof(boolean);
	return map_multiple(list_to_bool, size, NULL, 2, list_list, int_list);
}

array_t get_even_indexed_strings(array_t list) {
	// inversarea listei, deoarece va fi nevoie sa o inversam din nou
	array_t *new_list = malloc(sizeof(array_t));
	new_list->len = 0;
	new_list->elem_size = list.elem_size;
	new_list->destructor = list.destructor;
	new_list->data = malloc(list.elem_size * list.len);
	reduce(get_reverse, (void *)new_list, list);

	array_t *indexed_list = malloc(sizeof(array_t));
	indexed_list->len = 0;
	indexed_list->elem_size = sizeof(indexed_string_t);
	indexed_list->destructor = indexed_destructor;
	indexed_list->data = malloc(indexed_list->elem_size * list.len);

	// indexurile vor fi si ele inversate, asa ca adaptam programul in functie
	// de paritatea lungimii listei pentru ca acestia sa fie consecventi
	if (list.len % 2 == 1)
		reduce(get_reverse_indexed, (void *)indexed_list, *new_list);
	else
		reduce(get_reverse_indexed_even, (void *)indexed_list, *new_list);

	// printare pt debug
	// for_each(print_indexed, *indexed_list);

	// filtram indexurile pt a ramane doar cele pare
	array_t indexed_filtered = filter(even_index, *indexed_list);

	// eliberari de memorie
	for_each(list.destructor, list);
	free(list.data);
	free(new_list->data);
	free(new_list);
	free(indexed_list);

	// retinere doar stringuri din lista de indexed strings
	int ch_size = sizeof(char *);
	return map(strings_only, ch_size, string_destructor, indexed_filtered);
}

array_t generate_square_matrix(int n) {
	(void)n;
	return (array_t){0};
}
