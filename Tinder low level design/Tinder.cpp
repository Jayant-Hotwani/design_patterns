#include <bits/stdc++.h>
using namespace std;

// observer design pattern 

// abstract observer 

class NotificationObserver{
    public:
    virtual ~NotificationObserver(){};
    virtual void update (string msg)=0;
};


// concrete observer 
class UserNotificationObserver:public NotificationObserver{
    private:
    string userId;
    public:
    UserNotificationObserver(string id){
        this->userId=id;
    }
    void update(string msg) override{
        cout<<" notification recieved for user with id "
        <<userId<<" and the messsage recieved is -> "<<msg<<endl;
    }
};


// observable subject - singleton class 
class NotificationService{
    NotificationService(){};
    NotificationService(NotificationService& obj)=delete;
    NotificationService& operator=(NotificationService& obj)=delete;

    static NotificationService* notificationServiceInstance;
    map<string,NotificationObserver*>observers;

    public:
    static NotificationService* getInstance(){
        if (notificationServiceInstance==nullptr)
        notificationServiceInstance=new NotificationService();

        return notificationServiceInstance;
    }

    void registerObserver(string userId,NotificationObserver* obs){
        observers[userId]=obs;
    }

    void removeObserver(string userId){
        observers.erase(userId);
    }

    void notifyUser(string userId , string msg){
        if (observers.find(userId)!=observers.end()){
            observers[userId]->update(msg);
        }
    }
    void notifyAll(string msg){
        for(auto x:observers){
            x.second->update(msg);
        }
    }
};

NotificationService* NotificationService::notificationServiceInstance=nullptr;


// basic model classes // 

enum class Gender{
    MALE,
    FEMALE,
    OTHER
};


// location class 

class Location{
    double x , y;
    public:
    Location(){
        x=0.0;
        y=0.0;
    }
    Location(double x , double y){
        this->x=x;
        this->y=y;
    }

    double getXcordinate() const {
        return x;
    }
    double getYcordinate() const {
        return y;
    }
    void setXcordinate(double x){
        this->x=x;
    }
    void setYcordinate(double y){
        this->y=y;
    } 

    double calculateDistance(const Location* other) const {
        double ux = other->getXcordinate();
        double uy = other->getYcordinate();

        return sqrt((ux - x)*(ux - x) + (uy - y)*(uy - y));
    }
};


// creating interest class
// Interest class
class Interest {
private:
    std::string name;
    std::string category;
    
public:
    Interest() {
        name = "";
        category = "";
    }
    
    Interest(const string& n, const string& c) {
        name = n;
        category = c;
    }
    
    std::string getName() const {
        return name;
    }
    
    std::string getCategory() const {
        return category;
    }
};

// creating prefrence class 

class Preference{
    private:
    vector<Gender>interestedIn;
    int min_age;
    int max_age;
    double max_distance;
    vector<string>interests;
    public:
    Preference(){
        min_age=18;
        max_age=35;
        max_distance=10;
    }

    void addGenderPreference(Gender gender){
        interestedIn.push_back(gender);
    }
    void removeGenderPreference(Gender gender){
        interestedIn.erase(remove(interestedIn.begin(),interestedIn.end(),gender),interestedIn.begin());
    }
    void setAgeRange(int mini, int maxi){
        min_age=mini;
        max_age=maxi;
    }
    void setMaxDistance(double d){
        this->max_distance=d;
    }
    void addInterest(string interest){
        interests.push_back(interest);
    }
    void removeInterest(string& interest){
        interests.erase(remove(interests.begin(),interests.end(),interest),interests.begin());
    }

    bool isInterestedInGender(Gender gender){
        return find(interestedIn.begin(),interestedIn.end(),gender)!=interestedIn.end();
    }

    vector<string> getInterests(){
        return interests;
    }
    vector<Gender> getInterestedIn(){
        return interestedIn;
    }

