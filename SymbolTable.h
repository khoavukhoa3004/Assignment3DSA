#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class node{
public:
    string name;
    int block;
    node *next;
    node *prev;
    node(){
        name = "";
        block = -1;
        next = NULL;
        prev = NULL;
    }
    node(string name, int block){
        this->name = name;
        this->block = block;
        prev = NULL;
        next = NULL;
    }
    ~node(){
        name = "";
        block = 0;
    }
};


class SymbolTable
{
public:
    SymbolTable(){};
    friend class SEARCH;
    void run(string filename);
    bool checkName(string);
    bool checkStatement(string *, string);
    bool checkInt(string);
    bool checkString(string);
    int Mahoa(string, int,int);
};
// parameter = -1: not function, -2: not type, else is function
class Object{
    string name;
    int block;
    string ReturnType;
    int parameter;
public:
    string *ParaType;
    Object(){
        name = "";
        block = -1;
        ReturnType = "";
        parameter = -2; 
        ParaType = NULL;
    }
    Object(string name, int block,int parameter = -2){
        this->name = name;
        this->block = block;
        this->parameter = parameter;
    }
    void putName(string);
    void putBlock(int);
    void putReturnType(string);
    void putParameter(int);
    int getParameter();
    string getReturnType();
    string getName();
    int getBlock();
    ~Object(){
        clear();
    }
    void clear(){
        this->name ="";
        this->block = -1;
        this->ReturnType = "";
        this->parameter = -2;
        if(ParaType != NULL){
            delete[] ParaType;
            this->ParaType = NULL;
        }
    }
};

class SEARCH{
    string nameSearch;
    int capacity;
    int count;
    int currentBlock;
public:
    int *ci;
    Object *obj;
    node *head;
    node *tail;
    SEARCH(){
        nameSearch ="";
        capacity = 0;
        count = 0;
        ci = NULL;
        obj = NULL;
        currentBlock = 0;
        head = NULL;
        tail = NULL; 
    }
    SEARCH(string name, int capacity, int *ci){
        this->nameSearch = name;
        this->capacity = capacity;
        this->ci = ci;
        this->count = 0; 
        this->currentBlock = 0;
    }
    int searchKey(int, string, int);
    int getCount(){
        return this->count;
    }
    int getCapacity(){return this->capacity;}
    void reputCount(){
        this->count = 0;
    }
    void putCapacity(int cap){
        this->capacity = cap;
    }
    void putNameSearch(string name){
        this->nameSearch = name;
    }
    void print(){
        bool space = false;
        for(int i = 0; i < capacity; i++){
            if(obj[i].getName()!= ""){
                if(!space){
                    cout<<i<<" "<<obj[i].getName()<<"//"<<obj[i].getBlock();
                    space = true;
                }
                else{
                    cout<<";"<<i<<" "<<obj[i].getName()<<"//"<<obj[i].getBlock();
                }
            }
        }
        if(space)
            cout<<endl;
    }
    void putBEGIN(){
        currentBlock++;   
    }
    void putEND(){
        if(currentBlock == 0)
            throw UnknownBlock();
        for(int i = 0; i < capacity; i++){
            if(obj[i].getBlock() == currentBlock)
                obj[i].clear();
        }
        currentBlock--;
    }
    int getCurrentBlock(){
        return this->currentBlock;
    }
    ~SEARCH(){
        this->nameSearch ="";
        delete[] this->obj;
        this->capacity = 0;
        delete[] this->ci;
        this->count = 0;
        this->currentBlock = 0;
        while(tail){
            node *temp = tail;
            tail = tail->prev;
            if(tail)
                tail->next = NULL;
            temp->prev = NULL;
            delete temp;
        }
        if(head)
            head = NULL;
    }
    int checkFuct(string,string,int);
    void addNode(string , int );
    void removeNode(int block);
    int findNode(string name);
};
#endif