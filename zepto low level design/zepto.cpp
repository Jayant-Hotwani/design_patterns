#include <bits/stdc++.h>
using namespace std;

class Product{
    int sku;
    string name;
    double price;
    public:
    Product(int id,string name , double price){
        this->sku=id;
        this->name=name;
        this->price=price;
    }

    int getSku(){
        return sku;
    }

    double getPrice(){
        return this->price;
    }

    string getName(){
        return this->name;
    }
};


class ProductFactory{
    public:
    static Product* createProduct(int sku){
        string name;
        double price;
        if (sku==101){
            name="apple";
            price=100;
        }
        else if (sku==201){
            name="mobile";
            price=10000;
        }
        else{
            name="item_"+to_string(sku);
            price=200;
        }

        return new Product(sku,name,price);
    }
};


// abstract inventory store 

class InventoryStore{
    public:
    ~InventoryStore(){};
    virtual void addProduct(Product* p , int qty)=0;
    virtual void removeProduct(int sku , int qty)=0;
    virtual int checkStock(int sku)=0;
    virtual vector<Product*> getAvailableItems()=0;
};


// concreate inventory store;

class DBInventoryStore:public InventoryStore{
    private:
    map<int,int>stock;
    map<int,Product*>products;
    public:
    DBInventoryStore(){}

    void addProduct(Product* p , int qty) override{
        int sku=p->getSku();
        if (products.find(sku)==products.end()){
            products[sku]=p;
        }
        stock[sku]+=qty;
    }

    void removeProduct(int sku , int qty) override{
        if (stock.count(sku)==0) return ;
        stock[sku]-=qty;
        if (stock[sku]<0)
        stock.erase(sku);
    }

    int checkStock(int sku) override{
        if (stock.count(sku)==0) return 0;
        return stock[sku];
    }

   
    vector<Product*> getAvailableItems() override{
        vector<Product*> ans;
        for(auto &x : stock){
            if(x.second > 0){
                ans.push_back(products[x.first]);
            }
        }
        return ans;
    }
    
};

// inventory manager 
class InventoryManager{
    InventoryStore* store;
    public:
    InventoryManager(InventoryStore* store){
        this->store=store;
    }
    void addStock(int sku , int qty){
        Product* p =ProductFactory::createProduct(sku);
        store->addProduct(p,qty);
        cout<<"Inventory Manager added stock " << sku <<" of quanity "<< qty <<endl;
    }
    void removeStock(int sku , int qty){
        store->removeProduct(sku,qty);
    }

    int checkStock(int sku){
        return store->checkStock(sku);
    }

    vector<Product*> getAvailableProducts(){
        return store->getAvailableItems();
    }
};

// abstract class replinish strategy 

class ReplenishStrategy{
    public:
    virtual ~ReplenishStrategy(){};
    virtual void replenish(InventoryManager* manager ,map<int,int>itemsToReplenish)=0;
};

// concrete classes 

class ThresholdReplenishStrategy:public ReplenishStrategy{
    int threshold;
    public:
    ThresholdReplenishStrategy(int threshold){
        this->threshold=threshold;
    }
    void replenish(InventoryManager* manager ,map<int,int>itemsToReplenish)override{
        cout<<" starting to check items for replenishing .. "<<endl;
        for(auto x:itemsToReplenish){
            int sku=x.first;
            int qty=x.second;
            int currQty=manager->checkStock(sku);
            if (currQty<qty){
                manager->addStock(sku,qty);
            }
        }
    }
};

class WeeklyReplenishStrategy:public ReplenishStrategy{
    public:
    WeeklyReplenishStrategy(){
    }
    void replenish(InventoryManager* manager ,map<int,int>itemsToReplenish)override{
        cout<<"weekly replenish strategy started .. !!! "<<endl;
    }
};


// creating a dark store 

class DarkStore{
    string name;
    double x, y;
    InventoryManager* manager;
    ReplenishStrategy* strategy;
    public:
    DarkStore(string name , double x , double y){
        this->name=name;
        this->x=x;
        this->y=y;
        manager=new InventoryManager(new DBInventoryStore);
    }
    ~DarkStore(){
        delete manager;
        if (strategy) delete strategy;
    }

    double distanceTo(double ux , double uy){
        return sqrt((x-ux)*(x-ux) + (y-uy)*(y-uy));
    }

    void runReplenishment(map<int,int>itemsToReplenish){
        if(strategy){
            strategy->replenish(manager,itemsToReplenish);
        }
    }

    // delegating methods 

