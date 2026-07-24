#include <bits/stdc++.h>
using namespace std;

template <typename T>


class sharedPtr{
    private:
    T* ptr;
    int* reference_count;

    public:
    sharedPtr(T* p = nullptr){
        ptr=p;
        if (p){
            reference_count=new int(1);
        }
        else{
            reference_count=nullptr;
        }
    }

    // copy constructor 
    sharedPtr(const sharedPtr& other){
        ptr=other.ptr;
        reference_count=other.reference_count;
        if (reference_count){
            (*reference_count)++;
        }
    }

    // copy assignment = overloading 
    sharedPtr& operator=(const sharedPtr& other){
        if (this! = &other){
            release();
            ptr=other.ptr;
            reference_count=other.reference_count;
            if (reference_count){
                (*reference_count)++;
            }
        }

        return *this;
    }
    // Destructor
    ~sharedPtr() {
        release();
    }

    void release() {
        if (refCount) {
            (*refCount)--;

            if (*refCount == 0) {
                delete ptr;
                delete refCount;
            }
        }
    }

    // Dereference
    T& operator*() {
        return *ptr;
    }

    T* operator->() {
        return ptr;
    }

    int use_count() {
        return refCount ? *refCount : 0;
    }

};


int main() {
    sharedPtr<int> p1(new int(10));

    cout << p1.use_count() << endl; // 1

    sharedPtr<int> p2 = p1;

    cout << p1.use_count() << endl; // 2

    {
        sharedPtr<int> p3 = p2;
        cout << p1.use_count() << endl; // 3
    }

    cout << p1.use_count() << endl; // 2
}
  