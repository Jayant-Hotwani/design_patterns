#include <bits/stdc++.h>
using namespace std;

// flywieght
class TreeType{
    public:
    string name;
    string color;
    string texture;
    
    TreeType(string name, string color, string texture){
        this->name=name;
        this->color=color;
        this->texture=texture;
    }
    void draw(int x , int y){
        cout<<" name of tree " << name ;
        cout<<" color of tree " <<color ;
        cout<<" texture of tree  "<<texture;
        cout<< " -- planted at "<< x<< " && "<<y<<" cordinates "<<endl;
    }
};


// flywieght factory
class TreeFactory {
    public:
    unordered_map<string, TreeType*> cache;
    TreeType* getTreeType(string name, string color, string texture) {
    string key = name + "-" + color + "-" + texture;

        if (!cache.count(key)) {
            cache[key]=new TreeType(name, color, texture);
        }
        return cache[key];
    }
};


class Tree{
    public:
    int x; 
    int y;
    TreeType* treetype;
    Tree(int x , int y , TreeType* t){
        this->x=x;
        this->y=y;
        this->treetype=t;
    }

    void make_tree(){
        treetype->draw(x,y);
    }

};
// Forest (Client)
class Forest {
public:
    vector<Tree> trees;
    TreeFactory factory;

    void plantTree(int x, int y, string name, string color, string texture) {
        TreeType* type = factory.getTreeType(name, color, texture);
        trees.push_back(Tree(x, y, type));
    }

    void draw() {
        for (auto &tree : trees) {
            tree.make_tree();
        }
    }
};

int main() {
    Forest forest;

    forest.plantTree(1, 2, "Pine", "Green", "Rough");
    forest.plantTree(3, 4, "Pine", "Green", "Rough"); 
    forest.plantTree(5, 6, "Oak", "Dark Green", "Smooth");

    forest.draw();

    return 0;
}