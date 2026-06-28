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
 * The input parsing below is just boilerplate so you can focus on the
 * algorithm. Everything you actually need to think about goes where the
 * TODO is. (Delete/rewrite any of this if you'd rather build it all yourself.)
 * Hint on scope only, not on method: you'll need some way to look up
 * "how many earlier prefix sums equal X" in ~O(1). C gives you no such
 * container for free.
 * ------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    long long n, k;
	int answer = 0;
    if (scanf("%lld %lld", &n, &k) != 2) return 0;


	//printf("n %lld, k %lld\n",n,k);

	// the input
    long long *a = malloc(n * sizeof *a);

	// to save time
	long long *sums = malloc((n+1) * sizeof *a);
	long long *seen = malloc((n+1) * sizeof *a);

	// the record of counts indexed by sum
	// this needs to be (n +1) * n


    if (!a) return 1;

	long long sum = 0;
	sums[0] = 0; // first prior sum zero
	seen[0] = 0;
    for (long long i = 0; i < n; i++){
			scanf("%lld", &a[i]);
			sum += a[i];
			sums[i+1] = sum;
			seen[i+i] = 0;
	}




    /* ============================================================
     * TODO: compute the count of contiguous subarrays summing to k.
     *       Print the single integer answer to stdout.
     * ============================================================ */

	for(int j=0;j<=n;j++){
			long long v = a[i];
	}


	printf("%d\n",answer);

    free(a);
	free(sums);
    return 0;
}
