#include <bits/stdc++.h>
using namespace std;

/*
========================================================
SMART POINTERS FROM SCRATCH (SharedPtr + WeakPtr)
========================================================

Core Idea:
- SharedPtr → OWNS the object (shared ownership)
- WeakPtr   → OBSERVES the object (no ownership)

Both use a CONTROL BLOCK:
    [ptr] + [shared_count] + [weak_count]

========================================================
*/


// ================================
// CONTROL BLOCK
// ================================
template<typename T>
struct ControlBlock {
    T* ptr;
    int shared_count;
    int weak_count;

    ControlBlock(T* p)
        : ptr(p), shared_count(1), weak_count(0) {}
};


// Forward declaration
template<typename T>
class WeakPtr;


// ================================
// SHARED PTR
// ================================
template<typename T>
class SharedPtr {
private:
    ControlBlock<T>* cb;

public:
    // Default constructor
    SharedPtr() : cb(nullptr) {}

    // Constructor from raw pointer
    explicit SharedPtr(T* p) {
        if (p) {
            cb = new ControlBlock<T>(p);
        } else {
            cb = nullptr;
        }
    }

    // Copy constructor
    SharedPtr(const SharedPtr& other) {
        cb = other.cb;
        if (cb) cb->shared_count++;
    }

    // Copy assignment
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();

            cb = other.cb;
            if (cb) cb->shared_count++;
        }
        return *this;
    }

    // Destructor
    ~SharedPtr() {
        release();
    }

    // Release ownership
    void release() {
        if (!cb) return;

        cb->shared_count--;

        // If last owner → delete object
        if (cb->shared_count == 0) {
            delete cb->ptr;

            // If no weak_ptr → delete control block
            if (cb->weak_count == 0) {
                delete cb;
            }
        }

        cb = nullptr;
    }

    // Access raw pointer
    T* get() const {
        return cb ? cb->ptr : nullptr;
    }

    // Dereference
    T& operator*() const {
        return *(cb->ptr);
    }

    T* operator->() const {
        return cb->ptr;
    }

    // Reference count
    int use_count() const {
        return cb ? cb->shared_count : 0;
    }

    // Allow WeakPtr to access control block
    template<typename U>
    friend class WeakPtr;
};


// ================================
// WEAK PTR
// ================================
template<typename T>
class WeakPtr {
private:
    ControlBlock<T>* cb;

public:
    // Default constructor
    WeakPtr() : cb(nullptr) {}

    // From SharedPtr
    WeakPtr(const SharedPtr<T>& sp) {
        cb = sp.cb;
        if (cb) cb->weak_count++;
    }

    // Copy constructor
    WeakPtr(const WeakPtr& other) {
        cb = other.cb;
        if (cb) cb->weak_count++;
    }

    // Assignment
    WeakPtr& operator=(const WeakPtr& other) {
        if (this != &other) {
            release();
            cb = other.cb;
            if (cb) cb->weak_count++;
        }
        return *this;
    }

    // Destructor
    ~WeakPtr() {
        release();
    }

    void release() {
        if (!cb) return;

        cb->weak_count--;

        // Delete control block only when BOTH are zero
        if (cb->weak_count == 0 && cb->shared_count == 0) {
            delete cb;
        }

        cb = nullptr;
    }

    // Check if object is deleted
    bool expired() const {
        return !cb || cb->shared_count == 0;
    }

    // Convert to SharedPtr safely
    SharedPtr<T> lock() const {
        if (expired()) {
            return SharedPtr<T>(); // empty
        }

        SharedPtr<T> sp;
        sp.cb = cb;
        cb->shared_count++;
        return sp;
    }
};


// ================================
// MAIN (TESTING)
// ================================
int main() {

    cout << "Creating SharedPtr...\n";
    SharedPtr<int> sp(new int(42));

    cout << "Shared count: " << sp.use_count() << endl;

    cout << "\nCreating WeakPtr...\n";
    WeakPtr<int> wp(sp);

    {
        cout << "\nLocking weak_ptr...\n";
        SharedPtr<int> sp2 = wp.lock();

        if (sp2.get()) {
            cout << "Value: " << *sp2 << endl;
        }

        cout << "Shared count: " << sp.use_count() << endl;
    }

    cout << "\nReleasing SharedPtr...\n";
    sp.release();  // simulate destruction

    if (wp.expired()) {
        cout << "Object has been deleted\n";
    }

    cout << "\nEnd of program\n";

    return 0;
}