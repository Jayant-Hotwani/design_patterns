#include <bits/stdc++.h>
using namespace std;



class User;

class IMediator{

    public:
    virtual void addUsers(User* user)=0;
    virtual void sendAll(User* user,string& msg)=0;
    virtual void send(User* from,User* to ,string& msg)=0;
};

class User{
    public:
    string name;
    IMediator* mediator;
    User(string name , IMediator* med){
        this->name=name;
        this->mediator=med;
        mediator->addUsers(this);
    }

    void sendAll(string msg){

        mediator->sendAll(this,msg);
    }
    void send(User* u, string msg){
        mediator->send(this,u,msg);
    }
    void recieve(User* user,string& str){
        cout<<this->name<<" recieved message from "<<user->name<<endl;
    }
};

class ChatMediator:public IMediator{
    public:
    vector<User*>users;
    void addUsers(User* u){
        users.push_back(u);
    }
    void sendAll(User* user,string& msg){
        cout<<user->name<<" brodcasted "<<endl;
        for(auto x:users){
            if (x!=user){
                x->recieve(user,msg);
            }
        }
    }

    void send(User* from,User* to ,string& msg){
        cout<<from->name<<" sends msg to  "<<to->name<<endl;
        for(auto x:users){
            if (x==to){
                x->recieve(from,msg);
            }
        }
    }
};

int main(){
    IMediator* chatMediator = new ChatMediator();
    User* u1=new User("jayant",chatMediator);
    User* u2=new User("rishabh",chatMediator);
    User* u3=new User("shivam",chatMediator);

    u1->sendAll("hello_guys");
    u1->send(u2,"hello_rishabh");

    return 0;
}


