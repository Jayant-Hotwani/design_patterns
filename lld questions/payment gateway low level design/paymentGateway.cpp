#include <bits/stdc++.h>
using namespace std;



class PaymentRequest{
    public:
    string sender;
    string reciever;
    string currency;
    double amount;
    PaymentRequest(string sender, string reciever , string currency , double amount){
        this->amount=amount;
        this->currency=currency;
        this->reciever=reciever;
        this->sender=sender;
    }
};

// abstarct class for banking systems 

class IBankingSystem{
    public:
    IBankingSystem(){};
    virtual bool processPayment(double amount)=0;
    virtual ~IBankingSystem(){};

};

// concrete implementations for banking systems 

class PaytmBankingSystem:public IBankingSystem{
    public:
    PaytmBankingSystem(){};
    bool processPayment(double amount){
        cout<<" PaytmBankingSystem processing the amount of "<<amount<<endl;
        // business logic 
        int success=rand()%100;
        return success<10;
    }
    virtual ~PaytmBankingSystem(){};
};

class RazorPayBankingSystem:public IBankingSystem{
    public:
    RazorPayBankingSystem(){};
    bool processPayment(double amount){
        cout<<" RazorPayBankingSystem processing the amount of "<<amount<<endl;
        // business logic 
        int success=rand()%100;
        return success<90;
    }
    ~RazorPayBankingSystem(){};
};


class IPaymentGateway{
    protected:
    IBankingSystem* bankingSystem;
    public:
    IPaymentGateway(){
        bankingSystem=nullptr;
    }
    PaymentRequest* paymentRequest;
    //template design pattern 
    virtual bool processPayment(PaymentRequest* paymentRequest){
        if (validatePayment(paymentRequest)==false){
            cout<<" payment validation step falied for user " <<paymentRequest->sender<<endl;
            return false;
        }
        if (initiatePayment(paymentRequest)==false){
            cout<<" payment initiation step falied for user " <<paymentRequest->sender<<endl;
            return false;
        }
        if (confirmPayment(paymentRequest)==false){
            cout<<" payment confirmation step falied for user " <<paymentRequest->sender<<endl;
            return false;
        }
        return true;
    };
    virtual bool validatePayment(PaymentRequest* paymentRequest)=0;
    virtual bool initiatePayment(PaymentRequest* paymentRequest)=0;
    virtual bool confirmPayment(PaymentRequest* paymentRequest)=0;
    virtual ~IPaymentGateway(){
        delete bankingSystem;
        delete paymentRequest;
    };
};

class PaymentGatewayProxy:public IPaymentGateway{
    protected:
    IPaymentGateway* realPaymentGateway;
    int NumberOfRetry=0;
    public:
    PaymentGatewayProxy(IPaymentGateway* realPaymentGateway,int NumberOfRetry =3 )
    {
        this->NumberOfRetry=NumberOfRetry;
        this->realPaymentGateway=realPaymentGateway;
    }
    ~PaymentGatewayProxy(){
        delete realPaymentGateway;
    }
    bool processPayment(PaymentRequest* paymentRequest) override{
        if (realPaymentGateway->processPayment(paymentRequest)==true)return true;
        for(int attempts=0;attempts<NumberOfRetry;attempts++){
            cout<<" retrying payment .. this is our attempt number "<<attempts+1<<endl;
            if (realPaymentGateway->processPayment(paymentRequest)==true)return true;
        }
        cout<<" max limit of retries reached .. now try again after 24 hours "<<endl;
        return false;
    }

    bool validatePayment(PaymentRequest* paymentRequest) override{
        return realPaymentGateway->validatePayment(paymentRequest);
    }
    bool initiatePayment(PaymentRequest* paymentRequest) override{
        return realPaymentGateway->initiatePayment(paymentRequest);
    }
    bool confirmPayment(PaymentRequest* paymentRequest) override{
        return realPaymentGateway->confirmPayment(paymentRequest);
    }
};


enum class GatewayType{
    PAYTM,
    RAZORPAY
};





class PaytmPaymentGateway:public IPaymentGateway{
    public:
    PaytmPaymentGateway(){
        bankingSystem=new PaytmBankingSystem();
    }
    bool validatePayment(PaymentRequest* paymentRequest) override{
        cout<<" paytm validating payment for "<<paymentRequest->sender<<endl;
        if (paymentRequest->currency!="INR" || paymentRequest->amount<= 0){
            return false;
        }
        return true;
    }