    int getMinAge(){
        return min_age;
    }
    int getMaxAge(){
        return max_age;
    }
    double getMaxDistance(){
        return max_distance;
    }
    bool isDistanceAcceptable(double distance) const {
        return distance <= max_distance;
    }
    bool isAgeInRange(int age) const {
        return age >= min_age && age <= max_age;
    }
};

// creating message class 

class Message{
    string senderId;
    string content;
    time_t timestamp;
    public:
    Message(string senderId,string content){
        this->content=content;
        this->senderId=senderId;
    }
    string getSenderId(){
        return senderId;
    }
    string getContent(){
        return content;
    }
    time_t getTimestamp(){
        return timestamp;
    }
};


// creating a chat room class

class ChatRoom{
    string id;
    vector<string>participantsId;
    vector<Message*>messages;
    public:
    ChatRoom(string id , string userid1 ,string userid2){
        this->id=id;
        participantsId.push_back(userid1);
        participantsId.push_back(userid2);
    }

    string getId(){
        return this->id;
    }
    void addMessage(string senderId , string content){
        messages.push_back(new Message(senderId,content));
    }

    bool hasParticipant(string userId){
        return find(participantsId.begin(),participantsId.end(),userId)!=participantsId.end();
    }

    vector<string> getParticipants(){
        return participantsId;
    }
    vector<Message*> getMessages(){
        return messages;
    }
    void displayChat(){
        cout<<"chat room "<< id <<" messages displayed"<<endl;
        for(auto x:messages){
            cout<<x->getSenderId()<<" sent the message -> ";
            cout<<x->getContent()<<endl;
        }

    }
};


class UserProfile{
    private:
    string name;
    int age;
    Gender gender;
    string bio;
    vector<string> photos;
    vector<Interest*> interests;
    Location* location;
    
public:
    UserProfile() {
        name = "";
        age = 0;
        gender = Gender::OTHER;
    }
    
    ~UserProfile() {
        for (auto interest : interests) {
            delete interest;
        }
    }

    void setName(const string& n) {
        name = n;
    }
    
    void setAge(int a) {
        age = a;
    }
    
    void setGender(Gender g) {
        gender = g;
    }
    
    void setBio(const string& b) {
        bio = b;
    }
    
    void addPhoto(const string& photoUrl) {
        photos.push_back(photoUrl);
    }
    
    void removePhoto(const string& photoUrl) {
        photos.erase(remove(photos.begin(), photos.end(), photoUrl), photos.end());
    }
    
    void addInterest(const string& name, const string& category) {
        Interest* interest = new Interest(name, category);
        interests.push_back(interest);
    }
    
    void removeInterest(const string& name) {
        auto it = find_if(interests.begin(), interests.end(), 
            [&name](const Interest* interest) {
                return interest->getName() == name;
            });
        
        if (it != interests.end()) {
            delete *it;
            interests.erase(it);
        }
    }
    
    void setLocation(Location* loc) {
        this->location = loc;
    }
    
    string getName() const {
        return name;
    }
    
    int getAge() const {
        return age;
    }
    
    Gender getGender() const {
        return gender;
    }
    
    string getBio() const {
        return bio;
    }
    
    const vector<string>& getPhotos() const {
        return photos;
    }
    
    const vector<Interest*>& getInterests() const {
        return interests;
    }
    
    const Location* getLocation(){
        return location;
    }
    
    void display() const {
        cout << "===== Profile =====" << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Gender: ";
        switch (gender) {
            case Gender::MALE: cout << "Male"; break;
            case Gender::FEMALE: cout << "Female"; break;
            case Gender::OTHER: cout << "Other"; break;
        }
        cout << endl;
        
        cout << "Bio: " << bio << endl;
        
        cout << "Photos: ";
        for (const auto& photo : photos) {
            cout << photo << ", ";
        }
        cout << endl;
        
        cout << "Interests: ";
        for (const auto& interest : interests) {
            cout << interest->getName() << " (" << interest->getCategory() << "), ";
        }
        cout << endl;
        cout << "===================" << endl;
    } 
};

