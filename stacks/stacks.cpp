/*
 * std::stack tour (plus its sibling adaptors, queue and priority_queue).
 *
 * std::stack is not a container -- it's a container ADAPTOR. It wraps another
 * container and hides most of it, exposing only LIFO operations:
 *
 *     push / emplace   put one on top
 *     top              look at the top (does NOT remove)
 *     pop              remove the top (returns void -- see note below)
 *     empty / size
 *
 * That's the whole interface. No iteration, no indexing, no find. The point is
 * the restriction: a stack that can't be indexed into can't be misused, and the
 * signature documents "this algorithm is LIFO".
 *
 * By default it adapts a std::deque. You can swap in vector or list.
 *
 * Build: ./build.sh          (g++ -std=c++17 -Wall -o stacks stacks.cpp)
 * Run:   ./stacks
 */

#include <cctype>
#include <iostream>
#include <list>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

// ---------------------------------------------------------------
// Classic stack problem #1: balanced brackets
// ---------------------------------------------------------------
// Push every opener; on a closer, the top must be its match. This is THE
// canonical stack problem -- if a problem says "matching" or "nesting",
// reach for a stack.
bool balanced(const std::string& s) {
  std::stack<char> st;
  for (char c : s) {
    if (c == '(' || c == '[' || c == '{') {
      st.push(c);
    } else if (c == ')' || c == ']' || c == '}') {
      if (st.empty()) return false;  // closer with nothing open
      char open = st.top();
      st.pop();
      if ((c == ')' && open != '(') || (c == ']' && open != '[') ||
          (c == '}' && open != '{')) {
        return false;  // mismatched pair
      }
    }
  }
  return st.empty();  // leftovers mean something was never closed
}

// ---------------------------------------------------------------
// Classic stack problem #2: evaluate RPN (postfix)
// ---------------------------------------------------------------
// "3 4 + 2 *" -> 14. Operands get pushed; an operator pops two and pushes the
// result. Note the pop ORDER matters for - and /: the second pop is the left
// operand.
int evalRPN(const std::string& expr) {
  std::stack<int> st;
  std::istringstream in(expr);
  std::string tok;

  while (in >> tok) {
    if (tok == "+" || tok == "-" || tok == "*" || tok == "/") {
      int rhs = st.top(); st.pop();
      int lhs = st.top(); st.pop();
      if (tok == "+") st.push(lhs + rhs);
      else if (tok == "-") st.push(lhs - rhs);
      else if (tok == "*") st.push(lhs * rhs);
      else st.push(lhs / rhs);
    } else {
      st.push(std::stoi(tok));
    }
  }
  return st.top();
}

// ---------------------------------------------------------------
// Classic stack problem #3: monotonic stack -- next greater element
// ---------------------------------------------------------------
// The stack holds INDICES of elements still waiting for their answer, kept in
// decreasing value order. Each index is pushed and popped at most once, so the
// whole thing is O(n) despite the nested loop. This pattern also solves
// largest-rectangle-in-histogram, daily-temperatures, stock-span.
std::vector<int> nextGreater(const std::vector<int>& v) {
  std::vector<int> result(v.size(), -1);
  std::stack<size_t> st;

  for (size_t i = 0; i < v.size(); i++) {
    // Everything on the stack smaller than v[i] just found its answer.
    while (!st.empty() && v[st.top()] < v[i]) {
      result[st.top()] = v[i];
      st.pop();
    }
    st.push(i);
  }
  // Whatever is left never found a greater element -- stays -1.
  return result;
}

