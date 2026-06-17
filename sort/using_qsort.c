#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// examples of using qsort to sort stuff
//
// signature you MUST be able to write from memory:
//   void qsort(void *base, size_t nmemb, size_t size,
//              int (*compar)(const void *, const void *));
//
//   base   = pointer to start of array
//   nmemb  = number of elements
//   size   = size of ONE element (sizeof(arr[0]))
//   compar = returns <0 if a<b, 0 if equal, >0 if a>b  (ascending)
//
// The comparator ALWAYS receives `const void *` that point AT an element.
// So for an array of T, the void* points at a T. You cast to (const T *).

// ---- ints ------------------------------------------------------------
// GOTCHA: do NOT `return *x - *y;` — subtraction can overflow for large
// magnitudes (e.g. INT_MIN). Compare explicitly.
int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);   // -1, 0, or 1, overflow-safe
}

int cmp_int_desc(const void *a, const void *b) {
    return cmp_int(b, a);       // swap args to reverse order
}

// ---- strings (array of char *) --------------------------------------
// THE classic trip-up. The array element type is `char *`.
// So the void* points AT a `char *`. Cast to (char * const *) and
// dereference ONCE to get the char* that strcmp wants.
int cmp_str(const void *a, const void *b) {
    const char *x = *(const char * const *)a;
    const char *y = *(const char * const *)b;
    return strcmp(x, y);
}

// ---- struct by a field ----------------------------------------------
typedef struct {
    const char *name;
    int age;
} Person;

int cmp_person_age(const void *a, const void *b) {
    const Person *x = a;        // void* -> Person* is implicit in C
    const Person *y = b;
    return (x->age > y->age) - (x->age < y->age);
}

int main(void) {
    // ints
    int nums[] = {42, -7, 0, 1000000, 3, INT_MIN};
	printf("sizeof nums %lu",sizeof(nums[0]));

    int n = sizeof(nums) / sizeof(nums[0]);
    qsort(nums, n, sizeof(nums[0]), cmp_int);
    for (int i = 0; i < n; i++) printf("%d ", nums[i]);
    putchar('\n');

    // strings
    const char *words[] = {"banana", "apple", "cherry", "date"};
    int w = sizeof(words) / sizeof(words[0]);
    qsort(words, w, sizeof(words[0]), cmp_str);
    for (int i = 0; i < w; i++) printf("%s ", words[i]);
    putchar('\n');

    // structs
    Person people[] = {{"Alice", 30}, {"Bob", 25}, {"Carol", 35}};
    int p = sizeof(people) / sizeof(people[0]);
    qsort(people, p, sizeof(people[0]), cmp_person_age);
    for (int i = 0; i < p; i++) printf("%s(%d) ", people[i].name, people[i].age);
    putchar('\n');

    return 0;
}
