#include <bits/stdc++.h>
using namespace std;


// creating an abstract notification class 

class Inotification{

    public:
    Inotification(){}
    virtual string getContent()=0;
    virtual ~Inotification(){}
};

// concrete notification

class SimpleNotification:public Inotification{
    string text;
    public:
    SimpleNotification(string text){
        this->text=text;
    }
    string getContent() override {
        return text;
    }
};


// functional requirement to add behavior dynamically 
// using decorator design pattern to do so 


// abstarct decorator
class InotificationDecorator:public Inotification{
    protected:
    Inotification* notification;
    public:
    InotificationDecorator(Inotification* noti){
        this->notification=noti;
    }

    virtual ~InotificationDecorator(){
        delete notification;
    }
};

// concrete decorators 

class TimestampDecorator:public InotificationDecorator{
    public:
    TimestampDecorator(Inotification* noti):InotificationDecorator(noti){

    }
    string getContent() override{
        auto now = std::chrono::system_clock::now();
        // Convert to a time_t object for easy printing
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        // Display in human-readable format
        return "notification with text "+ notification->getContent() + "recieved at " + std::ctime(&now_c);
    }
};


class SignatureDecorator:public InotificationDecorator{
    public:
    SignatureDecorator(Inotification* noti):InotificationDecorator(noti){

    }
    string getContent() override{
        return notification->getContent()+ " -> sent by jayant hotwani ... " ;
    }
};



// now creating components for observer design pattern 
// subject == observable == Iobservable 


// abstarct observer  
class Iobserver{
    public:
    virtual void update()=0;
};

// abstract observervable  
class Iobservable{
    public:
    virtual void addObserver(Iobserver* )=0;
    virtual void removeObserver(Iobserver* )=0;
    virtual void notifyObserver()=0;
};

// concrete observable 
class NotificationObservable:public Iobservable{
    vector<Iobserver*>observers;
    Inotification* currentNotification;
    public:
    NotificationObservable(){
        this->currentNotification=nullptr;
    }
    void addObserver(Iobserver* observer) override{
        observers.push_back(observer);
    }
    void removeObserver(Iobserver* observer) override{
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }
    void notifyObserver() override{
        for( int i=0;i<observers.size();i++){
            observers[i]->update();
        }
    }

    Inotification* getNotification(){
        return currentNotification;
    }

    void setNotification(Inotification* noti){
        if (currentNotification!=nullptr){
            delete currentNotification;
        }
        currentNotification=noti;
        notifyObserver();
    }

    string getNotificationContent(){
        return currentNotification->getContent();
    }

    ~NotificationObservable(){
        delete currentNotification;
    }
};

// concrete observers 

class loggerObserver:public Iobserver{
    NotificationObservable* notificationObservable;
    public:
    loggerObserver(NotificationObservable* notificationObservable){
        this->notificationObservable=notificationObservable;
    }
    void update() override{
        cout<<" logging new notification on console -> "<<notificationObservable->getNotificationContent()<<endl;
    }
};

// forward declaration for strategy deisgn pattern 

// abstract strategy 
class INotificationStrategy{
    public:
    virtual void sendNotification(string content)=0;
};

// notification engine acting as concrete observer 

class NotificationEngine:public Iobserver{
    NotificationObservable* notificationObservable;
    vector<INotificationStrategy*> stratigies;

    public:
    NotificationEngine(NotificationObservable* notificationObservable){
        this->notificationObservable=notificationObservable;
    }

    void addStrategy(INotificationStrategy* s){
        stratigies.push_back(s);
    }
     void removeStrategy(INotificationStrategy* s){
        stratigies.erase(std::remove(stratigies.begin(),stratigies.end(),s),stratigies.end());
    }

    void update() override{
        for (int i=0;i<stratigies.size();i++){
            stratigies[i]->sendNotification(notificationObservable->getNotificationContent());
        }
    }
};



// creating concrete strategies 

class emailStrategy:public INotificationStrategy{
    private:
    string emailId;
    public:
    emailStrategy(string emailId){
        this->emailId=emailId;
    }

    void sendNotification(string content) override{
        cout<<" notification recieved via email on" << emailId <<" -----  "<< endl;
        cout<<content<<endl;
    }

};

class SMSStrategy:public INotificationStrategy{
    private:
    string phoneNumber;
    public:
    SMSStrategy(string phoneNumber){
        this->phoneNumber=phoneNumber;
    }

    void sendNotification(string content) override{
        cout<<" notification recieved via SMS on" << phoneNumber <<" -----  "<< endl;
        cout<<content<<endl;
    }
};



// finally we have notification service singleton class which acts as bridge 
// bridge between creating notifications and sending notifications 
// bridge between Inotification and NotificationObservable 

class NotificationService{
    NotificationObservable* notificationObservable;
    vector<Inotification*> notifications;
    static NotificationService* notificationService;
    // private cobstructor for singleton

    NotificationService(){
        notificationObservable=new NotificationObservable();
    }

    public:

    static NotificationService* getInstance(){
        if(notificationService==nullptr){
            notificationService=new NotificationService();
        }
        return notificationService;
    }

    NotificationObservable* getObservable(){
        return notificationObservable;
    }

    void sendNotification(Inotification* n){
        notifications.push_back(n);
        notificationObservable->setNotification(n);
    }
    ~NotificationService(){
        delete notificationObservable;
    }

};

NotificationService* NotificationService::notificationService=nullptr;


int main(){
    // client code 


    // create notification service object 
    NotificationService* notificationService = NotificationService::getInstance();

    // get observable 

    NotificationObservable* observable=notificationService->getObservable();

    // creating both observer one by one 
    loggerObserver* logger= new loggerObserver(observable);
    NotificationEngine* notificationEngine=new NotificationEngine(observable);
    notificationEngine->addStrategy(new emailStrategy("jayanthotwani@gmail.com"));
    notificationEngine->addStrategy(new SMSStrategy("9863527242"));

    // attaching the observers
    observable->addObserver(notificationEngine);
    observable->addObserver(logger);


    // creating a new notification and decorating 

    Inotification* notification= new SimpleNotification(" order has been shipped .. Hurrayyy ");
    notification=new TimestampDecorator(notification);
    notification=new SignatureDecorator(notification);

    notificationService->sendNotification(notification);

    cout<<" program ended "<<endl;
    return 0;
}