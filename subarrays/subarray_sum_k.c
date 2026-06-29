/* Subarray Sum Equals K  --  prefix-sum + hashmap practice (HPR prep, Day 2)
 *
 * Given n integers (negatives allowed) and a target k, COUNT how many
 * contiguous subarrays sum to exactly k.
 *
 * INPUT (stdin):
 *   line 1:  n  k
 *   line 2:  a[0] a[1] ... a[n-1]
 *
 * OUTPUT (stdout):
 *   a single integer: the count of contiguous subarrays summing to k
 *
 * Constraints:
 *   1 <= n <= 1e5
 *   -1e4 <= a[i] <= 1e4      (so a prefix sum fits in a long long easily)
 *   -1e9 <= k <= 1e9
 *
 * Examples:
 *   "3 2 / 1 1 1"    -> 2     ([1,1] and [1,1])
 *   "3 3 / 1 2 3"    -> 2     ([1,2] and [3])
 *   "3 0 / 1 -1 0"   -> 3     ([1,-1], [0], [1,-1,0])
 *
 * Build:  cc -Wall -O2 -o subarray_sum_k subarray_sum_k.c
 * Run:    ./subarray_sum_k < test1.txt
 *
 * ------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

unsigned long long** level1;
unsigned long long* level2;
#define HASH_ENTRIES_PER_LEVEL 1<<16

void hash_setup(){
		level1 = calloc(HASH_ENTRIES_PER_LEVEL,sizeof(long long*));
}

void hash_write(int index_wrd, long long data){

		// look at index and and break into
		unsigned short idx1 = (index_wrd >> 16) & 0xffff;
		unsigned short idx2 = index_wrd  & 0xffff;


		unsigned long long* level2 = level1[idx1];

		if (level2 == 0){
				level2 = calloc(HASH_ENTRIES_PER_LEVEL,sizeof(long long));
				level1[idx1] = level2;

		}

		level2[idx2] = data;

}

long long hash_read(int index_wrd){

		unsigned short idx1 = (index_wrd >> 16) & 0xffff;
		unsigned short idx2 = index_wrd  & 0xffff;

		unsigned long long* level2 = level1[idx1];

		if (level2 == 0 ){
				return 0;
		}


		return level2[idx2];

}

void hash_free(){
		if(level1){
				for(int i=0;i<HASH_ENTRIES_PER_LEVEL;i++){
						unsigned long long* level2 = level1[i];
						if (level2){
								free(level2);
						}
				}
				free(level1);
		}
}


int main(void) {
    long long n, k;
	long long count = 0;
    if (scanf("%lld %lld", &n, &k) != 2) return 0;

	hash_setup();


	hash_write(0,1);


	int total = 0;

    for (long long i = 0; i < n; i++) {
        int x;
        scanf("%d", &x);
		total += x;

		count += hash_read(total - k);

		long long tmp = hash_read(total);
		tmp++;
		hash_write(total,tmp);
	}


	printf("%lld\n",count);

	hash_free();

    return 0;
}
