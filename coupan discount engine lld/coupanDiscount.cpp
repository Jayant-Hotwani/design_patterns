#include <bits/stdc++.h>
using namespace std;


// intreface for discount strategy

class DiscountStrategy{

    public:
    virtual double calculateDiscount(double baseAmount)=0;
    virtual ~DiscountStrategy(){}
};

// implementing concrete classes for this dicount strtategy


class flatDiscountStrategy:public DiscountStrategy{
    double amount;
    public:
    flatDiscountStrategy(double amount){
        this->amount=amount;
    }
    double calculateDiscount(double baseAmount) override{
        return min(amount,baseAmount);
    }
};

class PercentageStrategy:public DiscountStrategy{
    double percentage;
    public:
    PercentageStrategy(double percentage){
        this->percentage=percentage;
    }
    double calculateDiscount(double baseAmount) override{
        double discount=(baseAmount*percentage)/100;
        return min(baseAmount,discount);
    }
};

class PercentageWithCapStrategy:public DiscountStrategy{
    double percentage;
    double cap;
    public:
    PercentageWithCapStrategy(double percentage , double cap){
        this->percentage=percentage;
        this->cap=cap;
    }
    double calculateDiscount(double baseAmount) override{
        double discount=(baseAmount*percentage)/100;
        return min(cap,discount);
    }
};

// creating enum for different types of discount strategy

enum StrategyType{
    FLAT,
    PERCENT,
    PERCENT_WITH_CAP
};

// creating singleton manager for discount strategy 

class DiscountStrategyManager{
    private:
    static DiscountStrategyManager* instance;
    DiscountStrategyManager(){};
    DiscountStrategyManager(const DiscountStrategyManager&)=delete;
    DiscountStrategyManager& operator=(const DiscountStrategyManager&)=delete;
    public:

    static DiscountStrategyManager* getInstance(){
        if (instance==nullptr)
        instance=new DiscountStrategyManager();

        return instance;
    }

    DiscountStrategy* getStrategy(StrategyType st,double param1 , double param2=0.0){
        if (st==StrategyType::FLAT){
            return new flatDiscountStrategy(param1);
        }
        if (st==StrategyType::PERCENT){
            return new PercentageStrategy(param1);
        }
        if (st==StrategyType::PERCENT_WITH_CAP){
            return new PercentageWithCapStrategy(param1,param2);
        }
        return nullptr;
    }
};
DiscountStrategyManager* DiscountStrategyManager :: instance =nullptr;



// creating product class 

class Product{
    string name;
    string category;
    double price;
    public:
    Product(string name,string category,double price){
        this->category=category;
        this->name=name;
        this->price=price;
    }

    string getName(){
        return name;
    }
    string getCategory(){
        return category;
    }
    double getPrice(){
        return price;
    }

};


// creating class cartItem


class CartItem{
    Product* product;
    int quantity=0;
    public:
    CartItem(Product* product , int quantity){
        this->product=product;
        this->quantity=quantity;
    }
    double getTotal(){
        return product->getPrice() *quantity; 
    }

    Product* getProduct(){
        return product;
    }
};


class Cart{
    private:
    vector<CartItem*>items;
    bool isLoyaltyMemeber;
    double originalTotal;
    double currentTotal;
    string paymentBank;
    public:
    Cart(){
        originalTotal=0.0;
        currentTotal=0.0;
        paymentBank="";
        isLoyaltyMemeber=false;
    }

    void addProduct(Product* p , int qty){
        CartItem* ct=new CartItem(p,qty);
        items.push_back(ct);
        originalTotal+= ct->getTotal();
        currentTotal+=ct->getTotal();
    }

    double getOriginalTotal(){
        return originalTotal;
    }
    double getCurrentTotal(){
        return currentTotal;
    }

    void applyDiscount(double discountAmount){
        currentTotal-=discountAmount;
        if (currentTotal<0)currentTotal=0;
    }

    void setLoyaltyMember(bool member){
        isLoyaltyMemeber=member;
    }
    bool getLoyaltyMember(){
        return isLoyaltyMemeber;
    }

    void setPaymentBank(string bank){
        this->paymentBank=bank;
    }
    string getPaymentBank(){
        return paymentBank;
    }

    vector<CartItem*> getItems(){
        return items;
    }
};


// creating abstarct coupan class implemnenting chain of responsiblity 


class Coupan{
    private:
    Coupan* next;
    public:
    Coupan(){
        next=nullptr;
    }
    virtual ~Coupan(){
        if (next){
            delete next;
        }
    }
    void setNext(Coupan* c){
        this->next=c;
    }
    Coupan* getNext(){
        return next;
    }

    // template design pattern 
    void applyDiscount(Cart* cart){
        if (isApplicable(cart)){
            double discount=getDiscount(cart);
            cart->applyDiscount(discount);
            cout<<name()<<" applied " <<discount<<endl;
            if (!isCombinable()){
                return ;
            }
            if (next){
                next->applyDiscount(cart);
            }
        }
    }

