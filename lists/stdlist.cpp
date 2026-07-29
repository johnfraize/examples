/*
 * std::list / std::forward_list tour.
 *
 * The C++ side of this directory. `lists.c` next door does the same idea with
 * the BSD <sys/queue.h> macros in plain C -- worth reading both back to back.
 *
 *   std::list          doubly-linked. O(1) insert/erase ANYWHERE (given an
 *                      iterator), O(1) push_front/push_back, no random access.
 *   std::forward_list  singly-linked. Smaller (one pointer per node), but you
 *                      can only walk forward and every mutation is *_after().
 *
 * The honest summary: you almost always want std::vector. Linked nodes are
 * scattered in memory, so traversal blows the cache, and finding the position
 * to insert at is O(n) anyway. std::list earns its keep for exactly two things:
 *
 *   1. Iterator/reference STABILITY -- inserting or erasing never invalidates
 *      iterators to other elements. vector invalidates everything on realloc.
 *   2. O(1) splice -- moving elements between lists by relinking pointers,
 *      no copying. This is what an LRU cache is built on.
 *
 * Build: ./build.sh          (g++ -std=c++17 -Wall -o stdlist stdlist.cpp)
 * Run:   ./stdlist
 */

#include <algorithm>
#include <forward_list>
#include <iostream>
#include <list>
#include <string>

template <typename L>
void dumpList(const std::string& label, const L& l) {
  std::cout << label << ": ";
  for (const auto& v : l) std::cout << v << " ";
  std::cout << std::endl;
}

class Song {
public:
  std::string title;
  int capo;
  Song(std::string t, int c) : title(std::move(t)), capo(c) {}
};

