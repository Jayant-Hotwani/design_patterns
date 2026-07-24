#include <bits/stdc++.h>
using namespace std;


class Momento{
    public:
    string state;
    Momento(string text){
        state=text;
    }
    string getState(){
        return state;
    }
};


class TextEditor{

    public:
    string content;
    void type(string str){
        content+=str;
    }
    Momento* save(){
        return new Momento(content);
    }
    void restore(Momento* m){
        content=m->getState();
    }
    string getContent(){
        return content;
    }
};

class History{

    private:
    vector<Momento*> momentos;

    public:
    void save(Momento* m) {
        momentos.push_back(m);
    }

    Momento* getLast() {
        Momento* m = momentos.back();
        momentos.pop_back();
        return m;
    }

};


int main(){
    TextEditor textEditor;
    History history;
    textEditor.type("hello");
    history.save(textEditor.save());
    textEditor.type("World!");
    cout << "Current: " << textEditor.getContent() << endl;
    textEditor.restore(history.getLast());
    cout << "After undo: " << textEditor.getContent() << endl;

    return 0;
}

