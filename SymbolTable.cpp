#include "SymbolTable.h"

void SymbolTable::run(string filename)
{
    ifstream ifs(filename);
    string GetLineOfFile;
    if(ifs.eof())
        throw InvalidInstruction("");
    string split[4];
    SEARCH manage;
    getline(ifs, GetLineOfFile);
    if(!checkStatement(split, GetLineOfFile))
        throw InvalidInstruction(GetLineOfFile);
    if(split[0] == "LINEAR"){
        if(!checkInt(split[1]) || !checkInt(split[2]) || split[3] !="")
            throw InvalidInstruction(GetLineOfFile);
        if(stoll(split[1]) <=0 || stoll(split[1]) > 999999 )
            throw InvalidInstruction(GetLineOfFile);

        manage.putCapacity(stoi(split[1]));
        manage.obj = new Object[manage.getCapacity()];
        manage.ci = new int[1];
        if(stoll(split[2]) > 999999 || stoll(split[2]) < 0)
            throw InvalidInstruction(GetLineOfFile);
        manage.ci[0] = stoi(split[2]);
        manage.putNameSearch("LINEAR");
    }
    else if(split[0] == "QUADRATIC"){
        if(!checkInt(split[1]) || !checkInt(split[2]) || !checkInt(split[3]))
            throw InvalidInstruction(GetLineOfFile);
        if(stoll(split[1]) > 999999 || stoll(split[1]) <=0)
            throw InvalidInstruction(GetLineOfFile);
        manage.putCapacity(stoi(split[1]));
        manage.obj = new Object[manage.getCapacity()];
        manage.ci = new int[2];

        if(stoll(split[2]) > 999999 || stoll(split[2]) <0)
            throw InvalidInstruction(GetLineOfFile);
        if( stoll(split[3]) > 999999 || stoll(split[3]) < 0)
            throw InvalidInstruction(GetLineOfFile);
        manage.ci[0] = stoi(split[2]);
        manage.ci[1] = stoi(split[3]);
        manage.putNameSearch("QUADRATIC");
    }
    else if(split[0] == "DOUBLE"){
        if(!checkInt(split[1]) || !checkInt(split[2]) ||split[3]!="")
            throw InvalidInstruction(GetLineOfFile);
        if(stoll(split[1]) > 999999 || stoll(split[1]) <=2)
            throw InvalidInstruction(GetLineOfFile);
        manage.putCapacity(stoi(split[1]));
        manage.obj = new Object[manage.getCapacity()];
        manage.ci = new int[1];
        if(stoll(split[2]) > 999999 || stoll(split[2]) < 0)
            throw InvalidInstruction(GetLineOfFile);
        manage.ci[0] = stoi(split[2]);
        manage.putNameSearch("DOUBLE");
    }
    else{
        throw InvalidInstruction(GetLineOfFile);
    }

    while(ifs.good()){
        getline(ifs, GetLineOfFile);
        checkStatement(split, GetLineOfFile);
        manage.reputCount();
/////////////////////////////////////////////////////////////////////////
        if(split[0] == "INSERT"){
            if(split[1] =="" || split[3] !="")
                throw InvalidInstruction(GetLineOfFile);
            if(!checkName(split[1]))
                throw InvalidInstruction(GetLineOfFile);
            if(split[2] != ""){
                if(!checkInt(split[2]))
                    throw InvalidInstruction(GetLineOfFile);
                if(manage.getCurrentBlock() != 0)
                    throw InvalidDeclaration(split[1]);
            }
            int i = manage.searchKey(manage.getCurrentBlock(), split[1],0);
            if (i == -1)
                throw Redeclared(split[1]);
            else if(i == -2)
                throw Overflow(GetLineOfFile);
            else{
                if(split[2] != "" && manage.getCurrentBlock() != 0)
                    throw InvalidDeclaration(split[1]);
                // gan gia tri vao hashtable
                manage.obj[i].putName(split[1]);
                manage.obj[i].putBlock(manage.getCurrentBlock());
                if(split[2] == "")
                    manage.obj[i].putParameter(-1);
                else{
                    manage.obj[i].putParameter(stoi(split[2]));
                }
                manage.addNode(split[1], manage.getCurrentBlock());
                cout<<manage.getCount()<<endl;  // in so slot phai di qua
                manage.reputCount();
            }
        }
/////////////////////////////////////////////////////////////////////////
        else if(split[0] == "ASSIGN"){
            // ASSIGN x func(x1,x2,x3)
            if(split[3] !="" || split[1] =="" || split[2] =="")
                throw InvalidInstruction(GetLineOfFile);
            string returnType = "";
            if(checkName(split[2])){
                int index2 = -2;

                int block =  manage.findNode(split[2]);
                if(block == -1)
                    throw Undeclared(split[2]);
                index2 = manage.searchKey(block, split[2], 1);

                if(index2 == -2)
                    throw Undeclared(split[2]);
                if(manage.obj[index2].getParameter() >=0)
                    throw InvalidInstruction(GetLineOfFile);
                
                if(!checkName(split[1]))
                    throw InvalidInstruction(GetLineOfFile);
                int index1 = -2;

                // tim split[1]
                block = manage.findNode(split[1]);
                if(block == -1)
                    throw Undeclared(split[1]);
                index1 = manage.searchKey(block, split[1],1);
                if(index1 == -2)
                    throw Undeclared(split[1]);
                if(manage.obj[index1].getReturnType() == "void" || manage.obj[index2].getReturnType() == "void")
                    throw TypeMismatch(GetLineOfFile);
                if(manage.obj[index1].getParameter() >=0)
                    throw TypeMismatch(GetLineOfFile);
                if(manage.obj[index1].getReturnType() == ""){
                    if(manage.obj[index2].getReturnType() =="")
                        throw TypeCannotBeInferred(GetLineOfFile);
                    else{
                        manage.obj[index1].putReturnType(manage.obj[index2].getReturnType());
                    }
                }
                else{
                    if(manage.obj[index2].getReturnType() == ""){
                        manage.obj[index2].putReturnType(manage.obj[index1].getReturnType());
                    }
                    else{
                        if(manage.obj[index2].getReturnType() != manage.obj[index1].getReturnType())
                            throw TypeMismatch(GetLineOfFile);
                    }
                }

            }
            else if(checkInt(split[2])){
                int index1 = -2;
                // Tim kiem danh hieu 1
                if(!checkName(split[1]))
                    throw InvalidInstruction(GetLineOfFile);
                
                int block = manage.findNode(split[1]);
                if(block == -1)
                    throw Undeclared(split[1]);
                // Thu xem danh hieu 1 da khai bao chwa
                index1 = manage.searchKey(block, split[1], 1);
                if(index1 == -2)
                    throw Undeclared(split[1]);
                // no co la kieu ham hay khon
                if(manage.obj[index1].getParameter() >=0)
                    throw TypeMismatch(GetLineOfFile);
                // kieu tra ve co phu hop khong
                if(manage.obj[index1].getReturnType() == "")
                    manage.obj[index1].putReturnType("number");
                else{
                    if(manage.obj[index1].getReturnType() != "number")
                        throw TypeMismatch(GetLineOfFile);
                }
            }
            else if(checkString(split[2])){
                int index1 = -2;
                // Tim kiem danh hieu 1
                if(!checkName(split[1]))
                    throw InvalidInstruction(GetLineOfFile);
                int block = manage.findNode(split[1]);
                if(block == -1)
                    throw Undeclared(split[1]);
                index1 = manage.searchKey(block,split[1],1);
                // Thu xem danh hieu 1 da khai bao chwa
                if(index1 == -2)
                    throw Undeclared(split[1]);
                // no co la kieu ham hay khon
                if(manage.obj[index1].getParameter() >=0)
                    throw TypeMismatch(GetLineOfFile);
                // kieu tra ve co phu hop khong
                if(manage.obj[index1].getReturnType() == "")
                    manage.obj[index1].putReturnType("string");
                else{
                    if(manage.obj[index1].getReturnType() != "string")
                        throw TypeMismatch(GetLineOfFile);
                }
            }
            else{
                if(split[2][int(split[2].size()) -1 ] != ')')
                    throw InvalidInstruction(GetLineOfFile);
                int index2 = manage.checkFuct(split[2].substr(0,int(split[2].size())-1), GetLineOfFile,0);
                if(manage.obj[index2].getReturnType() == "void" )
                    throw TypeMismatch(GetLineOfFile);
                if(!checkName(split[1]))
                    throw InvalidInstruction(GetLineOfFile);
                int index1 = -2;
                int block = manage.findNode(split[1]);
                if(block == -1)
                    throw Undeclared(split[1]);
                index1 = manage.searchKey(block, split[1],1);

                if(index1 == -2)
                    throw Undeclared(split[1]);

                if(manage.obj[index1].getParameter()>=0)
                    throw TypeMismatch(GetLineOfFile);
                if(manage.obj[index1].getReturnType() == "void")
                    throw TypeMismatch(GetLineOfFile);
                if(manage.obj[index1].getReturnType() ==""){
                    if(manage.obj[index2].getReturnType()=="")
                        throw TypeCannotBeInferred(GetLineOfFile);
                    else{
                        manage.obj[index1].putReturnType(manage.obj[index2].getReturnType());
                    }
                }
                else{
                    if(manage.obj[index2].getReturnType() == ""){
                        manage.obj[index2].putReturnType(manage.obj[index1].getReturnType());
                    }
                    else{
                        if(manage.obj[index1].getReturnType() != manage.obj[index2].getReturnType())
                            throw TypeMismatch(GetLineOfFile);
                    }
                }

            }
            cout<<manage.getCount()<<endl;
        }
/////////////////////////////////////////////////////////////////////////
        else if(split[0] == "CALL"){
            // vd: x(a1,a2,'fas',9)
            if(split[1] =="" || split[2] !="" || split[3] != "")
                throw InvalidInstruction(GetLineOfFile);


                
            int index = manage.checkFuct(split[1].substr(0, int(split[1].size())-1), GetLineOfFile,1);
            if(manage.obj[index].getReturnType() == "")
                manage.obj[index].putReturnType("void");
            else if(manage.obj[index].getReturnType() != "void")
                throw TypeMismatch(GetLineOfFile);
            cout<<manage.getCount()<<endl;
        }
/////////////////////////////////////////////////////////////////////////
        else if(split[0] == "BEGIN"){
            manage.putBEGIN();
        }
/////////////////////////////////////////////////////////////////////////
        else if(split[0] == "END"){
            manage.removeNode(manage.getCurrentBlock());
            manage.putEND();
        }
/////////////////////////////////////////////////////////////////////////
        else if(split[0] == "LOOKUP"){
            if(split[1] =="" || split[2]!="" || split[3] != "")
                throw InvalidInstruction(GetLineOfFile);
            if(!checkName(split[1]))
                throw InvalidInstruction(GetLineOfFile);
            int location = -2;

            int index = manage.findNode(split[1]);
            if(index == -1)
                throw Undeclared(split[1]);
            location = manage.searchKey(index, split[1],1);
            cout<<location<<endl;
        }
//////////////////////////////////////////////////////////////////////////
        else if(split[0] == "PRINT"){
            manage.print();
        }
        else{
            throw InvalidInstruction(GetLineOfFile);
        }
    }
    if(manage.getCurrentBlock() > 0)
        throw UnclosedBlock(manage.getCurrentBlock());

}
void Object::putName(string name){
    this->name = name;
}
void Object::putBlock(int block){
    this->block = block;
}
void Object::putReturnType(string ReturnType){
    this->ReturnType = ReturnType;
}
void Object::putParameter(int para){
    this->parameter = para;
    if(para > 0){
        this->ParaType = new string[para];
        for(int i = 0; i < para; i++){
            this->ParaType[i] ="";
        }
    }
}
int Object::getBlock(){return block;}
string Object::getName(){return name;}
int Object::getParameter(){
    return this->parameter;
}
string Object::getReturnType(){
    return this->ReturnType;
}

