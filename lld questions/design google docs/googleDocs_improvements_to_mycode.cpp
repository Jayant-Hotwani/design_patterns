#include <bits/stdc++.h>
using namespace std;

/*
========================================
IMPROVEMENTS APPLIED
========================================
1. Replaced raw pointers with smart pointers (shared_ptr)
2. Added virtual destructors (safe polymorphic deletion)
3. Fixed line break bug ("\n" instead of "/n")
4. Removed stale render cache (always render fresh)
5. Clear ownership (RAII - no manual delete needed)
6. Improved naming (PascalCase for classes)
7. Const correctness where applicable
8. Cleaner separation of concerns
========================================
*/


// ================================
// Base Class (Abstraction)
// ================================
class DocumentElement {
public:
    virtual string render() const = 0;  // const correctness
    virtual ~DocumentElement() {}       // virtual destructor (important)
};


// ================================
// Concrete Elements
// ================================

class TextElement : public DocumentElement {
    string text;

public:
    TextElement(const string& text) : text(text) {}

    string render() const override {
        return text;
    }
};


class ImageElement : public DocumentElement {
    string imagePath;

public:
    ImageElement(const string& path) : imagePath(path) {}

    string render() const override {
        return "[image at -> " + imagePath + "]";
    }
};


class LineBreakElement : public DocumentElement {
public:
    string render() const override {
        return "\n";  // FIXED bug (was "/n")
    }
};


// ================================
// Composite (Document)
// ================================

class Document {
    // Using shared_ptr to avoid memory leaks
    vector<shared_ptr<DocumentElement>> elements;

public:
    void addElement(shared_ptr<DocumentElement> element) {
        elements.push_back(element);
    }

    string render() const {
        string result;

        // Always render fresh (no stale cache bug)
        for (const auto& ele : elements) {
            result += ele->render();
        }

        return result;
    }
};


// ================================
// Strategy Pattern (Persistence)
// ================================

class PersistDocument {
public:
    virtual void saveDoc(const string& doc) const = 0;
    virtual ~PersistDocument() {}
};


class PersistToFile : public PersistDocument {
public:
    void saveDoc(const string& doc) const override {
        cout << doc << "\n[Saved to File]\n";
    }
};


class PersistToDB : public PersistDocument {
public:
    void saveDoc(const string& doc) const override {
        cout << doc << "\n[Saved to Database]\n";
    }
};


// ================================
// Editor (Facade / Orchestrator)
// ================================

class DocumentEditor {
    shared_ptr<Document> doc;
    shared_ptr<PersistDocument> persistDoc;

public:
    DocumentEditor(shared_ptr<Document> doc,
                   shared_ptr<PersistDocument> persistDoc)
        : doc(doc), persistDoc(persistDoc) {}

    // Helper methods to add elements
    void addTextElement(const string& text) {
        doc->addElement(make_shared<TextElement>(text));
    }

    void addImageElement(const string& path) {
        doc->addElement(make_shared<ImageElement>(path));
    }

    void addLineBreakElement() {
        doc->addElement(make_shared<LineBreakElement>());
    }

    // Always render fresh (no caching bug)
    string renderDoc() const {
        return doc->render();
    }

    // Save directly from latest render
    void saveDocument() const {
        persistDoc->saveDoc(doc->render());
    }
};


// ================================
// Main (Client)
// ================================

int main() {

    // Using smart pointers for ownership clarity
    auto doc = make_shared<Document>();
    auto persist = make_shared<PersistToDB>(); // can switch to File easily

    DocumentEditor editor(doc, persist);

    editor.addTextElement("Hello World ");
    editor.addLineBreakElement();
    editor.addImageElement("/users/jayant");
    editor.addLineBreakElement();
    editor.addTextElement("End of Document");

    cout << "Rendered Document:\n";
    cout << editor.renderDoc() << endl;

    editor.saveDocument();

    return 0;
}