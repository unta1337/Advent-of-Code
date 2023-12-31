#ifndef __LIST_H
#define __LIST_H

#include <stdio.h>
#include <stdbool.h>

/**
 * 메모리 조작 관련 헤더 및 동적할당된 메모리의 크기를 추적하기 위한 유틸리티 모음
 */

// Ref: https://stackoverflow.com/questions/8259817/how-to-track-malloc-and-free

#ifndef __MEMORY_H
#define __MEMORY_H

#include <stdlib.h>
#include <string.h>

/**
 * 현재까지 동적할당된 메모리의 총량
 */
size_t used_malloc = 0;

/**
 * @brief 메모리 사용량을 추적하는 malloc
 * @param size 할당 받을 메모리의 크기
 * @return 동적할당된 포인터
 */
void* malloc_s(size_t size)
{
    void* buffer = malloc(size + sizeof(size_t));
    if (buffer == NULL)
        return NULL;

    used_malloc += size;

    size_t* size_box = (size_t*)buffer;
    *size_box = size;

    return (char*)buffer + sizeof(size_t);
}

/**
 * @brief 메모리 사용량을 추적하는 calloc
 * @param n 할당 받을 요소의 개수
 * @param of_size 할당 받을 단일 요소의 크기
 * @return 동적할당된 포인터
 */
void* calloc_s(size_t n, size_t of_size)
{
    void* buffer = malloc(n * of_size + sizeof(size_t));
    if (buffer == NULL)
        return NULL;

    memset(buffer, 0, n * of_size + sizeof(size_t));

    used_malloc += n * of_size;

    size_t* size_box = (size_t*)buffer;
    *size_box = n * of_size;

    return (char*)buffer + sizeof(size_t);
}

/**
 * @brief 메모리 사용량을 추적하는 realloc
 * @param ptr 재할당 받을 포인터
 * @param size 할당 받을 메모리의 크기
 * @return 동적할당된 포인터
 */
void* realloc_s(void* ptr, size_t size)
{
    void* buffer = (char*)ptr - sizeof(size_t);
    size_t* size_box = (size_t*)buffer;

    used_malloc -= *size_box;

    buffer = realloc(buffer, size + sizeof(size_t));
    if (buffer == NULL)
        return NULL;

    size_box = (size_t*)buffer;

    used_malloc += size;
    *size_box = size;

    return (char*)buffer + sizeof(size_t);
}

/**
 * @brief 메모리 사용량을 추적하는 free
 * @param ptr 반납할 포인터
 */
void free_s(void* ptr)
{
    if (ptr == NULL)
        return;

    void* buffer = (char*)ptr - sizeof(size_t);
    size_t* size_box = (size_t*)buffer;

    used_malloc -= *size_box;

    free(buffer);
}

#endif

/**
 * 배열을 기반으로 구현된 가변 길이 리스트
 */
struct list
{
    /**
     * 실제 데이터를 저장할 공간에 대한 포인터
     */
    void* arr;
    /**
     * 리스트의 한계 용량
     */
    size_t capacity;
    /**
     * 리스트의 길이
     */
    size_t size;
    /**
     * 리스트 단일 요소의 크기
     */
    size_t of_size;
};

/**
 * *내부 함수
 *
 * @brief 리스트 크기 2배 증가
 * @param ths 대상 리스트 포인터
 */
void __list_double(struct list* ths)
{
    ths->capacity *= 2;
    ths->arr = realloc_s(ths->arr, ths->capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for list in __list_double().\n");
        abort();
    }
}

/**
 * *내부 함수
 *
 * @brief 리스트 크기 2배 감소
 * @param ths 대상 리스트 포인터
 */
void __list_half(struct list* ths)
{
    if (ths->size == 0)
        return;

    ths->capacity /= 2;
    ths->arr = realloc_s(ths->arr, ths->capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for list in __list_half().\n");
        abort();
    }
}

/**
 * *내부 함수
 *
 * @brief 리스트의 크기를 2의 제곱수로 교정
 * @param ths 대상 리스트 포인터
 */
void __list_capacity_correction(struct list* ths)
{
    size_t correct_capacity = 1;

    while (correct_capacity <= ths->size)
        correct_capacity *= 2;

    ths->arr = realloc_s(ths->arr, correct_capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for stack in __list_capacity_correction().\n");
        abort();
    }

    ths->capacity = correct_capacity;
}

/**
 * @brief 새로운 리스트 생성
 * @param of_size 리스트에 저장할 단일 요소의 크기
 * @return 동적으로 생성된 리스트의 주소
 */
struct list* list_create(size_t of_size)
{
    struct list* ths = (struct list*)malloc_s(sizeof(struct list));

    ths->arr = malloc_s(of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for list in list_create().\n");
        abort();
    }