int main() {
  // ---------------------------------------------------------------
  // 1. Building one, and pushing at BOTH ends
  // ---------------------------------------------------------------
  std::list<int> nums{10, 58, 33, 1, 2, 3, 77};

  nums.push_back(99);    // O(1), same as vector
  nums.push_front(0);    // O(1) -- vector cannot do this cheaply
  nums.emplace_back(100);
  nums.emplace_front(-1);
  dumpList("after pushes", nums);

  std::cout << "front=" << nums.front() << " back=" << nums.back()
            << " size=" << nums.size() << std::endl;  // size() is O(1) since C++11

  nums.pop_front();
  nums.pop_back();
  dumpList("after pops", nums);

  // ---------------------------------------------------------------
  // 2. No random access -- and how you get around it
  // ---------------------------------------------------------------
  // There is no nums[3] and no nums.at(3). Nodes are chained, not contiguous,
  // so the compiler will not even let you try. To reach position 3 you walk:
  auto it = nums.begin();
  std::advance(it, 3);  // O(n) -- this is the cost of a linked list
  std::cout << "4th element = " << *it << std::endl;

  // std::next/std::prev are the non-mutating versions.
  std::cout << "one past it = " << *std::next(it) << std::endl;

  // Iterators are bidirectional, not random-access: ++/-- work, it + 3 does not.
  --it;
  std::cout << "stepped back to " << *it << std::endl;

  // ---------------------------------------------------------------
  // 3. Insert and erase in the middle -- the thing list is good at
  // ---------------------------------------------------------------
  auto pos = std::find(nums.begin(), nums.end(), 33);  // the O(n) part
  if (pos != nums.end()) {
    nums.insert(pos, 333);  // the O(1) part -- just relinks two pointers
  }
  dumpList("after inserting 333 before 33", nums);

  // erase() returns the next iterator, same idiom as map/set.
  for (auto i = nums.begin(); i != nums.end();) {
    if (*i % 2 == 0) {
      i = nums.erase(i);
    } else {
      ++i;
    }
  }
  dumpList("odds only", nums);

  // remove/remove_if are MEMBER functions here and they actually remove.
  // (On a vector you need the erase-remove idiom; list does it in one call
  // because it can unlink nodes instead of shifting elements.)
  nums.remove(77);
  nums.remove_if([](int v) { return v > 100; });
  dumpList("after remove/remove_if", nums);

  // ---------------------------------------------------------------
  // 4. Iterator stability -- the real reason to pick std::list
  // ---------------------------------------------------------------
  std::list<int> stable{1, 2, 3};
  auto watch = std::find(stable.begin(), stable.end(), 2);  // points at the 2

  for (int i = 0; i < 1000; i++) stable.push_back(i);  // massive growth
  stable.push_front(-1);
  stable.erase(stable.begin());

  // `watch` is STILL valid and still points at the same element. Do this to a
  // vector and the reallocation leaves you with a dangling iterator.
  std::cout << "watched iterator survived 1000 inserts, still = " << *watch
            << std::endl;

  // ---------------------------------------------------------------
  // 5. Member sort/merge/unique/reverse
  // ---------------------------------------------------------------
  // std::sort needs random-access iterators, so it does NOT work on a list.
  // list ships its own merge sort that relinks nodes instead of moving values.
  std::list<int> messy{5, 3, 9, 1, 3, 7, 3};
  messy.sort();
  dumpList("sorted", messy);

  messy.unique();  // collapses ADJACENT duplicates -- sort first, like uniq(1)
  dumpList("uniqued", messy);

  messy.reverse();
  dumpList("reversed", messy);

  messy.sort(std::greater<int>());  // custom comparator
  dumpList("sorted descending", messy);

  // merge() consumes another sorted list, splicing its nodes in. No copies,
  // and `other` ends up empty.
  std::list<int> a{1, 4, 7}, b{2, 3, 8};
  a.merge(b);
  dumpList("merged", a);
  std::cout << "b is now empty: " << std::boolalpha << b.empty() << std::endl;

  // ---------------------------------------------------------------
  // 6. splice() -- O(1) transfer between lists
  // ---------------------------------------------------------------
  // Moves nodes from one list to another by rewiring pointers. Nothing is
  // copied or allocated, and iterators into the moved elements stay valid.
  std::list<std::string> playing{"one", "two", "three"};
  std::list<std::string> queued{"four", "five"};

  // Move ALL of queued to the end of playing.
  playing.splice(playing.end(), queued);
  dumpList("spliced whole list", playing);
  std::cout << "queued is empty: " << queued.empty() << std::endl;

  // Move ONE element to the front -- this single call is the core move of an
  // LRU cache: "touch this entry, promote it to most-recently-used", O(1).
  auto three = std::find(playing.begin(), playing.end(), "three");
  playing.splice(playing.begin(), playing, three);
  dumpList("promoted 'three' to front", playing);

  // Splice within one list is also how you do an O(1) rotate-by-one.
  playing.splice(playing.end(), playing, playing.begin());
  dumpList("rotated left by one", playing);

  // ---------------------------------------------------------------
  // 7. Holding objects, not ints
  // ---------------------------------------------------------------
  std::list<Song> setlist;
  setlist.emplace_back("Blackbird", 0);
  setlist.emplace_back("Here Comes the Sun", 7);
  setlist.emplace_front("Dear Prudence", 0);

  setlist.sort([](const Song& x, const Song& y) { return x.title < y.title; });
  for (const auto& s : setlist) {
    std::cout << s.title << " (capo " << s.capo << ")" << std::endl;
  }

  // ---------------------------------------------------------------
  // 8. std::forward_list -- singly linked, and it shows
  // ---------------------------------------------------------------
  // One pointer per node instead of two. The price: no size(), no back(),
  // no push_back(), no reverse iteration, and every mutation is "_after"
  // because you cannot reach the previous node to fix its next pointer.
  std::forward_list<int> fwd{3, 1, 4, 1, 5};
  fwd.push_front(9);
  dumpList("forward_list", fwd);

  // To insert "at position 2" you address the node BEFORE it.
  auto f = fwd.begin();
  std::advance(f, 1);
  fwd.insert_after(f, 42);
  dumpList("after insert_after", fwd);

  fwd.erase_after(f);  // erases the 42 we just added
  dumpList("after erase_after", fwd);

  // before_begin() is the trick for touching the very first element -- it's a
  // valid iterator you may not dereference, only pass to *_after.
  fwd.insert_after(fwd.before_begin(), 0);
  dumpList("inserted at head", fwd);

  // No size() member. You pay O(n) to count, which is the point: forward_list
  // refuses to store anything it doesn't have to.
  std::cout << "forward_list length = "
            << std::distance(fwd.begin(), fwd.end()) << std::endl;

  fwd.sort();
  fwd.unique();
  dumpList("sorted+uniqued", fwd);

  // ---------------------------------------------------------------
  // 9. Heap allocation, same as the array/vector/map examples
  // ---------------------------------------------------------------
  auto* bigList = new std::list<int>();
  for (int i = 0; i < 1000; i++) bigList->push_back(i);
  std::cout << "heap list size=" << bigList->size()
            << " back=" << bigList->back() << std::endl;
  delete bigList;

  return 0;
}