int SEARCH::searchKey(int block, string nameObj, int method = 0){
    // if method =0: find NULL bucket, if method =1: find key
    // Return key >=0 if found,
    // if redeclared return -1
    // if Overflow or not see value return -2
    SymbolTable temp;
    int ans = -2;
    if(nameSearch == "LINEAR"){
        int key = temp.Mahoa(nameObj, block, capacity);
        int h_k = key%capacity;
        int i = 0;
        int hp_i;
        while(true){
            hp_i = (h_k + ci[0]*i)% capacity;
            if(obj[hp_i].getName() =="" && method ==0){
                ans = hp_i;
                break;
            }
            else if(obj[hp_i].getBlock() == block && nameObj == obj[hp_i].getName() && method == 0){
                ans = -1;
                break;
            }
            else if(obj[hp_i].getBlock() == block && nameObj == obj[hp_i].getName() &&method == 1){
                ans = hp_i;
                break;
            }
            if(i == capacity){
                ans = -2;
                break;
            }
            count++;
            i++;
        }
    }
    else if(nameSearch == "QUADRATIC"){
        int key = temp.Mahoa(nameObj, block, capacity);
        int h_k = key%capacity;
        int i = 0;
        int hp_i;
        while(true){
            hp_i = (h_k + ci[0]*i + ci[1]*i*i)% capacity;
            if(obj[hp_i].getName() =="" && method ==0){
                ans = hp_i;
                break;
            }
            else if(obj[hp_i].getBlock()== block && nameObj == obj[hp_i].getName() && method == 0){
                ans = -1;
                break;
            }
            else if(obj[hp_i].getBlock() == block && nameObj == obj[hp_i].getName() && method == 1){
                ans = hp_i;
                break;
            }
            if(i == capacity){
                ans = -2;
                break;
            }
            count++;
            i++;
        }
    }
    else{
        int key1 = temp.Mahoa(nameObj, block, capacity);
        int key2 = temp.Mahoa(nameObj, block, capacity - 2);
        int h_k = key1%capacity;
        int h_k_2 = 1 + (key2 %(capacity-2));
        int i = 0;
        int hp_i;
        while(true){
            hp_i = (h_k + ci[0]*i*h_k_2)% capacity;
            if(obj[hp_i].getName() =="" && method ==0){
                ans = hp_i;
                break;
            }
            else if(obj[hp_i].getBlock() == block && nameObj == obj[hp_i].getName() && method == 0){
                ans = -1;
                break;
            }
            else if(obj[hp_i].getBlock() == block && nameObj == obj[hp_i].getName() && method == 1){
                ans = hp_i;
                break;
            }
            if(i == capacity){
                ans = -2;
                break;
            }
            count++;
            i++;
        }
    }
    return ans;

}

