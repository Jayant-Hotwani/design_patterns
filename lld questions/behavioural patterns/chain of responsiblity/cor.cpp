#include <bits/stdc++.h>
using namespace std;


class MoneyHandler{
    protected:
    MoneyHandler* nextHandler;
    public:
    MoneyHandler(){
        this->nextHandler=nullptr;
    }
    void setNextHandler(MoneyHandler* next){
        this->nextHandler=next;
    }
    virtual void dispense( int amount )=0;
};

class ThousandRupeeHandler:public MoneyHandler{
    protected:
    int notes;
    public:
    ThousandRupeeHandler(int notes){
        this->notes=notes;
    }


    void dispense(int amount) override{
        int required_notes=amount/1000;
        if(required_notes<notes){
            notes-=required_notes;
        }
        else{
            required_notes=notes;
            notes=0;
        }
        if(required_notes>0){
            cout<<" dispensing "<<required_notes<<" of 1000 rupees "<<endl;
        }
        int rem=amount-(1000*required_notes);
       
        if(rem>0){
            if(nextHandler!=nullptr)
            nextHandler->dispense(rem);
            else{
                cout<<"cannot dispense remaining amount of "<<rem<<" rupees"<<endl;
            }
        }
    }
};

class FiveHundredRupeeHandler:public MoneyHandler{
    protected:
    int notes;
    public:
    FiveHundredRupeeHandler(int notes){
        this->notes=notes;
    }


    void dispense(int amount) override{
        int required_notes=amount/500;
        if(required_notes<notes){
            notes-=required_notes;
        }
        else{
            required_notes=notes;
            notes=0;
        }
        if(required_notes>0){
            cout<<" dispensing "<<required_notes<<" of 500 rupees "<<endl;
        }
        
        int rem=amount-(500*required_notes);
     
        if(rem>0){
            if(nextHandler!=nullptr)
            nextHandler->dispense(rem);
            else{
                cout<<"cannot dispense remaining amount of "<<rem<<" rupees"<<endl;
            }
        }

    }
};

class HundredRupeeHandler:public MoneyHandler{
    protected:
    int notes;
    public:
    HundredRupeeHandler(int notes){
        this->notes=notes;
    }


    void dispense(int amount) override{
        int required_notes=amount/100;
        if(required_notes<notes){
            notes-=required_notes;
        }
        else{
            required_notes=notes;
            notes=0;
        }
        if(required_notes>0){
            cout<<" dispensing "<<required_notes<<" of 100 rupees "<<endl;
        }
        int rem=amount-(100*required_notes);
        
        if(rem>0){
            if(nextHandler!=nullptr)
            nextHandler->dispense(rem);
            else{
                cout<<"cannot dispense remaining amount of "<<rem<<" rupees"<<endl;
            }
        }

    }
};


int main(){
    MoneyHandler* thousandRupeeHandler=new ThousandRupeeHandler(3);
    MoneyHandler* fiveHundredRupeeHandler=new FiveHundredRupeeHandler(6);
    MoneyHandler* hundredRupeeHandler=new HundredRupeeHandler(20);

    thousandRupeeHandler->setNextHandler(fiveHundredRupeeHandler);
    fiveHundredRupeeHandler->setNextHandler(hundredRupeeHandler);
    hundredRupeeHandler->setNextHandler(nullptr);

    int amount =1240;
    cout<<"starting to dispense "<<amount<<endl;
    thousandRupeeHandler->dispense(amount);

    return 0;

}



