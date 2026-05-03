#include <bits/stdc++.h>
using namespace std;

/*
========================================
GOOGLE DOCS LLD (INTERVIEW READY)
Patterns Used:
- Composite (Document)
- Flyweight (TextStyle)
- Command (Undo/Redo)
- Strategy (Persistence)
- Facade (Editor)
========================================
*/


// ============================
// Flyweight (TextStyle)
// ============================
class TextStyle {
public:
    string font;
    int size;
    bool bold;

    TextStyle(string f, int s, bool b)
        : font(f), size(s), bold(b) {}
};

// Factory to reuse styles
class StyleFactory {
    static map<string, shared_ptr<TextStyle>> styles;

public:
    static shared_ptr<TextStyle> getStyle(string f, int s, bool b) {
        string key = f + to_string(s) + (b ? "b" : "n");

        if (!styles.count(key)) {
            styles[key] = make_shared<TextStyle>(f, s, b);
        }
        return styles[key];
    }
};

map<string, shared_ptr<TextStyle>> StyleFactory::styles;


// ============================
// Document Elements
// ============================
class DocumentElement {
public:
    virtual string render() const = 0;
    virtual ~DocumentElement() {}
};


class TextElement : public DocumentElement {
    string text;
    shared_ptr<TextStyle> style;

public:
    TextElement(string t, shared_ptr<TextStyle> s)
        : text(t), style(s) {}

    string render() const override {
        return text + (style->bold ? "[B]" : "");
    }
};


class ImageElement : public DocumentElement {
    string path;

public:
    ImageElement(string p) : path(p) {}

    string render() const override {
        return "[IMG:" + path + "]";
    }
};


// ============================
// Composite (Document)
// ============================
class Document {
    vector<shared_ptr<DocumentElement>> elements;

public:
    void add(shared_ptr<DocumentElement> e) {
        elements.push_back(e);
    }

    void removeLast() {
        if (!elements.empty()) elements.pop_back();
    }

    string render() const {
        string res;
        for (auto &e : elements) res += e->render();
        return res;
    }
};


// ============================
// Command Pattern
// ============================
class Command {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Command() {}
};


class AddTextCommand : public Command {
    shared_ptr<Document> doc;
    shared_ptr<DocumentElement> element;

public:
    AddTextCommand(shared_ptr<Document> d, shared_ptr<DocumentElement> e)
        : doc(d), element(e) {}

    void execute() override {
        doc->add(element);
    }

    void undo() override {
        doc->removeLast();
    }
};


// ============================
// Strategy (Persistence)
// ============================
class PersistStrategy {
public:
    virtual void save(const string& doc) = 0;
    virtual ~PersistStrategy() {}
};

class SaveToDB : public PersistStrategy {
public:
    void save(const string& doc) override {
        cout << doc << "\n[Saved to DB]\n";
    }
};

class SaveToFile : public PersistStrategy {
public:
    void save(const string& doc) override {
        cout << doc << "\n[Saved to File]\n";
    }
};


// ============================
// Editor (Facade)
// ============================
class Editor {
    shared_ptr<Document> doc;
    shared_ptr<PersistStrategy> persist;

    stack<shared_ptr<Command>> undoStack;

public:
    Editor(shared_ptr<Document> d,
           shared_ptr<PersistStrategy> p)
        : doc(d), persist(p) {}

    void addText(string text, string font="Arial",
                 int size=12, bool bold=false) {

        auto style = StyleFactory::getStyle(font, size, bold);
        auto element = make_shared<TextElement>(text, style);

        auto cmd = make_shared<AddTextCommand>(doc, element);
        cmd->execute();

        undoStack.push(cmd);
    }

    void undo() {
        if (!undoStack.empty()) {
            undoStack.top()->undo();
            undoStack.pop();
        }
    }

    void save() {
        persist->save(doc->render());
    }

    void show() {
        cout << doc->render() << endl;
    }
};


// ============================
// MAIN
// ============================
int main() {

    auto doc = make_shared<Document>();
    auto db = make_shared<SaveToDB>();

    Editor editor(doc, db);

    editor.addText("Hello ");
    editor.addText("World", "Arial", 12, true);

    editor.show();

    editor.undo();

    cout << "After Undo:\n";
    editor.show();

    editor.save();
}