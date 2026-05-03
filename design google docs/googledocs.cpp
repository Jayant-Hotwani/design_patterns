#include <bits/stdc++.h>
using namespace std;


class DocumentElement{
    public:
    DocumentElement(){}
    virtual string render()=0;
    ~DocumentElement(){}
};

class textElement:public DocumentElement{
    string text;
    public:
    textElement(string text){
        this->text=text;
    }
    string render() override{
        return text;
    }
};

class imageElement:public DocumentElement{
    string imagePath;
    public:
    imageElement(string imagePath){
        this->imagePath=imagePath;
    }
    string render() override{
        return "[ image at -> "+ imagePath+" ] ";
    }
};

class lineBreakElement:public DocumentElement{
    public:
    lineBreakElement(){
    }
    string render() override{
        return "/n";
    }
};

class Document{
    vector<DocumentElement*>elements;
    public:
    Document(){};
    void addElement(DocumentElement* ele){
        elements.push_back(ele);
    }
    string renderDocument(){
        string result;
        for(auto ele:elements){
            // cout<<ele->render()<<endl;
            result+=ele->render();
        }
        // cout<<"result is "<< result<<endl;
        return result;
    }
    ~Document(){};
};

class PersistDocument{
    public:
    PersistDocument(){};
    virtual void saveDoc(string doc)=0;
    ~PersistDocument(){};
};

class PersistToFile:public PersistDocument{
    public:
    void saveDoc(string doc) override{
        cout<< doc  <<" doxument saved to file "<<endl;
    }
};

class PersistToDB:public PersistDocument{
    public:
    void saveDoc(string doc) override{
        cout<< doc<<endl;
        cout <<" doxument saved to database "<<endl;
    }
};



// we can futher break this class 
// and create documentReneder class just for render functionality 
// similarly we can remove save also and put it client code 
class DocumentEditor{
    Document* doc;
    PersistDocument* persistDoc;
    string renderedDocument;
    public:
    DocumentEditor(Document* doc , PersistDocument* persistDoc){
        this->doc=doc;
        this->persistDoc=persistDoc;
    }
    void addTextElement(string text){
        doc->addElement(new textElement(text));
    }
    void addImageElement(string path){
        doc->addElement(new imageElement(path));
    }
    void addlineBreakElement(){
        doc->addElement(new lineBreakElement());
    }
    string renderDoc(){
        renderedDocument=doc->renderDocument();
        return renderedDocument;
    }
    void saveDocument(){
        persistDoc->saveDoc(renderedDocument);
    }
    ~DocumentEditor(){}
};

int main()
{
    Document* doc = new Document;
    PersistDocument* persistDocument= new PersistToDB();

    DocumentEditor* googleDocs=new DocumentEditor(doc,persistDocument);

    googleDocs->addTextElement(" hello world ");
    googleDocs->addlineBreakElement();
    googleDocs->addImageElement(" /users/jayanthotwani ");
    googleDocs->addlineBreakElement();

    cout<<googleDocs->renderDoc()<<endl;

    googleDocs->saveDocument();

    return 0;
}


