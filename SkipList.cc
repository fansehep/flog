#include <atomic>
#include <cassert>
#include <cstdlib>
#include <fmt/format.h>
#include <string>
#include <functional>


using Key = std::string;
using Value = std::string;

//* std::memory_order_relax 是一种无承诺的保证,
//* 使用这种模式与没有使用这种的模式的效果是一样的.

//* std::memory_order_releae: 在本行代码之前, 如果有任何写
//*     内存的操作, 都不能放在本行语句之后,

//* 最大的层级指针高度
enum {
  kMaxHeight = 12,
};

class SkipList {
private:
  struct Node {
    explicit Node(const Key &k) : key(k) {}

    Key const key;

    Node *Next(int n) {
      assert(n >= 0);
      return next_[n].load(std::memory_order_acquire);
    }

    void SetNext(int n, Node *x) {
      assert(n >= 0);
      next_[n].store(x, std::memory_order_release);
    }

    Node *NoBarrier_Next(int n) {
      assert(n >= 0);
      return next_[n].load(std::memory_order_relaxed);
    }

    void NoBarrier_SetNext(int n, Node *x) {
      assert(n >= 0);
      next_[n].store(x, std::memory_order_relaxed);
    }

  private:
    // 层级指针
    std::atomic<Node *> next_[1];
  };

public:
  void Insert(const Key &key) {
    Node *prev[kMaxHeight];
    Node *x = FindGreaterOrEqual(key, prev);
  }

private:

  Node *FindGreaterOrEqual(const Key &key, Node **prev) const {
    Node* x = head_;
    int level = GetMaxHeight() - 1;
    //* 二分查找?
    //
    while (true) {
      //*
      Node* next = x->Next(level);
      if (KeyIsAfterNode(key, next)) {
        x = next;
      } else {

        if (nullptr != prev) {
          prev[level] = x;
        }

        if (level == 0) {
          return next;
        } else {
          level--;
        }

      }
    }
  }

  Node* FindLessThan(const Key& key) const {
    Node* x = head_;
    int level = GetMaxHeight() - 1;
    while (true) {
      assert(x == head_ || compare(x->key, key))
    }
  }




  bool KeyIsAfterNode(const Key& key, Node* node) const {
    return (nullptr != node) && (compare_(node->key, key) < 0);
  }

  int GetMaxHeight() const {
    return max_height_.load(std::memory_order_relaxed);
  }

  
  std::function<const int (const Key&, const Key&)> compare_;


  Node* const head_;


  std::atomic<int> max_height_;


};

int main() {

  return 0;
}