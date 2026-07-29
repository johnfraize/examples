/*
 * std::map / std::unordered_map / std::multimap tour.
 *
 * Associative containers: key -> value.
 *
 *   std::map            sorted by key, red-black tree, O(log n) lookup/insert.
 *                       Iteration order = key order. Keys must be `<` comparable.
 *   std::unordered_map  hash table, O(1) average lookup/insert (O(n) worst case).
 *                       Iteration order = arbitrary. Keys need std::hash + ==.
 *   std::multimap       like map but the same key can appear many times.
 *
 * Rule of thumb: reach for unordered_map unless you need sorted iteration,
 * ordered range queries (lower_bound/upper_bound), or a key type with no hash.
 *
 * Build: ./build.sh          (g++ -std=c++17 -Wall -o maps maps.cpp)
 * Run:   ./maps
 */

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

// A value type, to show maps holding something other than ints.
class Song {
public:
  std::string title;
  int capo;

  // unordered_map::operator[] default-constructs missing values, so a value
  // type used that way needs a default constructor.
  Song() : title("unknown"), capo(0) {}
  Song(std::string t, int c) : title(std::move(t)), capo(c) {}
};

template <typename M>
void dumpMap(const std::string& label, const M& m) {
  std::cout << label << " (size " << m.size() << "): ";
  // Structured bindings (C++17). Pre-17 you'd use `it->first` / `it->second`.
  for (const auto& [key, value] : m) {
    std::cout << key << "=" << value << " ";
  }
  std::cout << std::endl;
}