int SEARCH::checkFuct(string s, string GetLineOfFile, int flag = 0){
    // IF flag = 1: that means use for CALL function, else for check in ASSIGN
    SymbolTable temp;
    string needAnalys = "";
    string nameFunc ="";
    for(int i = 0; i < int(s.size()); i++){
        if(s[i]== '('){
            nameFunc = s.substr(0,i);
            needAnalys = s.substr(i+1, int(s.size()) -1 - i);
            break;
        }
    }
    if(!temp.checkName(nameFunc)){
        throw InvalidInstruction(GetLineOfFile);
    }
    int index = -2;
    int block = findNode(nameFunc);
    if(block == -1)
        throw Undeclared(nameFunc);
    else if(block !=0){
        if(flag == 1)
            throw TypeMismatch(GetLineOfFile);
    }

    index = searchKey(block, nameFunc, 1);
    if(index == -2)
        throw Undeclared(nameFunc);
    // Xet xem no co phai ham khong: 
    if(obj[index].getParameter() < 0)
        throw TypeMismatch(GetLineOfFile);


    // Neu danh sach tham so rong
    if(obj[index].getParameter() == 0){
        if(needAnalys==""){
            return index;
        }
        else{
            throw TypeMismatch(GetLineOfFile);
        }
    }
    else if(needAnalys == "" && obj[index].getParameter() !=0)
        throw TypeMismatch(GetLineOfFile);

    // Dem so luong tham so truyen vao ham, so sanh voi ham
    int countPara = 1;
    bool daunhay = false;
    for(int i = 0 ; i < int(needAnalys.size()); i++){
        if(needAnalys[i] == '\'' &&!daunhay){
            daunhay = true;
            continue;
        }
        if(daunhay){
            if(needAnalys[i] == '\'')
                daunhay = false;
        }
        else{
            if(needAnalys[i] == ',' && needAnalys[i+1] == ',')
                throw InvalidInstruction(GetLineOfFile);
            else if(needAnalys[i] ==',' && i == int(needAnalys.size()) -1)
                throw InvalidInstruction(GetLineOfFile);
            if(needAnalys[i] == ',')
                countPara++;
        }
    }
    if(countPara != obj[index].getParameter())
        throw TypeMismatch(GetLineOfFile);

    // Vong lap so sanh tung tham so va bat dau suy dien kieu 
    daunhay = false;
    for(int i = 0; i < obj[index].getParameter(); i++){
        if(needAnalys.find(',') == std::string::npos){
            string value = needAnalys.substr(0,int(needAnalys.size()));
            // Neu la danh hieu
            if(temp.checkName(value)){
                int key = -2;
                int block = findNode(value);
                if(block == -1)
                    throw Undeclared(value);
                key = searchKey(block, value,1);
                if(key == -2)
                    throw Undeclared(value);
                if(obj[key].getParameter() != -1){
                    throw TypeMismatch(GetLineOfFile);
                }
                // So sanh hai parameter cua object va function
                if(obj[key].getReturnType() == "void")
                    throw TypeMismatch(GetLineOfFile);
                if(obj[index].ParaType[i] ==""){
                    if(obj[key].getReturnType() =="")
                        throw TypeCannotBeInferred(GetLineOfFile);
                    else{
                        obj[index].ParaType[i] = obj[key].getReturnType();
                    }
                }
                else{
                    if(obj[key].getReturnType() ==""){
                        obj[key].putReturnType(obj[index].ParaType[i]);
                    }
                    else{
                        if(obj[key].getReturnType() != obj[index].ParaType[i])
                            throw TypeMismatch(GetLineOfFile);
                    }
                }
            }
            // Neu la number
            else if(temp.checkInt(value)){
                if(obj[index].ParaType[i] == "")
                    obj[index].ParaType[i] = "number";
                else if(obj[index].ParaType[i] == "number")
                    break;
                else 
                    throw TypeMismatch(GetLineOfFile);
            }
            // Neu la string
            else if(temp.checkString(value)){
                if(obj[index].ParaType[i] == ""){
                    obj[index].ParaType[i] = "string";
                }
                else if(obj[index].ParaType[i] == "string")
                    break;
                else 
                    throw TypeMismatch(GetLineOfFile);
            }
            else{
                throw InvalidInstruction(GetLineOfFile);
            }
            break;

        }
        else{
            string value = needAnalys.substr(0,needAnalys.find(','));
            needAnalys = needAnalys.substr(needAnalys.find(',')+1 , int(needAnalys.size()) - needAnalys.find(',')-1);
            // Neu la danh hieu
            if(temp.checkName(value)){
                int key = -2;
                int block = findNode(value);
                if(block == -1)
                    throw Undeclared(value);
                key = searchKey(block, value, 1);
                if(key == -2)
                    throw Undeclared(value);
                if(obj[key].getParameter() != -1){
                    throw TypeMismatch(GetLineOfFile);
                }
                // So sanh hai parameter cua object va function
                if(obj[key].getReturnType() == "void")
                    throw TypeMismatch(GetLineOfFile);
                if(obj[index].ParaType[i] ==""){
                    if(obj[key].getReturnType() =="")
                        throw TypeCannotBeInferred(GetLineOfFile);
                    else{
                        obj[index].ParaType[i] = obj[key].getReturnType();
                    }
                }
                else{
                    if(obj[key].getReturnType() ==""){
                        obj[key].putReturnType(obj[index].ParaType[i]);
                    }
                    else{
                        if(obj[key].getReturnType() != obj[index].ParaType[i])
                            throw TypeMismatch(GetLineOfFile);
                    }
                }
            }
            // Neu la number
            else if(temp.checkInt(value)){
                if(obj[index].ParaType[i] == "")
                    obj[index].ParaType[i] = "number";
                else if(obj[index].ParaType[i] == "number");
                else 
                    throw TypeMismatch(GetLineOfFile);
            }
            // Neu la string
            else if(temp.checkString(value)){
                if(obj[index].ParaType[i] == ""){
                    obj[index].ParaType[i] = "string";
                }
                else if(obj[index].ParaType[i] == "string");
                else 
                    throw TypeMismatch(GetLineOfFile);
            }
            else{
                throw InvalidInstruction(GetLineOfFile);
            }
        }
    }
    return index;
}