// creating user system
enum class SwipeAction{
    LEFT,
    RIGHT
};

// user class 

class User{
    private:
    string id;
    UserProfile* profile;
    Preference* preference;
    map<string,SwipeAction>swipeHistory;
    NotificationObserver* notificationObserver;
    public:
    User(string id){
        this->id=id;
        profile=new UserProfile();
        preference=new Preference();
        notificationObserver=new UserNotificationObserver(id);
        NotificationService::getInstance()->registerObserver(id,notificationObserver);
    }
    ~User(){
        delete profile;
        delete preference;
        delete notificationObserver;
        NotificationService::getInstance()->removeObserver(id);
    }

     string getId() const {
        return id;
    }
    
    UserProfile* getProfile() {
        return profile;
    }
    
    Preference* getPreference() {
        return preference;
    }

    
    
    void swipe(const string& otherUserId, SwipeAction action) {
        swipeHistory[otherUserId] = action;
    }

    bool hasLiked(string otherUserId){
        auto it=swipeHistory.find(otherUserId);
        if(it!=swipeHistory.end()){
            if (it->second==SwipeAction::RIGHT)return true;
        }
        return false;
    }

    bool hasDisLiked(string otherUserId){
        auto it=swipeHistory.find(otherUserId);
        if(it!=swipeHistory.end()){
            if (it->second==SwipeAction::LEFT)return true;
        }
        return false;
    }

    bool hasInteractedWith(string otherUserId){
        return swipeHistory.find(otherUserId)!=swipeHistory.end();
    }

    void displayProfile(){
        profile->display();
    }
};


// creating location class && location strategy 


// abstract class 
class LocationStrategy{
    public:
    virtual ~LocationStrategy(){};
    virtual vector<User*> findNearbyUsers(const Location* location , double max_d , const vector<User*>& allUsers)=0;
};

// creating concrete class 

class BasicLocationStrategy:public LocationStrategy{
    public:
    BasicLocationStrategy(){};
    vector<User*> findNearbyUsers(const Location* location , 
        double max_d , const vector<User*>& allUsers)override{

            vector<User*>allNearbyUser;
            for(User* x:allUsers){
                double dist=location->calculateDistance(x->getProfile()->getLocation());
                if (dist<max_d){
                    allNearbyUser.push_back(x);
                }
            }
            return allNearbyUser;
    };
};

// Location service with Strategy Pattern
class LocationService {
private:
    LocationStrategy* strategy;
    
    // Singleton Pattern
    static LocationService* instance;
    
    LocationService() {
        strategy = new BasicLocationStrategy();
    }
    
public:
    static LocationService* getInstance() {
        if (instance == nullptr) {
            instance = new LocationService();
        }
        return instance;
    }
    
    ~LocationService() {
        delete strategy;
    }
    
    void setStrategy(LocationStrategy* newStrategy) {
        delete strategy;
        strategy = newStrategy;
    }
    
    vector<User*> findNearbyUsers(const Location* location, double maxDistance, const vector<User*>& allUsers) {
        return strategy->findNearbyUsers(location, maxDistance, allUsers);
    }
};

// Initialize static member
LocationService* LocationService::instance = nullptr;

// matching algortihm 


enum class MatcherType{
    BASIC,
    INTERESTS_BASED,
    LOCATION_BASED
};

class Matcher{
    public:
    virtual ~Matcher(){};
    virtual double calculateMatchScore(User* user1,User* user2)=0;
};

