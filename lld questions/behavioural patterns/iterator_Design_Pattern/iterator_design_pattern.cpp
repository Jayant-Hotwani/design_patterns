#include <bits/stdc++.h>
using namespace std;

class Iterator{
    public:
    virtual bool hasNext()=0;
    virtual int next()=0;
};

class NumberIterator:public Iterator{
    public:
    vector<int>numbers;
    int index;
    
    NumberIterator(vector<int>& v){
        this->index=0;
        this->numbers=v;
    }

    bool hasNext()override{
        return index<numbers.size();
    }

    int next() override{
        int x=numbers[index];
        index++;
        return x;
    }
};


class NumberCollection{
    public:
    vector<int>numbers;
    public:
    void add(int num) {
        numbers.push_back(num);
    }
    vector<int>& getNumbers() {
        return numbers;
    }
    Iterator* getIterator(){
        return new NumberIterator(numbers);
    }
};

int main(){

    NumberCollection numbercollection;
    numbercollection.add(10);
    numbercollection.add(20);
    numbercollection.add(30);
    numbercollection.add(40);
    numbercollection.add(50);
    Iterator* it=numbercollection.getIterator();
    while(it->hasNext()){
        cout<<it->next()<<endl;
    }

    return 0;
}

