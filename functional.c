#include "functional.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

void for_each(void (*func)(void *), array_t list)
{
	// aplica functia pentru fiecare element din lista
	for (int i = 0; i < list.len; i++)
		func((void *)((char *)list.data + i * list.elem_size));
}

array_t map(void (*func)(void *, void *),
			int new_list_elem_size,
			void (*new_list_destructor)(void *),
			array_t list)
{
	// creaza o lista noua cu atributele specificate
	array_t new_list;
	new_list.destructor = new_list_destructor;
	new_list.elem_size = new_list_elem_size;
	new_list.len = list.len;
	new_list.data = malloc(new_list.len * new_list_elem_size);
	// aplica functia pe elementul listei noi
	for (int i = 0; i < list.len; i++) {
		char *new_elem = (char *)new_list.data + i * new_list.elem_size;
		func(new_elem, (char *)list.data + i * list.elem_size);
		// elibereaza elementul listei vechi
		if (list.destructor && (char *)list.data + i * list.elem_size)
			list.destructor((char *)list.data + i * list.elem_size);
	}
	free(list.data);
	return new_list;
}

array_t filter(boolean(*func)(void *), array_t list)
{
	array_t new_list;
	int new_list_counter = 0;
	// creaza lista noua si ii afla lungimea necesara
	for (int i = 0; i < list.len; i++)
		if (func((char *)list.data + i * list.elem_size))
			new_list_counter++;
	new_list.len = new_list_counter;
	new_list.destructor = list.destructor;
	new_list.elem_size = list.elem_size;
	new_list.data = malloc(new_list.len * new_list.elem_size);

	new_list_counter = 0;
	// copiaza elementele in lista noua daca functia booleana returneaza 1
	for (int i = 0; i < list.len; i++) {
		if (func((char *)list.data + i * list.elem_size)) {
			char *source = (char *)list.data + i * list.elem_size;
			int offset = new_list_counter * list.elem_size;
			char *dest = (char *)new_list.data + offset;
			memcpy(dest, source, list.elem_size);
			new_list_counter++;
		} else if (list.destructor && (char *)list.data + i * list.elem_size)
			// in caz contrar elibereaza elementul listei vechi
				list.destructor((char *)list.data + i * list.elem_size);
	}
	free(list.data);
	return new_list;
}

void *reduce(void (*func)(void *, void *), void *acc, array_t list)
{
	// aplica functia pe fiecare element al listei
	for (int i = 0; i < list.len; i++)
		func(acc, ((char *)list.data + i * list.elem_size));
	return acc;
}

void for_each_multiple(void(*func)(void **), int varg_c, ...)
{
	va_list args;
	va_start(args, varg_c);
	array_t *collection = malloc(varg_c * sizeof(array_t));
	collection[0] = va_arg(args, array_t);
	int smallest = collection[0].len;
	int biggest_size = collection[0].elem_size;

	for (int i = 1; i < varg_c; i++) {
		// salveaza listele intr-o colectie
		collection[i] = va_arg(args, array_t);
		// gaseste cea mai mica lungime a unei liste
		if (collection[i].len < smallest)
			smallest = collection[i].len;

		// gaseste cea mai mare dimensiune posibila a unui element
		if (collection[i].elem_size > biggest_size)
			biggest_size = collection[i].elem_size;
	}

	// creaza vectorul de elemente de la un anumit index
	for (int j = 0; j < smallest; j++) {
		char **vector = (char **)malloc(varg_c * sizeof(char *));
		for (int k = 0; k < varg_c; k++)
			vector[k] = (char *)malloc(biggest_size);
		for (int i = 0; i < varg_c; i++) {
			int original_pos = j * collection[i].elem_size;
			// copiaza in vector numarul necesar de elemente din colectie
			char *source = (char *)(collection[i].data + original_pos);
			memcpy(*(vector + i), source, collection[i].elem_size);
		}
		// aplica functia pe vector
		func((void **)vector);
		for (int k = 0; k < varg_c; k++)
			free(vector[k]);
		free(vector);
	}
	va_end(args);
	free(collection);
}

