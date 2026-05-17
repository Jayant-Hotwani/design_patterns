#include <bits/stdc++.h>
using namespace std;

/*
========================================================
🚀 PAYMENT GATEWAY SYSTEM (FAANG-LEVEL LLD + HLD)
========================================================

🎯 GOAL:
Design a payment system that is:
- Idempotent (no double payments)
- Reliable (retry + backoff)
- Consistent (rollback on failure)
- Observable (audit logging)

--------------------------------------------------------
🧠 HIGH LEVEL DESIGN (HLD)

Client → Controller → PaymentService
                        ↓
                 Idempotency Store
                        ↓
                 Payment Orchestrator
                        ↓
              Gateway (Strategy Pattern)
                        ↓
                 Banking System
                        ↓
                 Audit Logging

--------------------------------------------------------
🔥 KEY CONCEPTS

1️⃣ IDEMPOTENCY
- Prevent duplicate payments using unique idempotency key
- Same request → same result (no double charge)

2️⃣ STRATEGY PATTERN
- Different payment gateways (Razorpay, Paytm)

3️⃣ FACTORY PATTERN
- Create gateway dynamically

4️⃣ TEMPLATE METHOD
- Standard payment flow: validate → pay → confirm

5️⃣ PROXY (Retry)
- Handles retries with exponential backoff

6️⃣ ROLLBACK (COMPENSATION)
- If payment fails after deduction → refund

7️⃣ AUDIT LOGGING
- Track every step for debugging + compliance

--------------------------------------------------------
🎯 INTERVIEW ONE-LINER

“This system ensures exactly-once payment using idempotency,
handles failures via retries and rollback, and logs all events
for observability and compliance.”

========================================================
*/


// ============================
// PAYMENT REQUEST
// ============================
class PaymentRequest {
public:
    string id;        // idempotency key
    string sender;
    string receiver;
    string currency;
    double amount;

    PaymentRequest(string id, string s, string r, string c, double a)
        : id(id), sender(s), receiver(r), currency(c), amount(a) {}
};


// ============================
// IDEMPOTENCY STORE
// ============================
class IdempotencyStore {
    unordered_map<string, bool> store;

public:
    bool exists(const string& key) {
        return store.count(key);
    }

    bool get(const string& key) {
        return store[key];
    }

    void save(const string& key, bool result) {
        store[key] = result;
    }
};


// ============================
// AUDIT SERVICE
// ============================
class AuditService {
public:
    void log(const string& txnId, const string& status) {
        cout << "[AUDIT] " << txnId << " → " << status << endl;
    }
};


// ============================
// BANKING SYSTEM (STRATEGY)
// ============================
class IBankingSystem {
public:
    virtual bool process(double amount) = 0;
    virtual ~IBankingSystem() = default;
};

class RazorpayBank : public IBankingSystem {
public:
    bool process(double amount) override {
        cout << "Razorpay processing: " << amount << endl;

        // simulate randomness
        return rand() % 100 < 80;
    }
};


// ============================
// PAYMENT GATEWAY (TEMPLATE)
// ============================
class IPaymentGateway {
public:
    virtual bool processPayment(const PaymentRequest& req) {
        if (!validate(req)) return false;
        if (!pay(req)) return false;
        return confirm(req);
    }

    virtual bool validate(const PaymentRequest&) = 0;
    virtual bool pay(const PaymentRequest&) = 0;
    virtual bool confirm(const PaymentRequest&) = 0;

    virtual ~IPaymentGateway() = default;
};


// ============================
// CONCRETE GATEWAY
// ============================
class RazorpayGateway : public IPaymentGateway {
    unique_ptr<IBankingSystem> bank;

public:
    RazorpayGateway() {
        bank = make_unique<RazorpayBank>();
    }

    bool validate(const PaymentRequest& req) override {
        cout << "Validating payment..." << endl;
        return req.amount > 0;
    }

    bool pay(const PaymentRequest& req) override {
        cout << "Initiating payment..." << endl;
        return bank->process(req.amount);
    }

    bool confirm(const PaymentRequest&) override {
        cout << "Confirming payment..." << endl;
        return true;
    }
};


// ============================
// PROXY (RETRY LOGIC)
// ============================
class RetryGateway : public IPaymentGateway {
    unique_ptr<IPaymentGateway> realGateway;
    int retries;

public:
    RetryGateway(unique_ptr<IPaymentGateway> g, int r)
        : realGateway(move(g)), retries(r) {}

    bool processPayment(const PaymentRequest& req) override {
        for (int i = 0; i < retries; i++) {
            cout << "Attempt " << i + 1 << endl;
            if (realGateway->processPayment(req)) return true;

            // exponential backoff simulation
            cout << "Retrying..." << endl;
        }
        return false;
    }

    bool validate(const PaymentRequest& r) override {
        return realGateway->validate(r);
    }

    bool pay(const PaymentRequest& r) override {
        return realGateway->pay(r);
    }

    bool confirm(const PaymentRequest& r) override {
        return realGateway->confirm(r);
    }
};


// ============================
// FACTORY
// ============================
enum class GatewayType { RAZORPAY };

class GatewayFactory {
public:
    static unique_ptr<IPaymentGateway> create(GatewayType type) {
        if (type == GatewayType::RAZORPAY) {
            return make_unique<RetryGateway>(
                make_unique<RazorpayGateway>(), 3
            );
        }
        return nullptr;
    }
};


// ============================
// PAYMENT SERVICE (CORE)
// ============================
class PaymentService {
    IdempotencyStore idempotency;
    AuditService audit;

public:
    static PaymentService& getInstance() {
        static PaymentService instance;
        return instance;
    }

    bool process(GatewayType type, const PaymentRequest& req) {

        // 🔥 IDEMPOTENCY CHECK
        if (idempotency.exists(req.id)) {
            cout << "Duplicate request detected\n";
            return idempotency.get(req.id);
        }

        audit.log(req.id, "STARTED");

        auto gateway = GatewayFactory::create(type);

        bool success = gateway->processPayment(req);

        if (!success) {
            rollback(req);
            audit.log(req.id, "FAILED");
        } else {
            audit.log(req.id, "SUCCESS");
        }

        idempotency.save(req.id, success);

        return success;
    }

    // 🔥 ROLLBACK (COMPENSATION)
    void rollback(const PaymentRequest& req) {
        cout << "Refund issued for amount: " << req.amount << endl;
    }
};


// ============================
// CONTROLLER
// ============================
class PaymentController {
public:
    bool handle(GatewayType type, const PaymentRequest& req) {
        return PaymentService::getInstance().process(type, req);
    }
};


// ============================
// MAIN
// ============================
int main() {
    srand(time(0)); // seed for randomness

    PaymentController controller;

    PaymentRequest req1("txn_123", "Jayant", "Rajesh", "INR", 100);

    bool result1 = controller.handle(GatewayType::RAZORPAY, req1);
    cout << (result1 ? "SUCCESS\n" : "FAILED\n");

    // 🔥 Duplicate request (Idempotency check)
    bool result2 = controller.handle(GatewayType::RAZORPAY, req1);
    cout << (result2 ? "SUCCESS\n" : "FAILED\n");

    return 0;
}