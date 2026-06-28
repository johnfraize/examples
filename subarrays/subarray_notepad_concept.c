/* subarray_notepad_concept.c  --  the "notepad" idea, simplest form (HPR prep, Day 2)
 *
 * PROBLEM: given n integers and a target K, are there TWO numbers
 * (at two different positions) that add up to K?  Print YES or NO.
 *
 * This is the warm-up that cements the concept: instead of scanning backward
 * for each number's partner (the nested loop, O(n^2)), you walk ONCE and keep
 * a NOTEPAD of numbers you've already passed. For each number you just glance
 * at the notepad instead of looping.
 *
 *     see x  ->  partner I need is (K - x).  Is it on the notepad?  if yes -> found.
 *                then jot x onto the notepad.
 *
 * INPUT (stdin):
 *   line 1:  n  K
 *   line 2:  a[0] a[1] ... a[n-1]
 *
 * OUTPUT (stdout):
 *   YES  if some pair sums to K, else  NO
 *
 * Constraints (kept small so the notepad can be a plain array):
 *   1 <= n <= 1000
 *   0 <= a[i] <= 1000
 *   0 <= K <= 2000
 *
 * Examples:
 *   "4 10 / 2 7 4 6"  -> YES   (4 + 6)
 *   "4 20 / 2 7 4 6"  -> NO
 *   "2 8  / 4 4"      -> YES   (the two 4s, different positions)
 *   "1 10 / 5"        -> NO    (can't use the single 5 twice)
 *
 * Build:  cc -Wall -O2 -o subarray_notepad_concept subarray_notepad_concept.c
 * Run:    ./subarray_notepad_concept < t_a.txt
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXV 1000        /* largest possible a[i] */

int main(void) {
    int n, K;
    if (scanf("%d %d", &n, &K) != 2) return 0;

    /* ---- THE NOTEPAD ----------------------------------------------------
     * seen[v] == 1  means "I have already passed the value v".
     * Start blank (all zeros). calloc zeroes it for you.
     * (For big/negative values this array can't work -> that's where a real
     *  hashmap comes in. Same idea, just a smarter notepad.)
     */
    int *seen = calloc(MAXV + 1, sizeof *seen);
    if (!seen) return 1;

    int found = 0;   /* flip to 1 the moment you find a pair */

    for (int i = 0; i < n; i++) {
        int x;
        scanf("%d", &x);

		// this is the number that would add up to K
		int needed = K - x;
		if ( needed >= 0 && needed <= MAXV && seen[needed] ){
				found = 1;
				break;
		}

		seen[x] = 1;

    }

    printf("%s\n", found ? "YES" : "NO");

    free(seen);
    return 0;
}