    ths->capacity = 1;
    ths->size = 0;
    ths->of_size = of_size;

    return ths;
}

/**
 * @brief 배열로부터 새로운 리스트 생성
 * @param arr 리스트로 생성할 배열의 포인터
 * @param size 리스트로 생성할 배열의 길이
 * @param of_size 리스트로 생성할 배열의 단일 요소의 크기
 * @return 동적으로 생성된 리스트의 주소
 */
struct list* list_create_from_array(void* arr, size_t size, size_t of_size)
{
    if (arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to initialize list since the original array is NULL.\n");
        abort();
    }
    else if (of_size == 0)
    {
        fprintf(stderr, "stderr: Size of a single element of list cannot be zero.");
        abort();
    }

    struct list* ths = (struct list*)malloc_s(sizeof(struct list));

    ths->arr = malloc_s(size * of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for list in list_create_from_array().\n");
        abort();
    }

    ths->capacity = size;
    ths->size = size;
    ths->of_size = of_size;

    memcpy(ths->arr, arr, size * of_size);
    __list_capacity_correction(ths);

    return ths;
}

/**
 * @brief 기본값을 설정하여 새로운 리스트 생성
 * @param value 리스트의 기본값으로 설정할 값의 포인터
 * @param size 생성할 리스트의 길이
 * @param of_size 리스트의 기본값으로 설정할 값의 크기
 * @return 동적으로 생성된 리스트의 주소
 */
struct list* list_create_from_value(void* value, size_t size, size_t of_size)
{
    if (value == NULL)
    {
        fprintf(stderr, "stderr: Failed to initialize list since the original array is NULL.\n");
        abort();
    }
    else if (of_size == 0)
    {
        fprintf(stderr, "stderr: Size of a single element of list cannot be zero.");
        abort();
    }

    struct list* ths = (struct list*)malloc_s(sizeof(struct list));
    ths->arr = malloc_s(size * of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for list in list_create_from_array().\n");
        abort();
    }

    ths->capacity = size;
    ths->size = size;
    ths->of_size = of_size;

    for (size_t i = 0; i < size; i++)
        memcpy((char*)ths->arr + i * of_size, value, of_size);
    __list_capacity_correction(ths);
    
    return ths;
}

/**
 * @brief 리스트 삭제
 * @param ths 대상 리스트 포인터
 */
void list_delete(struct list* ths)
{
    free_s(ths->arr);
    free_s(ths);
}

/**
 * @brief 리스트가 비었는지 여부 반환
 * @param ths 대상 리스트 포인터
 * @return 리스트 빔 여부
 */
bool list_empty(struct list* ths)
{
    return ths->size == 0;
}

/**
 * 매개변수 value는 삽입할 대상을 가리키는 포인터 또는 주소여야 한다.
 * int a를 삽입하고 싶다면 a에 대한 주소인 &a나 int* p = &a에서 p를 넘기면 된다.
 *
 * -> int a가 저장된 메모리상의 위치를 0x10라 하자.
 *    원하는 결과는 0x10에 저장된 값을 삽입하는 것이므로 이를 가리키는 포인터, 즉 a의 주솟값을 넘겨야 한다.
 *
 * int a를 가리키는 포인터 int* p를 삽입하고 싶다면 p의 주소나 int** q = &p에서 q를 넘기면 된다.
 *
 * -> int a가 저장된 메모리상의 위치를 0x10, int* p가 저장된 위치를 0x20이라 하자.
 *    원하는 결과는 a의 주소이므로 0x10을 삽입해야 한다.
 *    이를 저장하고 있는 주소는 p의 주소인 0x20이므로 &p 또는 int** q = &p의 q를 넘기면 된다.
 *
 * 단, 배열의 포인터를 삽입할 때는 배열을 이름으로 참조한 주솟값과 & 연산자로 참조한 주솟값이 동일하다.
 * 따라서 배열을 가리키는 포인터의 주소를 넘겨야 한다.
 *
 * -> 배열을 복사하여 삽입하는 경우.
 * -> int arr[3]가 저장된 메모리상의 위치를 0x10이라 하자.
 *    원하는 결과는 0x10로부터 3 * sizeof(int)만큼을 삽입하는 것이므로 배열의 이름을 그대로 넘기면 된다.
 *
 * -> 배열의 참조를 삽입하는 경우.
 * -> int arr[3]가 저장된 메모리상의 위치를 0x10이라 하자.
 *    원하는 결과는 주솟값이 0x10을 삽입하는 것이다.
 *    그런데 배열은 arr과 &arr이 동일한 값을 가지므로 arr을 가리키는 별도의 포인터를 이용해야 한다.
 *    int* p = arr로 p가 arr의 주소를 가리키도록 하자.
 *    p는 배열이 아닌 포인터이므로 &p를 통해 0x10를 가리키는 주솟값을 얻을 수 있다.
 *    따라서 &p를 넘기면 된다.
 *
 * 참고, int** 등의 이중 포인터 대신 void*를 매개변수로 넘겨도 된다.
 * int** q = &p 대신 void* q = &p.
 *
 * @brief 리스트에 새로운 요소 삽입
 * @param ths 대상 리스트 포인터
 * @param value 삽입할 대상을 가리키는 포인터
 */
