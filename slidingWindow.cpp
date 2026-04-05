/*
========================================================
SLIDING WINDOW MASTER NOTES (Templates + Top Problems)
Single CPP Revision File
========================================================

WHAT IS SLIDING WINDOW?
--------------------------------------------------------
Used when problem asks about:
- contiguous subarray / substring
- longest / shortest / count / max / min
- exact frequency / distinct / at most K / exactly K

Core 4 Questions:
1) What does entering element do?
2) What does leaving element do?
3) When is window valid / invalid?
4) What answer do I update?

========================================================
TEMPLATE 1) FIXED SIZE WINDOW
========================================================
Use when window size = k is already known.

Examples:
- max sum subarray of size k
- anagram / permutation matching
- count distinct in every window of size k

Pseudo:
--------------------------------------------------------
int start = 0;

for (int end = 0; end < n; end++) {
    // include arr[end]

    int wsize = end - start + 1;

    if (wsize < k) continue;

    // process current window of size k

    // remove arr[start]
    start++;
}

--------------------------------------------------------
Common signs:
- "substring/subarray of size k"
- "window length exactly pattern.length()"
========================================================



========================================================
TEMPLATE 2) VARIABLE SIZE WINDOW (SHRINK WHILE VALID)
========================================================
Use when:
- need smallest valid window
- need min length substring / subarray

Examples:
- minimum window containing all chars
- smallest subarray with sum >= target

Pseudo:
--------------------------------------------------------
int start = 0;

for (int end = 0; end < n; end++) {
    // include arr[end]

    while ( window valid ) {
        // update answer (minimum / shortest)

        // remove arr[start]
        start++;
    }
}

--------------------------------------------------------
Common signs:
- "minimum length"
- "smallest substring/subarray satisfying condition"
========================================================



========================================================
TEMPLATE 3) VARIABLE SIZE WINDOW (SHRINK WHILE INVALID)
========================================================
Use when:
- need longest valid window
- maintain valid window as much as possible

Examples:
- longest unique substring
- longest substring after at most k changes
- longest subarray with at most k bad elements

Pseudo:
--------------------------------------------------------
int start = 0;

for (int end = 0; end < n; end++) {
    // include arr[end]

    while ( window invalid ) {
        // remove arr[start]
        start++;
    }

    // update answer (maximum / longest)
}

--------------------------------------------------------
Common signs:
- "longest"
- "maximum length"
- "at most k ..."
========================================================



========================================================
TEMPLATE 4) AT MOST K
========================================================
Use when:
- at most K distinct / odd / zero / bad chars / etc.

Pseudo:
--------------------------------------------------------
int start = 0;

for (int end = 0; end < n; end++) {
    // include arr[end]
    // update count / distinct / freq

    while ( count > K ) {
        // remove arr[start]
        start++;
    }

    // current window is valid
    // update answer
}

--------------------------------------------------------
Common signs:
- "at most K distinct"
- "flip at most K zeros"
- "change at most K chars"
========================================================



========================================================
TEMPLATE 5) EXACTLY K
========================================================
Use when:
- exactly K distinct / odd / etc.

Trick:
exactly(K) = atMost(K) - atMost(K - 1)

Pseudo:
--------------------------------------------------------
int exactlyK(...) {
    return atMostK(k) - atMostK(k - 1);
}

--------------------------------------------------------
Common signs:
- "exactly K distinct"
- "exactly K odd numbers"
========================================================



========================================================
TEMPLATE 6) COUNT ALL VALID WINDOWS
========================================================
Use when:
- count number of valid subarrays / substrings

Pseudo:
--------------------------------------------------------
int ans = 0;
int start = 0;

for (int end = 0; end < n; end++) {
    // include arr[end]

    while ( invalid ) {
        // remove arr[start]
        start++;
    }

    // all windows ending at end and starting from [start..end] are valid
    ans += (end - start + 1);
}

--------------------------------------------------------
Common signs:
- "count subarrays / substrings"
- "how many valid windows"
========================================================



========================================================
TEMPLATE 7) LONGEST / MAX WINDOW
========================================================
Use when:
- find maximum length valid window

Pseudo:
--------------------------------------------------------
int ans = 0;
int start = 0;

for (int end = 0; end < n; end++) {
    // include arr[end]

    while ( invalid ) {
        // remove arr[start]
        start++;
    }

    ans = max(ans, end - start + 1);
}
========================================================



========================================================
TEMPLATE 8) SHORTEST / MIN WINDOW
========================================================
Use when:
- find minimum length valid window

Pseudo:
--------------------------------------------------------
int ans = INT_MAX;
int start = 0;

for (int end = 0; end < n; end++) {
    // include arr[end]

    while ( valid ) {
        ans = min(ans, end - start + 1);

        // remove arr[start]
        start++;
    }
}
========================================================



========================================================
TEMPLATE 9) ANAGRAM / PERMUTATION MATCHING
========================================================
Use when:
- exact frequency match of pattern in string

Examples:
- permutation in string
- find all anagrams

Idea:
freq[ch] = how many more of this char are needed
need = total chars still needed

Pseudo:
--------------------------------------------------------
vector<int> freq(26);
for (char c : pattern) freq[c - 'a']++;

int need = pattern.size();
int start = 0;

for (int end = 0; end < s.size(); end++) {
    if (freq[s[end] - 'a'] > 0) need--;
    freq[s[end] - 'a']--;

    int wsize = end - start + 1;
    if (wsize < pattern.size()) continue;

    if (need == 0) {
        // permutation / anagram found
    }

    freq[s[start] - 'a']++;
    if (freq[s[start] - 'a'] > 0) need++;
    start++;
}

Time:
O(n)
========================================================



========================================================
TEMPLATE 10) UNIQUE / NO DUPLICATES
========================================================
Use when:
- no repeating chars allowed

Pseudo:
--------------------------------------------------------
unordered_map<char, int> freq;
int start = 0;

for (int end = 0; end < s.size(); end++) {
    freq[s[end]]++;

    while (freq[s[end]] > 1) {
        freq[s[start]]--;
        start++;
    }

    // valid unique window
}
========================================================



========================================================
HOW TO IDENTIFY THE CORRECT TEMPLATE FAST
========================================================

1) Window size fixed?
=> FIXED SIZE

2) Need smallest valid window?
=> SHRINK WHILE VALID

3) Need longest valid window?
=> SHRINK WHILE INVALID

4) "At most K"?
=> AT MOST K

5) "Exactly K"?
=> atMost(K) - atMost(K-1)

6) Need count of all valid windows?
=> ans += (end - start + 1)

7) Need exact pattern frequency match?
=> ANAGRAM / PERMUTATION TEMPLATE

========================================================
TOP 20 SLIDING WINDOW PROBLEMS GROUPED BY TEMPLATE
========================================================



--------------------------------------------------------
[A] FIXED SIZE WINDOW
--------------------------------------------------------

1) LC 567. Permutation in String
   - Template: Fixed size + freq
   - Window size = s1.length()

2) LC 438. Find All Anagrams in a String
   - Same as LC 567
   - Store all starting indices

3) Max Sum Subarray of Size K (classic)
   - Fixed size numeric window

4) Count Distinct Elements in Every Window of Size K
   - Fixed size + hashmap

--------------------------------------------------------
[B] VARIABLE WINDOW (SHRINK WHILE VALID) / MIN WINDOW
--------------------------------------------------------

5) LC 76. Minimum Window Substring
   - Expand until valid
   - Shrink while valid
   - Find smallest valid substring

6) LC 209. Minimum Size Subarray Sum
   - Smallest subarray with sum >= target

7) Smallest Window Containing 0,1,2 (classic variant)
   - Same min-window pattern

--------------------------------------------------------
[C] VARIABLE WINDOW (SHRINK WHILE INVALID) / LONGEST WINDOW
--------------------------------------------------------

8) LC 3. Longest Substring Without Repeating Characters
   - Shrink when duplicate appears

9) LC 424. Longest Repeating Character Replacement
   - Shrink when replacements needed > k

10) LC 1493. Longest Subarray of 1's After Deleting One Element
   - Maintain at most one zero deleted

11) LC 1004. Max Consecutive Ones III
   - Flip at most K zeros

12) LC 2024. Maximize the Confusion of an Exam
   - Similar to LC 424

--------------------------------------------------------
[D] AT MOST K
--------------------------------------------------------

13) LC 904. Fruit Into Baskets
   - At most 2 distinct elements

14) LC 340. Longest Substring with At Most K Distinct Characters
   - At most K distinct chars

15) LC 159. Longest Substring with At Most Two Distinct Characters
   - At most 2 distinct

--------------------------------------------------------
[E] EXACTLY K
--------------------------------------------------------

16) LC 992. Subarrays with K Different Integers
   - exactly(K) = atMost(K) - atMost(K-1)

17) LC 1248. Count Number of Nice Subarrays
   - exactly K odd numbers

18) Count Substrings with Exactly K Distinct Characters (classic)
   - exactly K distinct chars

--------------------------------------------------------
[F] COUNT ALL VALID WINDOWS
--------------------------------------------------------

19) LC 930. Binary Subarrays With Sum
   - count using prefix or sliding window trick

20) LC 1358. Number of Substrings Containing All Three Characters
   - count all valid substrings

--------------------------------------------------------
[BONUS GOOD ONES]
--------------------------------------------------------

21) LC 239. Sliding Window Maximum
   - technically sliding window, but solved with deque

22) LC 1456. Maximum Number of Vowels in a Substring of Given Length
   - fixed size

23) LC 1208. Get Equal Substrings Within Budget
   - variable size, shrink while invalid

24) LC 1838. Frequency of the Most Frequent Element
   - sorted + variable window

25) LC 713. Subarray Product Less Than K
   - count all valid windows

========================================================
PROBLEM → TEMPLATE MAP (FAST REVISION)
========================================================

LC 567  -> Fixed window + freq
LC 438  -> Fixed window + freq
LC 76   -> Variable window (shrink while valid)
LC 209  -> Variable window (min length)
LC 3    -> Unique / shrink while invalid
LC 424  -> Longest valid window
LC 1004 -> At most K bad chars
LC 904  -> At most K distinct
LC 340  -> At most K distinct
LC 992  -> Exactly K distinct
LC 1248 -> Exactly K odd
LC 1358 -> Count all valid windows
LC 930  -> Count windows
LC 1456 -> Fixed window
LC 239  -> Deque window

========================================================
INTERVIEW MASTER RULE
========================================================

Before coding, ask:

1) Is window size fixed or variable?
2) What makes window valid / invalid?
3) What changes when right enters?
4) What changes when left leaves?
5) Am I finding:
   - max / longest
   - min / shortest
   - count
   - exact match

========================================================
END
========================================================
*/