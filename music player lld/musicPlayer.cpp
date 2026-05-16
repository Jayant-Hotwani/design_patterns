#include<bits/stdc++.h>
using namespace std;


class Song {
    public:
    string name ;
    string atrist ;
    Song(string name, string artist){
        this->name=name;
        this->atrist=artist;
    }
    void playSong(){
        cout<<"playing song " << name << " composed by " <<atrist<<endl;
    }
    ~Song(){}
};


class Playlist{
    public:
    string name;
    vector<Song*>songs;
    Playlist(string name){
        this->name=name;
    }
    void addSongToPlaylist(Song* song){
        songs.push_back(song);
    }
    void removeSongFromPlaylist(Song* song){
        songs.erase(std::remove(songs.begin(),songs.end(),song),songs.end());
    }

    string getName(){
        return this->name;
    }

    ~Playlist(){
        for(int i=0;i<songs.size();i++)
        delete songs[i];
    }
};


// singleton class playlist manager 
class PlaylistManager{
   map<string,Playlist*>playlistMap;
   PlaylistManager(){
   }
   public:
   PlaylistManager(const PlaylistManager& )=delete;
   PlaylistManager& operator=(const PlaylistManager& )=delete;

   static PlaylistManager& getInstance() {
        static PlaylistManager instance;
        return instance;
    }

    void createPlaylist(string name){
        if(playlistMap.find(name)!=playlistMap.end()){
            cout<<"playlist with name "<<name << " already exists.. try creating using a different name"<<endl;
        }
        else{
            playlistMap[name]=new Playlist(name);
            cout<<" playlist created with name  " << name  <<endl;
        }
    }

    void deletePlaylist(string name){
        if(playlistMap.find(name)==playlistMap.end()){
            cout<<"playlist with name " << name << " does not exists.. try deleting using a different name"<<endl;
        }
        else{
            playlistMap.erase(name);
        }
    }

    Playlist* getPlaylist(string playlistName){
        return playlistMap[playlistName]; 
    }

    void addSongtoPlaylist(Song* song , string playlistName){
         if (playlistMap.find(playlistName) == playlistMap.end()) {
            cout<<"creating new playlist...\n";
            playlistMap[playlistName] = new Playlist(playlistName);
         }
            playlistMap[playlistName]->addSongToPlaylist(song);
    }
};


// strategy design pattern for creating play music strategy 

class PlayStrategy{
    
    public:
    virtual void playMusic()=0;
    void addToQueue(){}
};

class SequentialPlayStrategy:public PlayStrategy{
    Playlist* playlist;
    public:
    SequentialPlayStrategy(Playlist* playlist){
        this->playlist=playlist;
    }
    void playMusic(){
        vector<Song*>songs=playlist->songs;
        for (int i = 0; i < songs.size(); i++)
        {
            songs[i]->playSong();
        }   
    }
};

class RandomPlayStrategy:public PlayStrategy{
    Playlist* playlist;
    public:
    RandomPlayStrategy(Playlist* playlist){
        this->playlist=playlist;
    }
    void playMusic(){
        vector<Song*> songs = playlist->songs;
        random_device rd;
        mt19937 g(rd());

        
        shuffle(songs.begin(), songs.end(), g);

        // play in shuffled order
        for (auto song : songs) {
            song->playSong();
        }
    }
};


class MusicPlayerFacade{

    public:
    Playlist* curr_playlist;
    PlayStrategy* strategy;
    PlaylistManager* playlistManager;


    MusicPlayerFacade(PlayStrategy* strategy,PlaylistManager* playlistManager){
        this->strategy=strategy;
        this->playlistManager=playlistManager;
    }

    void LoadPlaylist(string name){
        curr_playlist=playlistManager->getPlaylist(name);
    }
    void setStrategy(PlayStrategy* strategy){
        this->strategy=strategy;
    }

    void playMusic()
    {
        cout<<" playlist started .. created by -> "<<curr_playlist->getName()<<endl;
        strategy->playMusic();
    }
};


int main(){
    PlaylistManager& PlaylistManager=PlaylistManager::getInstance();

    PlaylistManager.createPlaylist("jayant");
    Song* song1=new Song("khariyat ","arijit");
    Song* song2=new Song("janam janam ","arijit");
    Song* song3=new Song(" ye dil deewaana " ," udit narayan ");

    PlaylistManager.createPlaylist("vishdih");
    Song* song4=new Song("cheques "," diljit ");
    Song* song5=new Song("daru band kal se ","mika");
 

    PlaylistManager.addSongtoPlaylist(song1,"jayant");
    PlaylistManager.addSongtoPlaylist(song2,"jayant");
    PlaylistManager.addSongtoPlaylist(song3,"jayant");
    PlaylistManager.addSongtoPlaylist(song4,"vishdih");
    PlaylistManager.addSongtoPlaylist(song5,"vishdih");
    

    Playlist* playlist1=PlaylistManager.getPlaylist("jayant");
    Playlist* playlist2=PlaylistManager.getPlaylist("vishdih");

    PlayStrategy* startegy1=new SequentialPlayStrategy(playlist1);
    PlayStrategy* startegy2=new RandomPlayStrategy(playlist2);

    MusicPlayerFacade* player=new MusicPlayerFacade(startegy1,&PlaylistManager);
    player->LoadPlaylist("jayant");
    player->playMusic();
    player->LoadPlaylist("vishdih");
    player->setStrategy(startegy2);
    player->playMusic();
    return 0;
}