void list_push(struct list* ths, void* value)
{
    if (ths->size == ths->capacity)
        __list_double(ths);

    memcpy((char*)ths->arr + ths->size++ * ths->of_size, value, ths->of_size);
}

/**
 * @brief 리스트의 마지막 요소를 삭제
 * @param ths 대상 리스트 포인터
 */
void list_pop(struct list* ths)
{
    if (list_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to pop an element from list because the list is empty.\n");
        abort();
    }

    ths->size--;

    if (ths->size == ths->capacity / 2)
        __list_half(ths);
}

/**
 * dest는 요소를 복사할 수 있을 만큼의 공간을 가리키는 포인터 또는 주소여야 한다.
 * dest는 삽입한 요소를 담을 수 있는 적절한 포인터 자료형이거나 void*여야 한다.
 *
 * -> 단일 요소의 크기가 2 * sizeof(int), 즉 배열을 삽입한 경우 아래와 같이 이용할 수 있다.
 *    int temp[2];
 *    list_pop(lst, temp);
 *
 *    또는,
 *    int* temp_v = malloc_s(2 * sizeof(int));
 *    list_pop(lst, temp_v);
 *
 * @brief 리스트의 특정 요소를 dest에 복사
 * @param ths 대상 리스트 포인터
 * @param dest 요소를 복사할 목적지
 * @param index 복사할 대상의 인덱스
 * @return 복사 성공 여부
 */
void list_get(struct list* ths, void* dest, size_t index)
{
    if (index >= ths->size)
    {
        fprintf(stderr, "stderr: List index out of range. Expected less then %zu but found %zu.\n", ths->size, index);
        abort();
    }

    memcpy(dest, (char*)ths->arr + index * ths->of_size, ths->of_size);
}

/**
 * @brief 리스트의 특정 요소 value로 변경
 * @param ths 대상 리스트 포인터
 * @param index 변경할 대상의 인덱스
 * @param value 새로 삽입할 요소를 가리키는 포인터
 */
void list_set(struct list* ths, size_t index, void* value)
{
    if (index >= ths->size)
    {
        fprintf(stderr, "stderr: List index out of range. Expected less then %zu but found %zu.\n", ths->size, index);
        abort();
    }

    memcpy((char*)ths->arr + index * ths->of_size, value, ths->of_size);
}

/**
 * @brief 리스트의 특정 위치에 요소 추가
 * @param ths 대상 리스트 포인터
 * @param index 새로운 요소를 추가할 위치
 * @param value 새로 삽입할 요소를 가리키는 포인터
 */
void list_insert(struct list* ths, size_t index, void* value)
{
    if (index >= ths->size + 1)
    {
        fprintf(stderr, "stderr: List index out of range. Expected less then %zu but found %zu.\n", ths->size, index);
        abort();
    }

    void* temp = malloc_s(ths->of_size);
    list_push(ths, temp);
    free_s(temp);

    memmove((char*)ths->arr + (index + 1) * ths->of_size, (char*)ths->arr + index * ths->of_size, (ths->size - index - 1) * ths->of_size);
    memcpy((char*)ths->arr + index * ths->of_size, value, ths->of_size);
}

/**
 * @brief 리스트의 특정 위치의 요소 삭제
 * @param ths 대상 리스트 포인터
 * @param index 삭제할 요소의 인덱스
 */
void list_remove(struct list* ths, size_t index)
{
    if (index >= ths->size)
    {
        fprintf(stderr, "stderr: List index out of range. Expected less then %zu but found %zu.\n", ths->size, index);
        abort();
    }

    memmove((char*)ths->arr + index * ths->of_size, (char*)ths->arr + (index + 1) * ths->of_size, (ths->size - index) * ths->of_size);
    list_pop(ths);
}

/**
 * @brief 리스트 초기화
 * @param ths 대상 리스트 포인터
 */
void list_clear(struct list* ths)
{
    ths->arr = realloc_s(ths->arr, ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for list in list_clear().\n");
        abort();
    }

    ths->capacity = 1;
    ths->size = 0;
}

/**
 * @brief 리스트 정렬
 * @param ths 대상 리스트 포인터
 * @param comp 정렬 기준을 정의하는 함수 (qsort의 comp와 동일)
 */
void list_sort(struct list* ths, int (*comp)(const void*, const void*))
{
    qsort(ths->arr, ths->size, ths->of_size, comp);
}

#endif