// Concrete matcher: Basic matcher
class BasicMatcher : public Matcher {
public:
    double calculateMatchScore(User* user1, User* user2) override {
        // Basic scoring, just check if preferences align
        bool user1LikesUser2Gender = user1->getPreference()->isInterestedInGender(user2->getProfile()->getGender());
        bool user2LikesUser1Gender = user2->getPreference()->isInterestedInGender(user1->getProfile()->getGender());
        
        if (!user1LikesUser2Gender || !user2LikesUser1Gender) {
            return 0.0;
        }
        
        // Check age preference
        bool user1LikesUser2Age = user1->getPreference()->isAgeInRange(user2->getProfile()->getAge());
        bool user2LikesUser1Age = user2->getPreference()->isAgeInRange(user1->getProfile()->getAge());
        
        if (!user1LikesUser2Age || !user2LikesUser1Age) {
            return 0.0;
        }
        
        // Check distance preference
        double distance = user1->getProfile()->getLocation()->calculateDistance(user2->getProfile()->getLocation());
        bool user1LikesUser2Distance = user1->getPreference()->isDistanceAcceptable(distance);
        bool user2LikesUser1Distance = user2->getPreference()->isDistanceAcceptable(distance);
        
        if (!user1LikesUser2Distance || !user2LikesUser1Distance) {
            return 0.0;
        }
        
        // If all basic criteria match, return a base score
        return 0.5; // 50% match
    }
};

    // Concrete matcher: Interests-based matcher
class InterestsBasedMatcher : public Matcher {
public:
    double calculateMatchScore(User* user1, User* user2) override {
        // First, check basic compatibility
        BasicMatcher basicMatcher;
        double baseScore = basicMatcher.calculateMatchScore(user1, user2);
        
        if (baseScore == 0.0) {
            return 0.0; // No need to continue if basic criteria don't match
        }
        
        // Calculate score based on shared interests
        std::vector<std::string> user1InterestNames;
        for (const auto& interest : user1->getProfile()->getInterests()) {
            user1InterestNames.push_back(interest->getName());
        }
        
        int sharedInterests = 0;
        for (const auto& interest : user2->getProfile()->getInterests()) {
            if (std::find(user1InterestNames.begin(), user1InterestNames.end(), interest->getName()) != user1InterestNames.end()) {
                sharedInterests++;
            }
        }
        
        // Bonus score based on shared interests (up to 0.5 additional points)
        double maxInterests = std::max(user1->getProfile()->getInterests().size(), user2->getProfile()->getInterests().size());
        double interestScore = maxInterests > 0 ? 0.5 * (sharedInterests / maxInterests) : 0.0;
        
        return baseScore + interestScore;
    }
};
    
// Concrete matcher: Location-based matcher
class LocationBasedMatcher : public Matcher {
public:
    double calculateMatchScore(User* user1, User* user2) override {
        // First, check basic compatibility
        InterestsBasedMatcher interestsMatcher;
        double baseScore = interestsMatcher.calculateMatchScore(user1, user2);
        
        if (baseScore == 0.0) {
            return 0.0; // No need to continue if basic criteria don't match
        }
        
        // Calculate score based on proximity
        double distance = user1->getProfile()->getLocation()->calculateDistance(user2->getProfile()->getLocation());
        double maxDistance = std::min(user1->getPreference()->getMaxDistance(), user2->getPreference()->getMaxDistance());
        
        // Closer is better, score decreases with distance (up to 0.2 additional points)
        double proximityScore = maxDistance > 0 ? 0.2 * (1.0 - (distance / maxDistance)) : 0.0;
        
        return baseScore + proximityScore;
    }
};


// Factory Pattern: Matcher factory
class MatcherFactory {
public:
    static Matcher* createMatcher(MatcherType type) {
        switch (type) {
            case MatcherType::BASIC:
                return new BasicMatcher();
            case MatcherType::INTERESTS_BASED:
                return new InterestsBasedMatcher();
            case MatcherType::LOCATION_BASED:
                return new LocationBasedMatcher();
            default:
                return new BasicMatcher();
        }
    }
};
// Facade Pattern: Dating app system
class DatingApp {
private:
    vector<User*> users;
    vector<ChatRoom*> chatRooms;
    Matcher* matcher;
    
    // Singleton Pattern
    static DatingApp* instance;
    
