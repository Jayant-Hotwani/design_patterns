#include <bits/stdc++.h>
using namespace std;

class ModelTrainer{
    public:
    void trainModel(){
        loadData();
        processData();
        trainingModel();
        testModel();
        saveData();
    }
    protected:
    void loadData(){
        cout<<"[common] - loading data into llm model"<<endl;
    }
    void processData(){
        cout<<"[common] - processing data into llm model"<<endl;
    }
    virtual void trainingModel()=0;
    virtual void testModel()=0;

    virtual void saveData(){
        cout<<"[common] - saving data into default llm model"<<endl;
    }
};


class GeminiModelTrainer:public ModelTrainer{

    void trainingModel() override{
        cout<<"training gemini"<<endl;
    }
    void testModel() override{
        cout<<" testing gemini model "<<endl;
    }
};

class CopilotModelTrainer:public ModelTrainer{

    void trainingModel() override{
        cout<<"training copilot"<<endl;
    }
    void testModel() override{
        cout<<" testing copilot model "<<endl;
    }
    void saveData(){
        cout<<"saving data into copilot model "<<endl;
    }
};


int main(){
    cout<<"model training for gemini"<<endl;
    ModelTrainer* geminiModelTrainer= new GeminiModelTrainer();
    geminiModelTrainer->trainModel();

    cout<<"model training for copilot"<<endl;
    ModelTrainer* copilotModelTrainer= new CopilotModelTrainer();
    copilotModelTrainer->trainModel();

    return 0;
}