int main() {
  // ---------------------------------------------------------------
  // 1. std::map -- always iterates in sorted key order
  // ---------------------------------------------------------------
  std::map<std::string, int> ages{
      {"john", 52}, {"alice", 30}, {"bob", 41}, {"zed", 7}};

  // Iteration comes out alice, bob, john, zed -- NOT insertion order.
  dumpMap("map sorted by key", ages);

  // ---------------------------------------------------------------
  // 2. Four ways to put something in, and how they differ
  // ---------------------------------------------------------------

  // operator[] : inserts a default-constructed value if the key is missing,
  // then hands you a reference. Convenient, but it MUTATES the map even when
  // you were only "reading" -- a classic bug source.
  ages["carol"] = 25;
  ages["john"] = 53;  // overwrites

  // insert() : does NOT overwrite. Returns {iterator, bool inserted}.
  auto [it, inserted] = ages.insert({"john", 999});
  std::cout << "insert john again -> inserted=" << std::boolalpha << inserted
            << ", current value=" << it->second << std::endl;

  // emplace() : constructs the pair in place, also does not overwrite.
  ages.emplace("dave", 60);

  // insert_or_assign() (C++17) : the "I really do want to overwrite" version,
  // and unlike operator[] it works for value types with no default ctor.
  ages.insert_or_assign("zed", 8);

  dumpMap("after inserts", ages);

  // ---------------------------------------------------------------
  // 3. Lookup -- find/count/contains, and the operator[] trap
  // ---------------------------------------------------------------

  // find() returns end() when absent. This is the safe read.
  if (auto f = ages.find("bob"); f != ages.end()) {
    std::cout << "found bob = " << f->second << std::endl;
  }

  // count() is 0 or 1 for map; contains() (C++20) reads better.
  std::cout << "count(nobody) = " << ages.count("nobody") << std::endl;

  // at() bounds-checks and throws std::out_of_range instead of inserting.
  try {
    std::cout << ages.at("ghost") << std::endl;
  } catch (const std::out_of_range& e) {
    std::cout << "at(ghost) threw: " << e.what() << std::endl;
  }

  // The trap: this *creates* ghost=0 just by looking.
  std::cout << "operator[](ghost) = " << ages["ghost"] << std::endl;
  std::cout << "map size is now " << ages.size() << " -- [] inserted ghost"
            << std::endl;

  // ---------------------------------------------------------------
  // 4. Erasing
  // ---------------------------------------------------------------
  ages.erase("ghost");         // by key, returns how many were removed
  ages.erase(ages.begin());    // by iterator (removes lowest key, "alice")
  dumpMap("after erases", ages);

  // Erasing while iterating: erase() invalidates only the erased iterator,
  // so capture the next one. Never `for (auto it = ...; ++it)` and erase inside.
  for (auto i = ages.begin(); i != ages.end();) {
    if (i->second < 30) {
      i = ages.erase(i);  // erase returns the next valid iterator
    } else {
      ++i;
    }
  }
  dumpMap("after erase-if under 30", ages);

  // ---------------------------------------------------------------
  // 5. Ordered-only tricks: lower_bound / upper_bound / reverse iteration
  // ---------------------------------------------------------------
  std::map<int, std::string> timeline{
      {1969, "moon"}, {1977, "voyager"}, {1989, "web"}, {2007, "iphone"}};

  // lower_bound: first key >= arg.  upper_bound: first key > arg.
  auto lb = timeline.lower_bound(1980);
  std::cout << "first event >= 1980: " << lb->first << " " << lb->second
            << std::endl;

  // Range query -- everything in [1970, 2000).
  std::cout << "events in the 70s-90s: ";
  for (auto i = timeline.lower_bound(1970); i != timeline.lower_bound(2000); ++i) {
    std::cout << i->first << " ";
  }
  std::cout << std::endl;

  // Walk backwards. unordered_map cannot do any of this.
  std::cout << "newest first: ";
  for (auto i = timeline.rbegin(); i != timeline.rend(); ++i) {
    std::cout << i->first << " ";
  }
  std::cout << std::endl;

  // A custom comparator flips the ordering (or orders a key type that has no <).
  std::map<int, std::string, std::greater<int>> descending(timeline.begin(),
                                                           timeline.end());
  std::cout << "map<greater> iterates: ";
  for (const auto& [year, what] : descending) std::cout << year << " ";
  std::cout << std::endl;

  // ---------------------------------------------------------------
  // 6. std::unordered_map -- the hash table
  // ---------------------------------------------------------------
  // Same interface, no ordering guarantee. This is the one you want for
  // frequency counting, memoization, seen-sets, two-sum style problems.
  std::unordered_map<char, int> freq;
  const std::string text = "the quick brown fox jumps over the lazy dog";
  for (char c : text) {
    if (c != ' ') {
      freq[c]++;  // [] default-constructs int to 0, then ++ -- idiomatic here
    }
  }
  std::cout << "letter 'o' appears " << freq['o'] << " times, "
            << freq.size() << " distinct letters" << std::endl;

  // reserve() up front avoids rehashing if you know roughly how many keys.
  std::unordered_map<std::string, Song> library;
  library.reserve(100);
  library["blackbird"] = Song("Blackbird", 0);
  library.emplace("here_comes", Song("Here Comes the Sun", 7));

  for (const auto& [key, song] : library) {
    std::cout << "library[" << key << "] = " << song.title
              << " capo " << song.capo << std::endl;
  }

  // Bucket stats -- only unordered containers expose these.
  std::cout << "freq buckets=" << freq.bucket_count()
            << " load_factor=" << freq.load_factor() << std::endl;

  // ---------------------------------------------------------------
  // 7. std::multimap -- duplicate keys allowed
  // ---------------------------------------------------------------
  // No operator[] (which value would it return?). Use equal_range to get
  // every value stored under one key.
  std::multimap<std::string, std::string> band;
  band.emplace("guitar", "john");
  band.emplace("guitar", "george");
  band.emplace("bass", "paul");
  band.emplace("drums", "ringo");

  auto [first, last] = band.equal_range("guitar");
  std::cout << "guitarists: ";
  for (auto i = first; i != last; ++i) std::cout << i->second << " ";
  std::cout << "(count=" << band.count("guitar") << ")" << std::endl;

  // The common alternative to multimap: map of vectors. Easier to work with
  // when you want the whole group at once.
  std::map<std::string, std::vector<std::string>> byInstrument;
  byInstrument["guitar"].push_back("john");     // [] creates the empty vector
  byInstrument["guitar"].push_back("george");
  std::cout << "map-of-vector guitar count: "
            << byInstrument["guitar"].size() << std::endl;

  // ---------------------------------------------------------------
  // 8. Custom key types
  // ---------------------------------------------------------------
  // std::map needs operator< (or a comparator). std::unordered_map needs a
  // std::hash specialization plus operator==. Ordering is the cheaper one to
  // supply, which is why map is sometimes the pragmatic pick.
  struct Point {
    int x, y;
    bool operator<(const Point& o) const {
      return x != o.x ? x < o.x : y < o.y;  // lexicographic
    }
  };

  std::map<Point, std::string> grid;
  grid[{0, 0}] = "origin";
  grid[{1, 2}] = "somewhere";
  for (const auto& [p, name] : grid) {
    std::cout << "(" << p.x << "," << p.y << ") = " << name << std::endl;
  }

  // ---------------------------------------------------------------
  // 9. Heap allocation, same as the array/vector examples
  // ---------------------------------------------------------------
  auto* bigMap = new std::unordered_map<int, int>();
  for (int i = 0; i < 1000; i++) {
    (*bigMap)[i] = i * i;
  }
  std::cout << "heap map[999] = " << bigMap->at(999) << std::endl;
  delete bigMap;  // or just use a stack map / unique_ptr and skip this

  return 0;
}
