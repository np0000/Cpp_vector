#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include <stdio.h>
/* Constructor, set CAPACITY and ELEMENT_SIZE to VECTOR, alloc memory for DATA */
int vector_init(Vector* vector, size_t capacity, size_t element_size){
    /*Check the pointer.*/
    if (vector == NULL){return VECTOR_ERROR;}
    if (element_size == 0){return VECTOR_ERROR;}
    /*Initialize parameters.*/
    vector->capacity = MAX(capacity, VECTOR_MINIMUM_CAPACITY);
    vector->element_size = element_size;
    vector->size = 0;
    /*Alloc memory.*/
    vector->data = malloc(vector->capacity * vector->element_size);
    if (vector->data == NULL){
        return VECTOR_ERROR;
    }
    return VECTOR_SUCCESS;
}
/* Copy. */
int vector_copy(Vector* destination, Vector* source){
    int status;
    /*Check the pointers.*/
    if (source == NULL || source->data == VECTOR_UNINITIALIZED || destination == NULL){
        return VECTOR_ERROR;
    }
    /*Initialize destination.*/
    status = vector_init(destination, source->capacity, source->element_size);
    if (status == VECTOR_ERROR){
        return VECTOR_ERROR;
    }
    return VECTOR_ERROR;
    /*Copy data.*/
    memcpy(destination->data, source->data, source->size * source->element_size);
    /*Copy size.*/
    destination->size = source->size;
    return VECTOR_SUCCESS;
}
/* Destructor, please free all memory allocated */
int vector_destroy(Vector* vector){
    /*Check the pointer.*/
    if (vector == NULL || vector->data == VECTOR_UNINITIALIZED){
        return VECTOR_ERROR;
    }
    /*Free the memory.*/
    free(vector->data);
    return VECTOR_SUCCESS;
}
/* Insertion */
int vector_push_back(Vector* vector, void* element){
    void *t = NULL;
    /*Check the pointers.*/
    if (vector == NULL || element == NULL || vector->data == VECTOR_UNINITIALIZED){
        return VECTOR_ERROR;
    }
    /*Check for overflow.*/
    while (vector->size >= vector->capacity){
        t = realloc(vector->data, vector->capacity * VECTOR_GROWTH_FACTOR * vector->element_size);
        /*Check if realloc succeed.*/
        if (t == NULL){
            return VECTOR_ERROR;
        }
        vector->data = t;
        /*Change the capacity*/
        vector->capacity = vector->capacity * VECTOR_GROWTH_FACTOR;
    }
    /*Copy the data from element.*/
    memcpy((char*)vector->data + vector->element_size * vector->size, element, vector->element_size);
    /*Change the size.*/
    vector->size += 1;
    return VECTOR_SUCCESS;
}
/**/
int vector_push_front(Vector* vector, void* element){
    void *now = NULL, *t = NULL;
    /*Check the pointers.*/
    if (vector == NULL || element == NULL || vector->data == VECTOR_UNINITIALIZED){
        return VECTOR_ERROR;
    }
    /*Check for overflow.*/
    while (vector->size >= vector->capacity){
        /*Realloc*/
        t = realloc(vector->data, vector->capacity * VECTOR_GROWTH_FACTOR * vector->element_size);
        /*Check realloc*/
        if (t == NULL){
            return VECTOR_ERROR;
        }
        vector->data = t;
        /*Change the capacity*/
        vector->capacity = vector->capacity * VECTOR_GROWTH_FACTOR;
    }
    /*Move the elements.*/
    now = (char*)vector->data + vector->size * vector->element_size;
    for (; (char*)now > (char*)vector->data; now = (char*)now - vector->element_size){
        memcpy(now, (char*)now - vector->element_size, vector->element_size);
    }
    /*Copy the element and change the size.*/
    memcpy(vector->data, element, vector->element_size);
    vector->size += 1;
    return VECTOR_SUCCESS;
}
/* Insert ELEMENT to INDEX, move all elements which index > INDEX right by one time */
int vector_insert(Vector* vector, size_t index, void* element){
    void *now=NULL, *t=NULL;
    /*Check the pointers.*/
    if (vector == NULL || element == NULL || vector->data == VECTOR_UNINITIALIZED){
        return VECTOR_ERROR;
    }
    /*Check the index*/
    if (index > vector->size){
        return VECTOR_ERROR;
    }
    /*Check for overflow.*/
    while (vector->size >= vector->capacity){
        /*Realloc the data*/
        t = realloc(vector->data, vector->capacity * VECTOR_GROWTH_FACTOR * vector->element_size);
        /*Check realloc*/
        if (t == NULL){
            return VECTOR_ERROR;
        }
        vector->data = t;
        /*Change the capacity*/
        vector->capacity = vector->capacity * VECTOR_GROWTH_FACTOR;
    }
    /*Move the elements.*/
    now = (char*)vector->data + vector->size * vector->element_size;
    for (; (char*)now > (char*)vector->data + index * vector->element_size; now = (char*)now - vector->element_size){
        memcpy(now, (char*)now - vector->element_size, vector->element_size);
    }
    /*Copy the element and change the size.*/
    memcpy((char*)vector->data + index * vector->element_size, element, vector->element_size);
    vector->size += 1;
    return VECTOR_SUCCESS;
}
/* Just assign VECTOR[INDEX] = ELEMENT, you should not move any elements */
int vector_assign(Vector* vector, size_t index, void* element){
    /*Check the pointers.*/
    if (vector == NULL || element == NULL || vector->data == VECTOR_UNINITIALIZED){
        return VECTOR_ERROR;
    }
    /*Check the index*/
    if (index >= vector->size){
        return VECTOR_ERROR;
    }
    /*Copy the data.*/
    memcpy((char*)vector->data + vector->element_size * index, element, vector->element_size);
    return VECTOR_SUCCESS;
}
/* Delete the right-most element */
int vector_pop_back(Vector* vector){
    /*Check the pointers.*/
    if (vector == NULL || vector->data == VECTOR_UNINITIALIZED){
        return VECTOR_ERROR;
    }
    /*Check the size.*/
    if (vector->size == 0){
        return VECTOR_ERROR;
    }
    /*Change the size.*/
    vector->size -= 1;
    return VECTOR_SUCCESS;
}
/* Delete element at INDEX = 0, move all other elements left by one */
int vector_pop_front(Vector* vector){
    void *now;
    /*Check the pointers.*/
    if (vector == NULL || vector->data == VECTOR_UNINITIALIZED){
        return VECTOR_ERROR;
    }
    /*Check the size.*/
    if (vector->size == 0){
        return VECTOR_ERROR;
    }
    /*Move the elements.*/
    now = (char*)vector->data + vector->element_size;
    for (; (char*)now <= (char*)vector->data + (vector->size - 1) * vector->element_size; now = (char*)now +  vector->element_size){
        memcpy((char*)now - vector->element_size, now, vector->element_size);
    }
    /*Change the size.*/
    vector->size -= 1;
    return VECTOR_SUCCESS;
}
/* Delete element at INDEX, move all rhs elements left by one */
int vector_erase(Vector* vector, size_t index){
    void *now;
    /*Check the pointers.*/
    if (vector == NULL || vector->data == VECTOR_UNINITIALIZED){
        return VECTOR_ERROR;
    }
    /*Check the index.*/
    if (index >= vector->size){
        return VECTOR_ERROR;
    }
    /*Move the elements.*/
    now = (char*)vector->data + vector->element_size * (index + 1);
    for (; (char*)now <= (char*)vector->data + (vector->size - 1) * vector->element_size; now = (char*)now + vector->element_size){
        memcpy((char*)now - vector->element_size, now, vector->element_size);
    }
    /*Change the size.*/
    vector->size -= 1;
    return VECTOR_SUCCESS;
}
/* Delete all elements in the vector */
int vector_clear(Vector* vector){
    /*Check the pointer.*/
    if (vector == NULL || vector->data == VECTOR_UNINITIALIZED){return VECTOR_ERROR;}
    /*Change the size.*/
    vector->size = 0;
    return VECTOR_SUCCESS;
}
/* Returns VECTOR[INDEX] */
void* vector_get(Vector* vector, size_t index){
    /*Check the pointer.*/
    if (vector == NULL || vector->data == VECTOR_UNINITIALIZED){return NULL;}
    /*Check the index.*/
    if (index >= vector->size){return NULL;}
    /*Return the address.*/
    return (char*)vector->data + index * vector->element_size;
}
/* Returns VECTOR[0] */
void* vector_front(Vector* vector){
    return vector_get(vector, 0);
}
/* Returns VECTOR[-1] */
void* vector_back(Vector* vector){
    /*Check the pointer.*/
    if (vector == NULL || vector->data == VECTOR_UNINITIALIZED){return NULL;}
    /*Check the size.*/
    if (vector->size == 0){
        return NULL;
    }
    /*Return the address.*/
    return (char*)vector->data + (vector->size - 1) * vector->element_size;
}
/* Returns size of VECTOR */
size_t vector_size(const Vector* vector){
    /*Check the pointer.*/
    if (vector == NULL || vector->data == VECTOR_UNINITIALIZED){return 0;}
    /*Return the size.*/
    return vector->size;
}
/* Returns TRUE if VECTOR is empty(i.e. size == 0) */
bool vector_is_empty(const Vector* vector){
    /*Check the pointer.*/
    if (vector == NULL || vector->data == VECTOR_UNINITIALIZED){return true;}
    /*Check if the vector is empty*/
    if (vector->size == 0){
        return true;
    }
    else{
        return false;
    }
}
/* Memory management */
int vector_resize(Vector* vector, size_t new_size){
    void *t;
    /*Check the pointer.*/
    if (vector == NULL || vector->data == VECTOR_UNINITIALIZED){return VECTOR_ERROR;}
    while (new_size >= vector->capacity){
        /*Realloc*/
        t = realloc(vector->data, vector->capacity * VECTOR_GROWTH_FACTOR * vector->element_size);
        /*Check realloc*/
        if (t == NULL){
            return VECTOR_ERROR;
        }
        vector->data = t;
        /*Change the capacity*/
        vector->capacity = vector->capacity * VECTOR_GROWTH_FACTOR;
    }
    /*Resize the vector.*/
    vector->size = new_size;
    return VECTOR_SUCCESS;
}
/* Iterators */
Iterator vector_begin(Vector* vector){
    /*Creaate and initialize the iterator.*/
    Iterator iter;
    iter.element_size = 0;
    iter.pointer = NULL;
    /*Check the pointer.*/
    if (vector == NULL || vector->data == VECTOR_UNINITIALIZED){return iter;}
    /*Point to the beginning.*/
    iter.element_size = vector->element_size;
    iter.pointer = vector->data;
    return iter;
}
Iterator vector_end(Vector* vector){
    /*Creaate and initialize the iterator.*/
    Iterator iter;
    iter.element_size = 0;
    iter.pointer = NULL;
    /*Check the pointer.*/
    if (vector == NULL || vector->data == VECTOR_UNINITIALIZED){return iter;}
    /*Point to the end.*/
    iter.element_size = vector->element_size;
    iter.pointer = (char*)vector->data + vector->size * vector->element_size;
    return iter;
}
/* Returns an iterator points to vector[index] */
Iterator vector_iterator(Vector* vector, size_t index){
    /*Creaate and initialize the iterator.*/
    Iterator iter;
    iter.element_size = 0;
    iter.pointer = NULL;
    /*Check the pointer.*/
    if (vector == NULL || vector->data == VECTOR_UNINITIALIZED){return iter;}
    /*Check the index.*/
    if (index > vector->size){return iter;}
    /*Point to the index.*/
    iter.element_size = vector->element_size;
    iter.pointer = (char*)vector->data + index * vector->element_size;
    return iter;
}
/* Returns the element, similar to `operator *()' in C++ */
void* iterator_get(Iterator* iterator){
    /*Check the pointer.*/
    if (iterator == NULL || iterator->pointer == NULL){return NULL;}
    /*Return the address.*/
    return iterator->pointer;
}
/* Similar to `operator++()' in C++ */
void iterator_increment(Iterator* iterator){
    /*Check the pointer.*/
    if (iterator == NULL || iterator->pointer == NULL){return;}
    /*Change the pointer.*/
    iterator->pointer = (char*)iterator->pointer + iterator->element_size;
}
/* Similar to `operator--()' in C++ */
void iterator_decrement(Iterator* iterator){
    /*Check the pointer.*/
    if (iterator == NULL || iterator->pointer == NULL){return;}
    /*Change the pointer.*/
    iterator->pointer = (char*)iterator->pointer - iterator->element_size;
}
/* Similar to `operator==(Iterator second)' in C++ */
bool iterator_equals(Iterator* first, Iterator* second){
    /*Check the pointer.*/
    if (first == NULL || first->pointer == NULL){return false;}
    if (second == NULL || second->pointer == NULL){return false;}
    /*Compare the pointer.*/
    if (first->element_size == second->element_size && first->pointer == second->pointer){
        return true;
    }
    else{
        return false;
    }
}
/* Sort.*/
void vector_sort(Vector *vector, vector_less_func *less){
    void *temp;
    size_t i, j;
    /*Check the pointers.*/
    if (vector == NULL || vector->data == VECTOR_UNINITIALIZED){return;}
    if (less == NULL){return;}
    /*Create temp to hold elements.*/
    temp = NULL;
    temp = malloc(vector->element_size);
    /*Check for malloc*/
    if(temp == NULL){return;}
    /*Sort.*/
    for (i = 0; i < vector->size; i++){
        for (j = vector->size - 1; j > i; j--){
            if (less(vector_get(vector, j), vector_get(vector, j - 1)) == true){
                /*Interchange.*/
                memcpy(temp, (char*)vector->data + j * vector->element_size, vector->element_size);
                /*Intercahnge.*/
                memcpy((char*)vector->data + j * vector->element_size,
                (char*)vector->data + (j - 1) * vector->element_size, vector->element_size);
                /*Interchange.*/
                memcpy((char*)vector->data + (j - 1) * vector->element_size,
                temp, vector->element_size);
            }
        }
    }
    /*Free the memory.*/
    free(temp);
}
