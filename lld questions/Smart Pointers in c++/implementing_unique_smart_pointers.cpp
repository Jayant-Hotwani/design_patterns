#include <iostream>
using namespace std;

template <typename T>
class UniquePtr {
private:
    T* ptr;

public:
    // Constructor
    explicit UniquePtr(T* p = nullptr) : ptr(p) {}

    // Delete copy constructor (no copying allowed)
    UniquePtr(const UniquePtr&) = delete;

    // Delete copy assignment
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Move constructor
    UniquePtr(UniquePtr&& other) noexcept {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    // Move assignment
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;            // delete current
            ptr = other.ptr;       // steal
            other.ptr = nullptr;   // nullify source
        }
        return *this;
    }

    // Destructor
    ~UniquePtr() {
        delete ptr;
    }

    // Dereference operator
    T& operator*() {
        return *ptr;
    }

    // Arrow operator
    T* operator->() {
        return ptr;
    }

    // Get raw pointer
    T* get() {
        return ptr;
    }

    // Release ownership
    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    // Reset pointer
    void reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }
};


int main() {
    UniquePtr<int> p1(new int(10));

    cout << *p1 << endl;

    // UniquePtr<int> p2 = p1; ❌ ERROR (copy not allowed)

    UniquePtr<int> p2 = std::move(p1); // ✅ move allowed

    if (p1.get() == nullptr)
        cout << "p1 is now empty\n";

    cout << *p2 << endl;

    return 0;
}