    void addStock(int sku , int qty){
        Product* p=ProductFactory::createProduct(sku);
        manager->addStock(sku,qty);
       
    }
    void removeStock(int sku , int qty){
        manager->removeStock(sku,qty);
    }

    int checkStock(int sku){
        return manager->checkStock(sku);
    }

    vector<Product*> getAvailableProducts(){
        return manager->getAvailableProducts();
    }

    // gettes n setters

    ReplenishStrategy* getStrategy(){
        return this->strategy;
    }

    void setStrategy(ReplenishStrategy* str){
        this->strategy=str;
    }

    string getName(){
        return this->name;
    }

    double getXcoordinate(){
        return this->x;
    }
    double getYcoordinate(){
        return this->y;
    }

    InventoryManager* getManager(){
        return this->manager;
    }
};

// dark store manager singleton class
class DarkStoreManager{
    private:
    static DarkStoreManager* darkStoreManagerInstance;
    DarkStoreManager(){};
    // delete copy constructor 
    DarkStoreManager(DarkStoreManager& obj)=delete;
    // delete copy assigment operator  
    DarkStoreManager& operator=(DarkStoreManager& obj)=delete;


    vector<DarkStore*>darkStores;
    
    public:
    static DarkStoreManager* getInstance(){
        if (darkStoreManagerInstance==nullptr){
            darkStoreManagerInstance=new DarkStoreManager();
        }
        return darkStoreManagerInstance;
    }

    void registerDarkStore(DarkStore* ds){
        darkStores.push_back(ds);
    }

    vector<DarkStore*> getNearbyDarkStores(double ux , double uy , double maxd){

        vector<pair<double,DarkStore*>>listOfDarkStores;
        for(auto x:darkStores){
            double dist=x->distanceTo(ux,uy);
            if (dist<=maxd){
                listOfDarkStores.push_back({dist,x});
            }
        }

        sort(listOfDarkStores.begin(),listOfDarkStores.end(),
        [](auto& a , auto& b){
            return a.first<b.first;
        });

        vector<DarkStore*>ans;
        for(auto x:listOfDarkStores){
            ans.push_back(x.second);
        }
        return ans;
    }
};

DarkStoreManager* DarkStoreManager::darkStoreManagerInstance=nullptr;


// creating cart and user 


class Cart{
    public:
    vector<pair<Product*,int>>items;

    void addItem(int sku , int qty){
        Product* p=ProductFactory::createProduct(sku);
        items.push_back({p,qty});

        cout<<"[Cart Added ]"<< qty <<"quanities of product "<<p->getName()<<endl;
    }

    double getTotal(){
        double sum=0.0;
        for(auto it:items){
            sum+=(it.first->getPrice())*(it.second);
        }
        return sum;
    }

    vector<pair<Product*,int>> getItems(){
        return items;
    }
};


// creating user class 

class User{
    public:
    string name ;
    double x, y;
    Cart* cart;

    User(string name , double x , double y){
        this->name=name;
        this->x=x;
        this->y=y;
        cart=new Cart();
    }

    Cart* getCart(){
        return cart;
    }
};


// creating a dilevery partner class

class DileveryPartner{
    public:
    string name;
    DileveryPartner(string name){
        this->name=name;
    }
};

// creating class for order 


class Order{
    public:
    static int nextID;
    int orderId;
    User* user;

    vector<pair<Product*,int>>items;
    vector<DileveryPartner*>dileveryPartners;
    double totalAmount;
    Order(User* u){
        orderId=nextID++;
        user=u;
        totalAmount=0.0;
    }
};

int Order::nextID=1;


// singleton class Order manager


class OrderManager{
    private:
    static OrderManager* orderManagerInstance;
    OrderManager(){};
    OrderManager(OrderManager& obj)=delete;
    OrderManager& operator=(OrderManager& obj)=delete;

    vector<Order*>orders;

    public:
    static OrderManager* getInstance(){
        if (orderManagerInstance==nullptr){
            orderManagerInstance=new OrderManager();
        }
        return orderManagerInstance;
    }

    vector<Order*> getAllOrder(){
        return this->orders;
    }