int main() {
  // ---------------------------------------------------------------
  // 1. The basic interface
  // ---------------------------------------------------------------
  std::stack<int> st;
  st.push(10);
  st.push(20);
  st.emplace(30);  // constructs in place, like emplace_back

  std::cout << "size=" << st.size() << " top=" << st.top() << std::endl;

  // top() returns a REFERENCE, so you can modify the top in place.
  st.top() = 99;
  std::cout << "modified top=" << st.top() << std::endl;

  // The drain loop. Note pop() returns void -- you must read top() first.
  // Why void? A pop that returned by value couldn't be exception-safe: if the
  // copy out threw, the element would already be gone. So the standard splits
  // it into "look" and "remove".
  std::cout << "draining (LIFO): ";
  while (!st.empty()) {
    std::cout << st.top() << " ";
    st.pop();
  }
  std::cout << std::endl;

  // Calling top() or pop() on an empty stack is UNDEFINED BEHAVIOR, not an
  // exception. There is no .at(). Always guard with empty().
  std::cout << "empty now? " << std::boolalpha << st.empty() << std::endl;

  // ---------------------------------------------------------------
  // 2. There is no iteration -- that is the whole design
  // ---------------------------------------------------------------
  // No begin(), no end(), no range-based for, no operator[]. To see the
  // contents you have to destroy them (drain into a copy), which is why the
  // idiom is to copy the stack first if you need to peek through it.
  std::stack<int> original;
  for (int i = 1; i <= 5; i++) original.push(i);

  std::stack<int> copy = original;  // cheap enough, and leaves the original alone
  std::cout << "peeking via a copy: ";
  while (!copy.empty()) { std::cout << copy.top() << " "; copy.pop(); }
  std::cout << "(original still has " << original.size() << ")" << std::endl;

  // If you find yourself needing to iterate, you wanted a vector or deque.

  // ---------------------------------------------------------------
  // 3. Choosing the underlying container
  // ---------------------------------------------------------------
  // Default is deque: grows without reallocating existing elements.
  std::stack<int> defaultStack;                              // deque
  std::stack<int, std::vector<int>> vectorStack;             // contiguous, fast
  std::stack<int, std::list<int>> listStack;                 // stable nodes

  vectorStack.push(1);
  listStack.push(2);
  std::cout << "vector-backed top=" << vectorStack.top()
            << ", list-backed top=" << listStack.top() << std::endl;

  // vector is usually the fastest choice (contiguous, one allocation to grow),
  // but it can't reserve() through the adaptor. If you need that, construct
  // the stack FROM a container you already sized:
  std::vector<int> seed{1, 2, 3};
  seed.reserve(1000);
  std::stack<int, std::vector<int>> preloaded(std::move(seed));
  std::cout << "preloaded from a vector, top=" << preloaded.top()
            << " size=" << preloaded.size() << std::endl;

  // ---------------------------------------------------------------
  // 4. The classic problems
  // ---------------------------------------------------------------
  for (const std::string& test : {"({[]})", "(]", "((()", "a(b)[c]{d}"}) {
    std::cout << "balanced(\"" << test << "\") = " << balanced(test) << std::endl;
  }

  std::cout << "evalRPN(\"3 4 + 2 *\") = " << evalRPN("3 4 + 2 *") << std::endl;
  std::cout << "evalRPN(\"5 1 2 + 4 * + 3 -\") = "
            << evalRPN("5 1 2 + 4 * + 3 -") << std::endl;

  std::vector<int> heights{2, 1, 2, 4, 3};
  std::vector<int> greater = nextGreater(heights);
  std::cout << "nextGreater: ";
  for (size_t i = 0; i < heights.size(); i++) {
    std::cout << heights[i] << "->" << greater[i] << " ";
  }
  std::cout << std::endl;

  // ---------------------------------------------------------------
  // 5. Reversing, and the "stack is just recursion made explicit" point
  // ---------------------------------------------------------------
  std::string word = "stressed";
  std::stack<char> chars;
  for (char c : word) chars.push(c);
  std::string reversed;
  while (!chars.empty()) { reversed += chars.top(); chars.pop(); }
  std::cout << word << " reversed = " << reversed << std::endl;

  // Any recursive algorithm can be rewritten with an explicit stack -- that's
  // literally what the call stack is. Here's an iterative countdown:
  std::stack<int> frames;
  for (int i = 1; i <= 5; i++) frames.push(i);
  std::cout << "unwinding 'call frames': ";
  while (!frames.empty()) { std::cout << frames.top() << " "; frames.pop(); }
  std::cout << std::endl;

  // ---------------------------------------------------------------
  // 6. The sibling adaptors -- same mechanism, different discipline
  // ---------------------------------------------------------------
  // std::queue: FIFO. front()/back() instead of top(), push/pop at opposite
  // ends. Also deque-backed by default.
  std::queue<std::string> q;
  q.push("first");
  q.push("second");
  q.push("third");
  std::cout << "queue front=" << q.front() << " back=" << q.back() << std::endl;
  std::cout << "draining (FIFO): ";
  while (!q.empty()) { std::cout << q.front() << " "; q.pop(); }
  std::cout << std::endl;

  // std::priority_queue: top() is always the LARGEST (a binary heap over a
  // vector). O(log n) push/pop, O(1) top.
  std::priority_queue<int> maxHeap;
  for (int v : {3, 1, 4, 1, 5, 9, 2, 6}) maxHeap.push(v);
  std::cout << "max-heap drain: ";
  while (!maxHeap.empty()) { std::cout << maxHeap.top() << " "; maxHeap.pop(); }
  std::cout << std::endl;

  // Min-heap: flip the comparator. The verbose template args are why people
  // just memorize this line.
  std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
  for (int v : {3, 1, 4, 1, 5, 9, 2, 6}) minHeap.push(v);
  std::cout << "min-heap drain: ";
  while (!minHeap.empty()) { std::cout << minHeap.top() << " "; minHeap.pop(); }
  std::cout << std::endl;

  // Custom comparator -- keep the k smallest, priority by a struct field.
  struct Task { std::string name; int priority; };
  auto byPriority = [](const Task& a, const Task& b) {
    return a.priority < b.priority;  // "less" => highest priority on top
  };
  std::priority_queue<Task, std::vector<Task>, decltype(byPriority)> tasks(byPriority);
  tasks.push({"email", 1});
  tasks.push({"outage", 10});
  tasks.push({"standup", 3});
  std::cout << "task order: ";
  while (!tasks.empty()) {
    std::cout << tasks.top().name << "(" << tasks.top().priority << ") ";
    tasks.pop();
  }
  std::cout << std::endl;

  // ---------------------------------------------------------------
  // 7. Heap allocation, same as the array/vector/map examples
  // ---------------------------------------------------------------
  auto* bigStack = new std::stack<int>();
  for (int i = 0; i < 1000; i++) bigStack->push(i);
  std::cout << "heap stack size=" << bigStack->size()
            << " top=" << bigStack->top() << std::endl;
  delete bigStack;

  return 0;
}
