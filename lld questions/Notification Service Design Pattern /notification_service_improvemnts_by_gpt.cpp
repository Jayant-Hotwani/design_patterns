#include <bits/stdc++.h>
using namespace std;

/*
========================================================
PRODUCTION-GRADE NOTIFICATION SYSTEM
Patterns:
- Decorator
- Observer (push model)
- Strategy
- Singleton (thread-safe)
========================================================
*/


// ============================
// Notification Interface
// ============================
class INotification {
public:
    virtual string getContent() const = 0;
    virtual ~INotification() = default;
};


// ============================
// Concrete Notification
// ============================
class SimpleNotification : public INotification {
    string text;

public:
    explicit SimpleNotification(string t) : text(move(t)) {}

    string getContent() const override {
        return text;
    }
};


// ============================
// Decorator Base
// ============================
class NotificationDecorator : public INotification {
protected:
    shared_ptr<INotification> notification;

public:
    explicit NotificationDecorator(shared_ptr<INotification> n)
        : notification(move(n)) {}

    virtual ~NotificationDecorator() = default;
};


// Timestamp Decorator
class TimestampDecorator : public NotificationDecorator {
public:
    using NotificationDecorator::NotificationDecorator;

    string getContent() const override {
        auto now = chrono::system_clock::now();
        time_t now_c = chrono::system_clock::to_time_t(now);

        return notification->getContent() +
               " | received at: " + string(ctime(&now_c));
    }
};


// Signature Decorator
class SignatureDecorator : public NotificationDecorator {
public:
    using NotificationDecorator::NotificationDecorator;

    string getContent() const override {
        return notification->getContent() +
               " | sent by Jayant";
    }
};


// ============================
// Observer Pattern (Push Model)
// ============================

class IObserver {
public:
    virtual void update(const string& content) = 0;
    virtual ~IObserver() = default;
};


class IObservable {
public:
    virtual void addObserver(shared_ptr<IObserver>) = 0;
    virtual void removeObserver(shared_ptr<IObserver>) = 0;
    virtual void notifyObservers(const string&) = 0;
    virtual ~IObservable() = default;
};


// Concrete Observable
class NotificationObservable : public IObservable {
    vector<weak_ptr<IObserver>> observers;

public:
    void addObserver(shared_ptr<IObserver> observer) override {
        observers.push_back(observer);
    }

    void removeObserver(shared_ptr<IObserver> observer) override {
        observers.erase(
            remove_if(observers.begin(), observers.end(),
                [&](weak_ptr<IObserver>& wp) {
                    auto sp = wp.lock();
                    return !sp || sp == observer;
                }),
            observers.end()
        );
    }

    void notifyObservers(const string& content) override {
        for (auto it = observers.begin(); it != observers.end();) {
            if (auto obs = it->lock()) {
                obs->update(content);
                ++it;
            } else {
                it = observers.erase(it); // clean expired
            }
        }
    }
};


// ============================
// Strategy Pattern
// ============================
class INotificationStrategy {
public:
    virtual void send(const string& content) = 0;
    virtual ~INotificationStrategy() = default;
};


class EmailStrategy : public INotificationStrategy {
    string email;

public:
    explicit EmailStrategy(string e) : email(move(e)) {}

    void send(const string& content) override {
        cout << "[Email to " << email << "]\n" << content << "\n";
    }
};


class SMSStrategy : public INotificationStrategy {
    string phone;

public:
    explicit SMSStrategy(string p) : phone(move(p)) {}

    void send(const string& content) override {
        cout << "[SMS to " << phone << "]\n" << content << "\n";
    }
};


// ============================
// Observer Implementations
// ============================

class LoggerObserver : public IObserver {
public:
    void update(const string& content) override {
        cout << "[LOG] " << content << endl;
    }
};


class NotificationEngine : public IObserver {
    vector<shared_ptr<INotificationStrategy>> strategies;

public:
    void addStrategy(shared_ptr<INotificationStrategy> s) {
        strategies.push_back(s);
    }

    void update(const string& content) override {
        for (auto& s : strategies) {
            s->send(content);
        }
    }
};


// ============================
// Singleton Service
// ============================

class NotificationService {
    shared_ptr<NotificationObservable> observable;

    NotificationService() {
        observable = make_shared<NotificationObservable>();
    }

public:
    static NotificationService& getInstance() {
        static NotificationService instance; // thread-safe
        return instance;
    }

    shared_ptr<NotificationObservable> getObservable() {
        return observable;
    }

    void send(shared_ptr<INotification> notification) {
        observable->notifyObservers(notification->getContent());
    }

    // delete copy
    NotificationService(const NotificationService&) = delete;
    NotificationService& operator=(const NotificationService&) = delete;
};


// ============================
// MAIN
// ============================

int main() {

    auto& service = NotificationService::getInstance();
    auto observable = service.getObservable();

    // Observers
    auto logger = make_shared<LoggerObserver>();
    auto engine = make_shared<NotificationEngine>();

    engine->addStrategy(make_shared<EmailStrategy>("jayant@gmail.com"));
    engine->addStrategy(make_shared<SMSStrategy>("9999999999"));

    observable->addObserver(logger);
    observable->addObserver(engine);

    // Create notification
    shared_ptr<INotification> notif =
        make_shared<SimpleNotification>("Order shipped!");

    notif = make_shared<TimestampDecorator>(notif);
    notif = make_shared<SignatureDecorator>(notif);

    service.send(notif);

    return 0;
}