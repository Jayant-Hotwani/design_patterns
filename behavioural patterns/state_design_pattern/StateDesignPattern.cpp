#include <bits/stdc++.h>
using namespace std;

class VendingMachine;
class idleState;
class hasCoinState;
class dispenseState;


class State
{
public:
    virtual void insertCoin(VendingMachine *vendingMachine) = 0;
    virtual void selectItem(VendingMachine *vendingMachine) = 0;
    virtual void dispense(VendingMachine *vendingMachine) = 0;
    virtual ~State() {};
};



class VendingMachine{
    public:
    State* IdleState;
    State* HasCoinState;
    State* DispenseState;
    State* currentState;

    VendingMachine();

    void insertCoin() {
        currentState->insertCoin(this);
    }

    void selectItem() {
        currentState->selectItem(this);
    }

    void dispense() {
        currentState->dispense(this);
    }
    void setState(State* state) {
        currentState = state;
    }


    State* getIdleState() { return IdleState; }
    State* getHasMoneyState() { return HasCoinState; }
    State* getDispensingState() { return DispenseState; }

  

};

class idleState:public State{
    public:
    void insertCoin(VendingMachine *vendingMachine) override;
    void selectItem(VendingMachine *vendingMachine) override;
    void dispense(VendingMachine *vendingMachine) override;
};

void idleState::insertCoin(VendingMachine *vendingMachine){
    cout<<"coin inserted"<<endl;
    vendingMachine->setState(vendingMachine->getHasMoneyState());
}

void idleState::selectItem(VendingMachine *vendingMachine){
    cout<<"please insert coin"<<endl;
    vendingMachine->setState(vendingMachine->getIdleState());
}

void idleState::dispense(VendingMachine *vendingMachine){
    cout<<"please insert coin"<<endl;
    vendingMachine->setState(vendingMachine->getIdleState());
}



class hasCoinState:public State{
    public:
    void insertCoin(VendingMachine *vendingMachine) override;
    void selectItem(VendingMachine *vendingMachine) override;
    void dispense(VendingMachine *vendingMachine) override;
};

void hasCoinState::insertCoin(VendingMachine *vendingMachine){
    cout<<"coin already present make selection "<<endl;
    vendingMachine->setState(vendingMachine->getHasMoneyState());
}

void hasCoinState::selectItem(VendingMachine *vendingMachine){
    cout<<"item selected ... dispensing"<<endl;
    vendingMachine->setState(vendingMachine->getDispensingState());
}

void hasCoinState::dispense(VendingMachine *vendingMachine){
    cout<<"cannot dispense .. before selecting "<<endl;
    vendingMachine->setState(vendingMachine->getHasMoneyState());
}


class dispenseState:public State{
    public:
    void insertCoin(VendingMachine *vendingMachine) override;
    void selectItem(VendingMachine *vendingMachine) override;
    void dispense(VendingMachine *vendingMachine) override;
};

void dispenseState::insertCoin(VendingMachine *vendingMachine){
     cout<<"coin already present .. dispense "<<endl;
     vendingMachine->setState(vendingMachine->getDispensingState());
}

void dispenseState::selectItem(VendingMachine *vendingMachine){
     cout<<"item already selected just.. dispense "<<endl;
     vendingMachine->setState(vendingMachine->getDispensingState());
}

void dispenseState::dispense(VendingMachine *vendingMachine){
     cout<<"dispensed item "<<endl;
     vendingMachine->setState(vendingMachine->getIdleState());
}
VendingMachine::VendingMachine() {
    IdleState = new idleState();
    HasCoinState = new hasCoinState();
    DispenseState = new dispenseState();
    currentState = IdleState;
}


int main()
{
    // Your code here
    VendingMachine vm;

    vm.selectItem();
    vm.insertCoin();
    vm.selectItem();
    vm.dispense();

    return 0;

}