void SEARCH::addNode(string name, int block){
    if(!head){
        head = new node(name, block);
        tail = head;
        return;
    }
    else{
        tail->next = new node(name,block);
        tail->next->prev = tail;
        tail = tail->next;
        return;
    }
}

void SEARCH::removeNode(int block){
    if(!tail)
        return;
    node *temp = tail;
    while(temp){
        if(temp->block != block)
            break;
        tail = tail->prev;
        if(tail)
            tail->next = NULL;
        temp->prev = NULL;
        delete temp;
        if(tail)
            temp = tail;
        else 
            temp = NULL;
    }
    if(!tail){
        head = NULL;
    }
}

int SEARCH::findNode(string name){
    node *temp = tail;
    while(temp){
        if(temp->name == name){
            return temp->block;
        }
        temp = temp->prev;
    }
    return -1; // Not found
}

bool SymbolTable::checkStatement(string *ar, string s){
   // return false: InvalidInstruction
   ar[0] = "";
   ar[1] = "";
   ar[2] = "";
   ar[3] = "";
   bool daunhay1 = false;
   int j = 0;
   int preCount = 0;
   for(int i = 0; i < int(s.size()); i++){
        if(s[i] ==' ' && s[i+1] == ' '&& !daunhay1)
            return false;
        if(s[i] == ' ' && i == int(s.size()) - 1)
            return false;
        if(s[i] == '\'' && !daunhay1){
            daunhay1 = true;
            continue;
        }
        else if(s[i] == '\'' && daunhay1){
            daunhay1 = false;
        }
        if(daunhay1 && i != int(s.size()) - 1)
            continue;
        if(s[i] == ' ' || i == int(s.size()) - 1){  
            if (j >=4)
                return false;
            if(s[i] == ' '){
                ar[j] = s.substr(preCount,i - preCount);
                preCount = i+1;
                j++;
            }
            else{
                ar[j] = s.substr(preCount, int(s.size()) - preCount);
                break;
            }
        }
   }
   return true;
}