    bool initiatePayment (PaymentRequest* paymentRequest) override{
        cout<<" paytm initiating payment for "<<paymentRequest->sender<<endl;
        return bankingSystem->processPayment(paymentRequest->amount);
    }

    bool confirmPayment (PaymentRequest* paymentRequest) override{
        cout<<" paytm confirming payment for "<<paymentRequest->sender<<endl;
        return true;
    }
};

class RazorPayPaymentGateway:public IPaymentGateway{
    public:
    RazorPayPaymentGateway(){
        bankingSystem=new RazorPayBankingSystem();
    }
    bool validatePayment(PaymentRequest* paymentRequest) override{
        cout<<" RazorPay validating payment for "<<paymentRequest->sender<<endl;
        if (paymentRequest->amount<= 0){
            return false;
        }
        return true;
    }

    bool initiatePayment (PaymentRequest* paymentRequest) override{
        cout<<" RazorPay initiating payment for "<<paymentRequest->sender<<endl;
        return bankingSystem->processPayment(paymentRequest->amount);
    }

    bool confirmPayment (PaymentRequest* paymentRequest) override{
        cout<<" RazorPay confirming payment for "<<paymentRequest->sender<<endl;
        return true;
    }
};

// creating gateway factory to return type of gateways
// lets make this class singleton

class GatewayFactory{
    private :
    static GatewayFactory* gatewayInstance;
    GatewayFactory(){};
    GatewayFactory(const GatewayFactory& )=delete;
    GatewayFactory& operator=(const GatewayFactory& )=delete;
    public:

    static GatewayFactory* getInstance(){
        if (gatewayInstance==nullptr)
        {
            gatewayInstance=new GatewayFactory();
        }
        return gatewayInstance;
    }

    IPaymentGateway* getPaymentGateway(GatewayType type){
        if (type==GatewayType::PAYTM){
            PaytmPaymentGateway* paytmPaymentGateway=new PaytmPaymentGateway();
            return new PaymentGatewayProxy(paytmPaymentGateway,3);
        }
        else{
            RazorPayPaymentGateway* RazorpayPaymentGateway=new RazorPayPaymentGateway();
            return new PaymentGatewayProxy(RazorpayPaymentGateway,5);
        }
    }

};
GatewayFactory* GatewayFactory::gatewayInstance=nullptr;
// create payment service


class PaymentService{
    private:
    static PaymentService* paymentServiceInstance;
    IPaymentGateway* paymentGateway;
    PaymentService(){
        paymentServiceInstance=nullptr;
    };
    
    ~PaymentService(){
        delete paymentGateway;
    };

    PaymentService& operator=(const PaymentService& obj)=delete;
    PaymentService(const PaymentService& obj)=delete;

    public:

    static PaymentService* getInstance(){
        if(paymentServiceInstance==nullptr){
            paymentServiceInstance=new PaymentService();
        }
        return paymentServiceInstance;
    }

    void setGateway(IPaymentGateway* paymentGateway){
        this->paymentGateway=paymentGateway;
    }

    bool processPayment(PaymentRequest* pr){
        return paymentGateway->processPayment(pr);
    }

};

PaymentService* PaymentService::paymentServiceInstance=nullptr;


// singleton Controller class for all client requests 
class PaymentController{
    protected:
    PaymentService* paymentService =PaymentService::getInstance();
    static PaymentController* paymentControllerInstance;
    private:
    PaymentController(){
        paymentControllerInstance=nullptr;
    };
    
    PaymentController(const PaymentController& )=delete;
    PaymentController& operator=(const PaymentController& )=delete;

    public:
    static PaymentController* getInstance(){
        if (paymentControllerInstance == nullptr) {
            paymentControllerInstance = new PaymentController();
        }
        return paymentControllerInstance;
    }

    bool handlePayment(GatewayType type ,PaymentRequest* pr){

        IPaymentGateway* pg=GatewayFactory::getInstance()->getPaymentGateway(type);
        paymentService->setGateway(pg);
        return paymentService->processPayment(pr);
    }

};


PaymentController* PaymentController:: paymentControllerInstance=nullptr;




int main(){
    srand(time(0));
    PaymentRequest* pr1=new PaymentRequest("JAYANT","RAJESH","INR",100);
    bool res1=PaymentController::getInstance()->handlePayment(GatewayType::PAYTM,pr1);

    
    return 0;
}