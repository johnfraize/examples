/*
 * std::set / std::unordered_set / std::multiset tour.
 *
 * A set is a map with no values -- just keys, each stored once.
 *
 *   std::set             sorted, red-black tree, O(log n). Iterates in order.
 *   std::unordered_set   hash table, O(1) average. Arbitrary iteration order.
 *   std::multiset        sorted, duplicates allowed (a sorted bag/counter).
 *
 * What sets are actually for: membership tests ("have I seen this?"),
 * deduplication, and -- for std::set -- keeping things sorted as they arrive.
 *
 * Build: ./build.sh          (g++ -std=c++17 -Wall -o sets sets.cpp)
 * Run:   ./sets
 */

#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

template <typename S>
void dumpSet(const std::string& label, const S& s) {
  std::cout << label << " (size " << s.size() << "): ";
  for (const auto& v : s) std::cout << v << " ";
  std::cout << std::endl;
}

int main() {
  // ---------------------------------------------------------------
  // 1. std::set -- sorted and deduplicated for free
  // ---------------------------------------------------------------
  std::set<int> nums{10, 58, 33, 1, 2, 3, 77, 3, 3, 3, 5};

  // The four 3s collapse into one, and it all comes out sorted.
  dumpSet("set from a messy list", nums);

  // ---------------------------------------------------------------
  // 2. Inserting -- the pair<iterator,bool> return is the useful part
  // ---------------------------------------------------------------
  auto [it, inserted] = nums.insert(42);
  std::cout << "insert 42 -> inserted=" << std::boolalpha << inserted << std::endl;

  // Inserting a duplicate is not an error, it's just a no-op that reports false.
  // That bool is the whole "have I seen this before?" idiom in one call:
  auto [it2, inserted2] = nums.insert(42);
  std::cout << "insert 42 again -> inserted=" << inserted2
            << " (already present)" << std::endl;

  nums.emplace(99);                    // construct in place
  nums.insert({100, 101, 102});        // bulk insert from an initializer list

  std::vector<int> more{7, 8, 9, 7, 8};
  nums.insert(more.begin(), more.end());  // bulk insert from a range
  dumpSet("after inserts", nums);

  // ---------------------------------------------------------------
  // 3. Lookup -- there is no operator[]
  // ---------------------------------------------------------------
  // Elements are immutable (changing one would break the ordering invariant),
  // so a set gives you find/count/contains and nothing that hands out a
  // mutable reference. To "change" an element: erase it, insert the new one.
  std::cout << "count(33) = " << nums.count(33) << std::endl;  // 0 or 1
  if (nums.find(1234) == nums.end()) {
    std::cout << "1234 is not in the set" << std::endl;
  }
  // C++20: if (nums.contains(33)) ...

  // ---------------------------------------------------------------
  // 4. Erasing
  // ---------------------------------------------------------------
  size_t removed = nums.erase(42);      // by value; returns count removed
  std::cout << "erase(42) removed " << removed << std::endl;
  nums.erase(nums.begin());             // by iterator (drops the smallest)

  // Erase-while-iterating: same rule as map -- take the iterator erase returns.
  for (auto i = nums.begin(); i != nums.end();) {
    if (*i % 2 == 0) {
      i = nums.erase(i);
    } else {
      ++i;
    }
  }
  dumpSet("odds only", nums);

  // ---------------------------------------------------------------
  // 5. Ordered-only tricks: min/max, range queries, reverse walk
  // ---------------------------------------------------------------
  std::set<int> scores{15, 4, 88, 23, 42, 61, 7};

  // Because it's sorted, the ends are the min and max -- O(1), no scan.
  std::cout << "min=" << *scores.begin()
            << " max=" << *scores.rbegin() << std::endl;

  // lower_bound: first element >= arg.  upper_bound: first element > arg.
  std::cout << "first score >= 20: " << *scores.lower_bound(20) << std::endl;

  // Everything in [20, 65) -- this is the thing unordered_set cannot do.
  std::cout << "scores in [20,65): ";
  for (auto i = scores.lower_bound(20); i != scores.lower_bound(65); ++i) {
    std::cout << *i << " ";
  }
  std::cout << std::endl;

  std::cout << "descending: ";
  for (auto i = scores.rbegin(); i != scores.rend(); ++i) std::cout << *i << " ";
  std::cout << std::endl;

  // A comparator changes the ordering -- and therefore what counts as a
  // duplicate. Two elements are "the same" when neither compares less.
  std::set<int, std::greater<int>> descending(scores.begin(), scores.end());
  dumpSet("set<greater>", descending);

  // ---------------------------------------------------------------
  // 6. Set algebra -- <algorithm> works on any two sorted ranges
  // ---------------------------------------------------------------
  std::set<std::string> mine{"apple", "banana", "cherry", "date"};
  std::set<std::string> yours{"banana", "date", "elderberry"};

  std::vector<std::string> both, onlyMine, all;
  std::set_intersection(mine.begin(), mine.end(), yours.begin(), yours.end(),
                        std::back_inserter(both));
  std::set_difference(mine.begin(), mine.end(), yours.begin(), yours.end(),
                      std::back_inserter(onlyMine));
  std::set_union(mine.begin(), mine.end(), yours.begin(), yours.end(),
                 std::back_inserter(all));

  auto dumpVec = [](const std::string& label, const std::vector<std::string>& v) {
    std::cout << label << ": ";
    for (const auto& s : v) std::cout << s << " ";
    std::cout << std::endl;
  };
  dumpVec("intersection", both);
  dumpVec("difference (mine only)", onlyMine);
  dumpVec("union", all);

  // These need SORTED input, which is exactly what std::set guarantees.
  // Running them on an unordered_set is undefined behavior, not just slow.
  std::cout << "is mine a subset of all? " << std::boolalpha
            << std::includes(all.begin(), all.end(), mine.begin(), mine.end())
            << std::endl;

  // ---------------------------------------------------------------
  // 7. std::unordered_set -- the "seen" set
  // ---------------------------------------------------------------
  // This is the one that shows up in interview problems: O(1) membership.
  std::vector<int> stream{4, 8, 15, 16, 23, 42, 8, 15, 4};
  std::unordered_set<int> seen;
  seen.reserve(stream.size());  // avoid rehashing

  std::cout << "first duplicate in stream: ";
  for (int v : stream) {
    if (!seen.insert(v).second) {  // insert returns false if already there
      std::cout << v << std::endl;
      break;
    }
  }

  // Deduplicate a vector while keeping first-seen order -- set alone would
  // sort it, which is often not what you want.
  std::unordered_set<int> dedupSeen;
  std::vector<int> deduped;
  for (int v : stream) {
    if (dedupSeen.insert(v).second) deduped.push_back(v);
  }
  std::cout << "deduped, original order: ";
  for (int v : deduped) std::cout << v << " ";
  std::cout << std::endl;

  std::cout << "seen buckets=" << seen.bucket_count()
            << " load_factor=" << seen.load_factor() << std::endl;

  // ---------------------------------------------------------------
  // 8. std::multiset -- a sorted bag / counter
  // ---------------------------------------------------------------
  // Keeps duplicates, stays sorted. Handy when you need a running min/max
  // over a collection you also delete from (sliding-window problems).
  std::multiset<int> bag{5, 3, 5, 1, 5, 3};
  dumpSet("multiset", bag);
  std::cout << "count(5) = " << bag.count(5) << std::endl;

  // WATCH OUT: erase(value) removes EVERY copy. To remove just one, erase
  // a single iterator.
  bag.erase(bag.find(5));  // removes one 5
  std::cout << "after erasing one 5, count(5) = " << bag.count(5) << std::endl;
  bag.erase(5);            // removes all remaining 5s
  dumpSet("after erase(5)", bag);

  // equal_range gives you the whole run of equal elements.
  std::multiset<std::string> words{"a", "b", "b", "b", "c"};
  auto [first, last] = words.equal_range("b");
  std::cout << "run of b's: " << std::distance(first, last) << std::endl;

  // ---------------------------------------------------------------
  // 9. Custom element types
  // ---------------------------------------------------------------
  // std::set needs operator< (or a comparator); std::unordered_set needs a
  // std::hash specialization plus operator==. Note the comparator also
  // *defines identity* -- here two Songs with the same title are one element,
  // even if their capo differs.
  struct Song {
    std::string title;
    int capo;
    bool operator<(const Song& o) const { return title < o.title; }
  };

  std::set<Song> setlist;
  setlist.insert({"Blackbird", 0});
  setlist.insert({"Blackbird", 7});  // rejected -- same title
  setlist.insert({"Here Comes the Sun", 7});
  for (const auto& s : setlist) {
    std::cout << s.title << " (capo " << s.capo << ")" << std::endl;
  }
  std::cout << "setlist size = " << setlist.size() << std::endl;

  // ---------------------------------------------------------------
  // 10. Heap allocation, same as the array/vector examples
  // ---------------------------------------------------------------
  auto* bigSet = new std::unordered_set<int>();
  for (int i = 0; i < 1000; i++) bigSet->insert(i * 3);
  std::cout << "heap set has " << bigSet->size() << " entries, contains 2997? "
            << std::boolalpha << (bigSet->count(2997) > 0) << std::endl;
  delete bigSet;

  return 0;
}