bool SymbolTable::checkInt(string s){
    if(s=="")
        return false;
    for(int i = 0; i < int(s.size()); i++){
        if(s[i] <'0' || s[i] >'9')
            return false;
    }
    return true;
}bool SymbolTable::checkString(string s){
    if(s=="")  
        return false;
    if(s[0] != '\'' || s[int(s.size()) - 1] != '\'')
        return false;
    for(int i = 1; i < int(s.size()) -1; i++){
        if((s[i] >='0' && s[i]<='9') || (s[i] >='a' && s[i] <='z') ||(s[i] >='A' && s[i] <='Z') || (s[i] ==' '))
            continue;
        else 
            return false;
    }
    return true;
}

bool SymbolTable::checkName(string s){
    if(s =="")
        return false;
    if(s == "string" || s == "void" || s =="number")
        return false;
    if(s[0] <'a' || s[0] >'z')
        return false;
    for(int i = 1; i < int(s.size()); i++){
        if((s[i] >='a' && s[i] <='z') || (s[i] >='A' && s[i]<='Z') || (s[i]>='0' && s[i] <='9')||s[i] == '_')
            continue;
        else
            return false;
    }
    return true;
}

int SymbolTable::Mahoa(string s, int block, int capa){
    int temp = block;
    int t;
    for(int i = 0; i < int(s.size()); i++){
        t = int(s[i] - 48);
        if(t >=10){
            temp = ((temp*100)%capa+t%capa)%capa;
        }
        else{
            temp = ((temp *10)%capa +t%capa)%capa;
        }
    }
    return temp%capa;
}