    void placeOrder(User* user, Cart* cart){
        cout<<" ORDER MANAGER placing order for username -> "<<user->name<<endl;

        vector<pair<Product*,int>>items=cart->getItems();
        // find nearby store in 5km
        double maxDistance=5;

        vector<DarkStore*>darkStores=
        DarkStoreManager::getInstance()->getNearbyDarkStores(user->x,user->y,maxDistance);

        if(darkStores.empty()){
            cout<<"NO NEARBY DARK STORES AT THE MOMENT"<<endl;
            return ;
        }

        DarkStore* first=darkStores[0];

        bool allAvailable=true;
        for(pair<Product*,int>& item:items){
            int sku=item.first->getSku();
            int qty=item.second;

            if (first->checkStock(sku)<qty){
                allAvailable=false;
                break;
            }
        }

        Order* order=new Order(user);
        if (allAvailable){
            cout<<"[All items available at first store .. hurray only single dilevery partner needed]"<<endl;
            // remove products from store 
            for(pair<Product*,int>& item:items){
                int sku=item.first->getSku();
                int qty=item.second;
                first->removeStock(sku,qty);
                order->items.push_back({item.first,qty});
            }

            order->totalAmount=cart->getTotal();
            order->dileveryPartners.push_back(new DileveryPartner("partner1"));
            cout<<"assigned dilevery partner"<<endl;
        }
        else{
            cout<<"spiltting order via multiple stores"<<endl;
        }
    }
};
OrderManager* OrderManager::orderManagerInstance=nullptr;


class ZeptoHelper {
public:
    static void showAllItems(User* user) {
        cout << "\n[Zepto] All Available products within 5 KM for " << user->name << ":\n";

        DarkStoreManager* dsManager = DarkStoreManager::getInstance();
        vector<DarkStore*> nearbyStores = dsManager->getNearbyDarkStores(user->x, user->y, 5.0);

        // Collect each SKU → price (so we only display each product once)
        map<int, double> skuToPrice;
        map<int, string> skuToName;

        for (DarkStore* darkStore : nearbyStores) {
            vector<Product*> products = darkStore->getAvailableProducts();

            for (Product* product : products) {
                int sku = product->getSku();

                if (skuToPrice.count(sku) == 0) {
                    skuToPrice[sku] = product->getPrice();
                    skuToName[sku]  = product->getName();
                }
            }
        }

        for (auto& entry : skuToPrice) {
            int sku = entry.first;
            double price = entry.second;
            cout << "  SKU " << sku << " - " << skuToName[sku] << " @ ₹" << price << "\n";
        }
    }

    static void initialize() {
        auto dsManager = DarkStoreManager::getInstance();

        // DarkStore A.......
        DarkStore* darkStoreA = new DarkStore("DarkStoreA", 0.0, 0.0);
        darkStoreA->setStrategy(new ThresholdReplenishStrategy(3));
 
        cout << "\nAdding stocks in DarkStoreA...." << endl;  
        darkStoreA->addStock(101, 5); // Apple
        darkStoreA->addStock(102, 2); // Banana

        // DarkStore B.......
        DarkStore* darkStoreB = new DarkStore("DarkStoreB", 4.0, 1.0);
        darkStoreB->setStrategy(new ThresholdReplenishStrategy(3));

        cout << "\nAdding stocks in DarkStoreB...." << endl; 
        darkStoreB->addStock(101, 3); // Apple
        darkStoreB->addStock(103, 10); // Chocolate

        // DarkStore C.......
        DarkStore* darkStoreC = new DarkStore("DarkStoreC", 2.0, 3.0);
        darkStoreC->setStrategy(new ThresholdReplenishStrategy(3));

        cout << "\nAdding stocks in DarkStoreC...." << endl; 
        darkStoreC->addStock(102, 5); // Banana
        darkStoreC->addStock(201, 7); // T-Shirt

        dsManager->registerDarkStore(darkStoreA);
        dsManager->registerDarkStore(darkStoreB);
        dsManager->registerDarkStore(darkStoreC);
    }
};

/////////////////////////////////////////////
// Main(): High-Level Flow
/////////////////////////////////////////////

int main() {

    // 1) Initialize.
    ZeptoHelper::initialize();

    // 2) A User comes on Platform
    User* user = new User("Aditya", 1.0, 1.0);
    cout <<"\nUser with name " << user->name<< " comes on platform" << endl;

    // 3) Show all available items via Zepto
    ZeptoHelper::showAllItems(user);

    // 4) User adds items to cart (some not in a single store)
    cout<<"\nAdding items to cart\n";
    Cart* cart = user->getCart();
    cart->addItem(101, 4);  // dsA has 5, dsB has 3 
    cart->addItem(102, 3);  // dsA has 2, dsC has 5
    cart->addItem(103, 2);  // dsB has 10

    // 5) Place Order
    OrderManager::getInstance()->placeOrder(user, user->cart);

    // 6) Cleanup
    delete user;
    delete DarkStoreManager::getInstance();  // deletes all DarkStores and their inventoryManagers

    return 0;
}