    DatingApp() {
        // Default to location-based matcher
        matcher = MatcherFactory::createMatcher(MatcherType::LOCATION_BASED);
    }
    
public:
    static DatingApp* getInstance() {
        if (instance == nullptr) {
            instance = new DatingApp();
        }
        return instance;
    }
    
    ~DatingApp() {
        for (auto user : users) {
            delete user;
        }
        
        for (auto chatRoom : chatRooms) {
            delete chatRoom;
        }
        
        delete matcher;
    }
    
    void setMatcher(MatcherType type) {
        delete matcher;
        matcher = MatcherFactory::createMatcher(type);
    }
    
    User* createUser(const string& userId) {
        User* user = new User(userId);
        users.push_back(user);
        return user;
    }
    
    User* getUserById(const string& userId) {
        for (auto user : users) {
            if (user->getId() == userId) {
                return user;
            }
        }
        return nullptr;
    }
    
    std::vector<User*> findNearbyUsers(const std::string& userId, double maxDistance = 5.0) {
        User* user = getUserById(userId);
        if (user == nullptr) {
            return vector<User*>();
        }
        
        // Find users within maxDistance km
        vector<User*> nearbyUsers = LocationService::getInstance()->findNearbyUsers(
            user->getProfile()->getLocation(), maxDistance, users);
        
        // Filter out the user themselves
        nearbyUsers.erase(remove(nearbyUsers.begin(), nearbyUsers.end(), user), nearbyUsers.end());
        
        // Filter out users that don't match preferences or have already been swiped
        vector<User*> filteredUsers;
        for (User* otherUser : nearbyUsers) {
            // Skip users that have already been interacted with
            if (user->hasInteractedWith(otherUser->getId())) {
                continue;
            }
            
            // Calculate match score
            double score = matcher->calculateMatchScore(user, otherUser);
            
            // If score is above 0, they meet basic preference criteria
            if (score > 0) {
                filteredUsers.push_back(otherUser);
            }
        }
        
        return filteredUsers;
    }
    
    bool swipe(const string& userId, const string& targetUserId, SwipeAction action) {
        User* user = getUserById(userId);
        User* targetUser = getUserById(targetUserId);
        
        if (user == nullptr || targetUser == nullptr) {
            cout << "User not found." << endl;
            return false;
        }
        
        user->swipe(targetUserId, action);
        
        // Check if it's a match
        if (action == SwipeAction::RIGHT && targetUser->hasLiked(userId)) {
            // It's a match!
            string chatRoomId = userId + "_" + targetUserId;
            ChatRoom* chatRoom = new ChatRoom(chatRoomId, userId, targetUserId);
            chatRooms.push_back(chatRoom);
            
            // Notify both users
            NotificationService::getInstance()->notifyUser(userId, "You have a new match with " + targetUser->getProfile()->getName() + "!");
            NotificationService::getInstance()->notifyUser(targetUserId, "You have a new match with " + user->getProfile()->getName() + "!");
            return true;
        }
        return false;
    }
    
    ChatRoom* getChatRoom(const string& user1Id, const string& user2Id) {
        for (auto chatRoom : chatRooms) {
            if (chatRoom->hasParticipant(user1Id) && chatRoom->hasParticipant(user2Id)) {
                return chatRoom;
            }
        }
        return nullptr;
    }
    
    void sendMessage(const string& senderId, const string& receiverId, const string& content) {
        ChatRoom* chatRoom = getChatRoom(senderId, receiverId);
        if (chatRoom == nullptr) {
            cout << "No chat room found between these users." << endl;
            return;
        }
        
        chatRoom->addMessage(senderId, content);
        
        // Notify the receiver
        NotificationService::getInstance()->notifyUser(receiverId, "New message from " + getUserById(senderId)->getProfile()->getName());
    }
    
    void displayUser(const string& userId) {
        User* user = getUserById(userId);
        if (user == nullptr) {
            cout << "User not found." << endl;
            return;
        }
        
        user->displayProfile();
    }
    
