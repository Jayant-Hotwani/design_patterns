/*Bridge Design Pattern (Structural Pattern)

The Bridge Design Pattern is used to decouple an abstraction from its implementation so that the two can vary independently.

Instead of creating a large inheritance hierarchy, Bridge splits the system into two separate hierarchies:

Abstraction (what you do)
Implementation (how you do it)
*/

#include <bits/stdc++.h>
using namespace std;

// implmenting low level classes 
class Engine{
    public :
    Engine(){}
    virtual void start()=0;
    virtual ~Engine(){};
};

class dieselEngine : public Engine{
    public:
    void start(){
        cout<<" diesel engine started "<<endl;
    }
};
class petrolEngine : public Engine{
    public:
    void start(){
        cout<<" petrol engine started "<<endl;
    }
};
class electricEngine : public Engine{
    public:
    void start(){
        cout<<" electric engine started "<<endl;
    }
};


// implementing high level classes 

class Car{
    public:
    Engine* e;
    Car(Engine* e){
        this->e=e;
    }
    virtual void runCar()=0;
};

class Suv:public Car{
    public:
    Suv(Engine* e):Car(e){
    }
    void runCar(){
        e->start();
        cout<<" suv car running "<<endl;
    }
};
class Sedan:public Car{
    public:
    Sedan(Engine* e):Car(e){
    }
    void runCar(){
        e->start();
        cout<<" sedan car running "<<endl;
    }
};


int main(){

    Engine* petrol=new petrolEngine();
    Engine* diesel=new dieselEngine();
    Engine* electric=new electricEngine();

    Car* suv=new Suv(petrol);
    Car* sedan=new Sedan(diesel);

    suv->runCar();
    sedan->runCar();


    return 0;
}