array_t map_multiple(void (*func)(void *, void **),
					 int new_list_elem_size,
					 void (*new_list_destructor)(void *),
					 int varg_c, ...)
{
	va_list args;
	va_start(args, varg_c);
	array_t *collection = malloc(varg_c * sizeof(array_t));
	collection[0] = va_arg(args, array_t);
	int smallest = collection[0].len;
	int biggest_size = collection[0].elem_size;

	for (int i = 1; i < varg_c; i++) {
		// salveaza listele intr-o colectie
		collection[i] = va_arg(args, array_t);
		// gaseste cea mai mica lungime a unei liste
		if (collection[i].len < smallest)
			smallest = collection[i].len;

		// gaseste cea mai mare dimensiune posibila a unui element
		if (collection[i].elem_size > biggest_size)
			biggest_size = collection[i].elem_size;
	}

	// creaza noua lista
	array_t new_array;
	new_array.destructor = new_list_destructor;
	new_array.elem_size = new_list_elem_size;
	new_array.len = smallest;
	new_array.data = malloc(smallest * new_list_elem_size);
	int counter = 0;

	for (int j = 0; j < smallest; j++) {
		// alocarea vectorului temporar
		char **vector = (char **)malloc(varg_c * sizeof(char *));
		for (int k = 0; k < varg_c; k++)
			vector[k] = (char *)malloc(biggest_size);

		// popularea vectorului temporar
		for (int i = 0; i < varg_c; i++) {
			int original_pos = j * collection[i].elem_size;
			char *source = (char *)(collection[i].data + original_pos);
			memcpy(*(vector + i), source, collection[i].elem_size);
		}

		// aplicarea functiei pe vectorul temporar
		func(new_array.data + counter * new_list_elem_size, (void **)vector);
		counter++;

		// eliberarea vectorului temporar
		for (int k = 0; k < varg_c; k++)
			free(vector[k]);
		free(vector);
	}

	for (int j = 0; j < smallest; j++) {
		for (int i = 0; i < varg_c; i++) {
			// elibereaza elementele din colectie
			int original_pos = j * collection[i].elem_size;
			char *elem = (char *)(collection[i].data + original_pos);
			if (collection[i].destructor && elem)
				collection[i].destructor(elem);
		}
	}

	// elibereaza restul colectiei
	va_end(args);
	for (int i = 0; i < varg_c; i++)
		free(collection[i].data);
	free(collection);

	return new_array;
}

void *reduce_multiple(void(*func)(void *, void **), void *acc, int varg_c, ...)
{
	// creaza colectia
	va_list args;
	va_start(args, varg_c);
	array_t *collection = malloc(varg_c * sizeof(array_t));
	collection[0] = va_arg(args, array_t);
	int smallest = collection[0].len;
	int biggest_size = collection[0].elem_size;

	for (int i = 1; i < varg_c; i++) {
		// salveaza listele date ca parametru in colectie
		collection[i] = va_arg(args, array_t);
		// gaseste lungimea cele mai scurte liste
		if (collection[i].len < smallest)
			smallest = collection[i].len;

		// gaseste dimensiunea celui mai mare element din liste
		if (collection[i].elem_size > biggest_size)
			biggest_size = collection[i].elem_size;
	}

	//creaza vectorul temporar
	for (int j = 0; j < smallest; j++) {
		char **vector = (char **)malloc(varg_c * sizeof(char *));
		for (int k = 0; k < varg_c; k++)
			vector[k] = (char *)malloc(biggest_size);
		for (int i = 0; i < varg_c; i++) {
			int original_pos = j * collection[i].elem_size;
			// copiaza in vectorul temporar elementele necesare din colectie
			char *source = (char *)(collection[i].data + original_pos);
			memcpy(*(vector + i), source, collection[i].elem_size);
		}
		// aplica functia
		func(acc, (void **)vector);

		// elibereeaza vectorul temporar
		for (int k = 0; k < varg_c; k++)
			free(vector[k]);
		free(vector);
	}
	va_end(args);
	// elibereaza colectia
	free(collection);
	return acc;
}