    virtual bool isApplicable(Cart* cart)=0;
    virtual bool isCombinable(){
        return true;
    }
    virtual double getDiscount(Cart* cart)=0;
    virtual string name()=0;
};


// implementing concrete coupans 

class SeasonalCoupan:public Coupan{
    double percentage;
    string category;
    DiscountStrategy* ds;
    public:
    SeasonalCoupan(double percentage,string category){
        this->category=category;
        this->percentage=percentage;
        ds=DiscountStrategyManager::getInstance()->getStrategy(StrategyType::PERCENT,percentage);
    }

    ~SeasonalCoupan(){
        delete ds;
    }

    bool isApplicable(Cart* cart)override{
        for(auto x:cart->getItems()){
            if (x->getProduct()->getCategory()==category)return true;
        }
        return false;
    }

    double getDiscount(Cart* cart)override{
        double subtotal=0.0;
         for(auto x:cart->getItems()){
            if (x->getProduct()->getCategory()==category){
                subtotal+=x->getTotal();
            }
        }

        return ds->calculateDiscount(subtotal);
    }

    bool isCombinable()override
    {
        return true;
    }
    string name() override{
        return "Seasonal offer on category " +  category + " applied .. ";
    }

};

class LoyaltyCoupan:public Coupan{
    double percentage;
    DiscountStrategy* ds;
    public:
    LoyaltyCoupan(double percentage){
        this->percentage=percentage;
        ds=DiscountStrategyManager::getInstance()->getStrategy(StrategyType::PERCENT,percentage);
    }

    ~LoyaltyCoupan(){
        delete ds;
    }

    bool isApplicable(Cart* cart)override{
        return cart->getLoyaltyMember();
    }

    double getDiscount(Cart* cart)override{
        return ds->calculateDiscount(cart->getCurrentTotal());
    }
    bool isCombinable()override
    {
        return true;
    }
    string name() override{
        return "loyalty offer applied .. ";
    }
};

class BulkPurchaseCoupan:public Coupan{
    double flatOff;
    DiscountStrategy* ds;
    double threshold;
    public:
    BulkPurchaseCoupan(double flatOff, double threshhold){
        this->flatOff=flatOff;
        this->threshold=threshhold;
        ds=DiscountStrategyManager::getInstance()->getStrategy(StrategyType::FLAT,flatOff);
    }
    ~BulkPurchaseCoupan(){
        delete ds;
    }

    bool isApplicable(Cart* c)override{
        return c->getOriginalTotal()>=threshold; 
    }

    double getDiscount(Cart* c) override{
        return ds->calculateDiscount(c->getCurrentTotal());
    }

    string name() override{
        return "buld purchase coupan applied .. ";
    }
};


// creating coupan manager singleton class 

class CoupanManager{
    static CoupanManager* instance;
    Coupan* head;
    mutable mutex mtx;
    CoupanManager(){
        head=nullptr;
    }

    CoupanManager (const CoupanManager&)=delete;
    CoupanManager& operator=(const CoupanManager&)=delete;

    public:
    static CoupanManager* getInstance(){
        if (instance==nullptr)
        instance=new CoupanManager();

        return instance;
    }

    void registerCoupan(Coupan* c){
        lock_guard<mutex>lock(mtx);
        if(!head)
        head=c;
        else{
            Coupan* cur=head;
            while(cur->getNext()!=nullptr){
                cur=cur->getNext();
            }
            cur->setNext(c);
        }
    }

    vector<string> getApplicableCoupans(Cart* cart){
        lock_guard<mutex>lock(mtx);
        vector<string>res;
        Coupan* curr=head;
        while(curr!=nullptr){
            if (curr->isApplicable(cart))
            res.push_back(curr->name());

            curr=curr->getNext();
        }
        return res;
    }

    double ApplyAllCoupans(Cart* cart){
        lock_guard<mutex>lock(mtx);
        if(head){
            head->applyDiscount(cart);
        }
        return cart->getCurrentTotal();
    }
};

CoupanManager* CoupanManager::instance=nullptr;

int main(){

    Product* p1=new Product("winter jacket","clothing",1000.0);
    Product* p2=new Product("summer jacket","clothing",1000.0);
    Product* p3=new Product("smartphone","electronics",10000.0);

    Cart* c=new Cart();
    c->addProduct(p1,2);
    c->addProduct(p2,2);
    c->addProduct(p3,1);
    c->setLoyaltyMember(true);


    cout<<"original total of cart is "<<c->getOriginalTotal()<<endl;

    CoupanManager* mgr=CoupanManager::getInstance();
    mgr->registerCoupan(new SeasonalCoupan(10,"clothing"));
    mgr->registerCoupan(new LoyaltyCoupan(5));
    mgr->registerCoupan(new BulkPurchaseCoupan(100,10000));



    vector<string>applicable=mgr->getApplicableCoupans(c);
    cout<<"applicable copupans are. below "<<endl;
    for(auto x:applicable){
        cout<<x<<endl;
    }

    double finalTotal=mgr->ApplyAllCoupans(c);
    cout<<"final total after applying all coupans is "<<finalTotal<<endl;


    return 0;
}