    void displayChatRoom(const string& user1Id, const string& user2Id) {
        ChatRoom* chatRoom = getChatRoom(user1Id, user2Id);
        if (chatRoom == nullptr) {
            cout << "No chat room found between these users." << endl;
            return;
        }
        
        chatRoom->displayChat();
    }
};

// Initialize static member
DatingApp* DatingApp::instance = nullptr;

int main(){
    DatingApp* app = DatingApp::getInstance();
    
    // Create users
    User* user1 = app->createUser("user1");
    User* user2 = app->createUser("user2");

    // Set user1 profile
    UserProfile* profile1 = user1->getProfile();
    profile1->setName("Rohan");
    profile1->setAge(28);
    profile1->setGender(Gender::MALE);
    profile1->setBio("I am a software developer");
    profile1->addPhoto("rohan_photo1.jpg");
    profile1->addInterest("Coding", "Programming");
    profile1->addInterest("Travel", "Lifestyle");
    profile1->addInterest("Music", "Entertainment");
    
    // Setup user1 preferences
    Preference* pref1 = user1->getPreference();
    pref1->addGenderPreference(Gender::FEMALE);
    pref1->setAgeRange(25, 30);
    pref1->setMaxDistance(10.0);
    pref1->addInterest("Coding");
    pref1->addInterest("Travel");
    
    // Setup user2 profile
    UserProfile* profile2 = user2->getProfile();
    profile2->setName("Neha");
    profile2->setAge(27);
    profile2->setGender(Gender::FEMALE);
    profile2->setBio("Art teacher who loves painting and traveling.");
    profile2->addPhoto("neha_photo1.jpg");
    profile2->addInterest("Painting", "Art");
    profile2->addInterest("Travel", "Lifestyle");
    profile2->addInterest("Music", "Entertainment");
    
    // Setup user2 preferences
    Preference* pref2 = user2->getPreference();
    pref2->addGenderPreference(Gender::MALE);
    pref2->setAgeRange(27, 30);
    pref2->setMaxDistance(15.0);
    pref2->addInterest("Coding");
    pref2->addInterest("Movies");

    // Set location for user1
    Location* location1;
    location1->setXcordinate(1.01);
    location1->setYcordinate(1.02);
    profile1->setLocation(location1);
    
    // Set location for user2 (Close to user1, within 5km)
    Location* location2;
    location2->setXcordinate(1.03);
    location2->setYcordinate(1.04);
    profile2->setLocation(location2);

    // Display user profiles
    std::cout << "---- User Profiles ----" << std::endl;
    app->displayUser("user1");
    app->displayUser("user2");
    
    // Find nearby users for user1 (within 5km)
    std::cout << "\n---- Nearby Users for user1 (within 5km) ----" << std::endl;
    std::vector<User*> nearbyUsers = app->findNearbyUsers("user1", 5.0);
    std::cout << "Found " << nearbyUsers.size() << " nearby users" << std::endl;
    for (User* user : nearbyUsers) {
        std::cout << "- " << user->getProfile()->getName() << " (" << user->getId() << ")" << std::endl;
    }
    
    // User1 swipes right on User2
    std::cout << "\n---- Swipe Actions ----" << std::endl;
    std::cout << "User1 swipes right on User2" << std::endl;
    app->swipe("user1", "user2", SwipeAction::RIGHT);
    
    // User2 swipes right on User1 (creating a match)
    std::cout << "User2 swipes right on User1" << std::endl;
    app->swipe("user2", "user1", SwipeAction::RIGHT);
    
    // Send messages in the chat room
    std::cout << "\n---- Chat Room ----" << std::endl;
    app->sendMessage("user1", "user2", "Hi Neha, Kaise ho?");
    
    app->sendMessage("user2", "user1", "Hi Rohan, Ma bdiya tum btao");
    
    // Display the chat room
    app->displayChatRoom("user1", "user2");
}