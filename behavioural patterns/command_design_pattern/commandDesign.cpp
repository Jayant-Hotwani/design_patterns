#include <bits/stdc++.h>
using namespace std;

// command interface 
class command{
    public:
    virtual void execute()=0;
    virtual void undo()=0;
};

// concrete reciervers 

class light{

    public:
    void turnOn(){
        cout<<"light turned on"<<endl;
    }
     void turnOff(){
        cout<<"light turned off"<<endl;
    }
};

class fan{

    public:
    void turnOn(){
        cout<<"fan turned on"<<endl;
    }
     void turnOff(){
        cout<<"fan turned off"<<endl;
    }
};


// concrete commands 
class lightCommand : public command{
    light* lightObj;
public:
    lightCommand(light* l){
        this->lightObj = l;
    }

    void execute(){
        lightObj->turnOn();
    }
    void undo(){
        lightObj->turnOff();
    }
};

class FanCommand : public command{
    fan* fanObj;
    public:
    FanCommand (fan* f){
        this->fanObj=f;

    }
    void execute(){
        fanObj->turnOn();
    }
    void undo() {
        fanObj->turnOff();
    }
};


// sender / client 

class RemoteControl{
    public:
    static const int btn=4;
    command* btns[btn];
    bool pressed[btn];
    
    RemoteControl(){
        for(int i=0;i<btn;i++){
            this->btns[i]=nullptr;
            this->pressed[i]=false;
        }
    }

    void setCommand(command* c , int index){
        if (index>=0 && index<btn){
            if (btns[index]!=nullptr){
                delete btns[index];
            }
            btns[index]=c;
            pressed[index]=false;
        }
        else{
            cout<<"cant sent command , index out of scope"<<endl;
        }
    }


    void pressbutton(int index){
        if (index>=0 && index<btn){
            if (btns[index]!=nullptr){
                if(pressed[index]){
                    btns[index]->undo();
                }
                else{
                     btns[index]->execute();
                }

                pressed[index]=!pressed[index];
            }
            else{
                cout<<"command not set "<<endl;
            }
        }
        else{
            cout<<"cant sent command , index out of scope"<<endl;
        }
    }

};


int main(){
    RemoteControl remote ;

    light* Light;
    fan* Fan;

    lightCommand* LightCommand = new lightCommand(Light);
    FanCommand* fanCommand = new FanCommand(Fan);

    remote.setCommand(LightCommand,0);
    remote.setCommand(fanCommand,1);

    remote.pressbutton(0);
     remote.pressbutton(1);
 remote.pressbutton(0); 
 remote.pressbutton(1);

}