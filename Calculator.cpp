
#include "Calculator.h"

using namespace std;

vector<string> parser(string raw, string previousResult){
    string parsed;
    string temp;
    vector<string> splitUp;
    bool insertedOperator = false;// True if we just inserted an operator. Prevents back to back operators
    int currNumNegatives = 0;//if odd, number is negative. even or 0 = positive
    int numOpenParentheses = 0;
    bool firstEquation = (previousResult == "")? true : false;
    
    

    //Remove spaces
    for (int i = 0; i < raw.length(); i++) {
        if(raw[i] != ' '){
            parsed += raw[i];
        }
    }

    //Adding previous result to the equation if there is an operator at the front
    if((!isdigit(parsed[0]))&&(!isalpha(parsed[0]))&&!firstEquation){
        parsed = previousResult + parsed;
    }
    
    //Handle negatives.
    //Odd number of negatives(5---3) turned to just one negative(5-3);
    //Even number of negatives(5--3) turned to just a plus.(5+3);
    for (int i = 0; i < parsed.length(); i++) {
        if(parsed[i]=='-'){
            currNumNegatives++;
        }
        //Finished counting negatives w/ odd number of them. add only 1
        else if(currNumNegatives%2){
            temp+='-';
            temp+=parsed[i];
            currNumNegatives = 0;
        }
        //Finished counting negatives w/ even number. Add 1 +
        else if(currNumNegatives>0){
            temp+='+';
            temp+=parsed[i];
            currNumNegatives = 0;
        }
        //Wasn't counting negatives.
        else{
            temp+=parsed[i];
        }
    }
    
    //Ensuring matching parentheses
    for (int i = 0; i < parsed.length(); i++) {
        if(parsed[i]=='('){
            numOpenParentheses++;
        }
        if(parsed[i]==')'){
            numOpenParentheses--;
        }
    }
    if(numOpenParentheses != 0){
        cout<<"Error! You have mismatching parentheses!"<<endl;
        throw 20;
    }
    
    parsed = temp;//Parsed now holds the equation w/o spaces and with proper negatives
    temp ="";

    for (int i = 0; i < parsed.length(); i++) {
        
        //Is a number, add it to temp
        if(isdigit(parsed[i])){
            
            //Number directly after a ). This indicates multiplication
            //ex. (5+3)2 would be 16
            if((i!=0)&&parsed[i-1] == ')'){
                splitUp.push_back("*");
            }
            
            temp+=parsed[i];
            insertedOperator = false;
        }
        
        //Handling decimals
        else if(parsed[i]=='.'){
            if(!isdigit(parsed[i+1])){
                cout<<"Can't have a decimal point without a decimal!"<<endl;
                throw 20;
            }
            temp+=parsed[i];
            insertedOperator = false;
        }
        
        else{
            //Checking first/last character. Typically don't want a nonnumber here
            if((i == 0)||(i == (parsed.length() -1))){
                
                //Unless it's a plus. We take this as user stating the number is positive
                if(i==0 && parsed[i] == '+'){
                    continue;//continue for loop, basically skipping this because it just indicates a number is positive.
                }
                
                //Or it's a minus. Also assume it's for signage
                if(i == 0 && parsed[i]=='-'){
                    temp+=parsed[i];
                    insertedOperator = true;
                    continue;
                }
                
                //Open parenthesis can be at beginning as well
                if(i == 0 && parsed[i]=='('){
                    insertedOperator = false;
                    temp = parsed[i];
                    splitUp.push_back(temp);//add operator to vector
                    temp="";
                    continue;
                }
                
                //Closed parenthesis is the only nonnumber that can be at end
                if(i == parsed.length()-1 && parsed[i] == ')'){
                    insertedOperator = false;
                    splitUp.push_back(temp);//add number to vector
                    temp = parsed[i];
                    continue;
                    //Since this is the last thing, don't push the parenthesis.
                    //We do an extra push outside of the loop
                }
                    cout<<"Error! Non-number at beginning or end of an equation!"<<endl;
                    cout<<"It was: "<<parsed[i]<<endl;
                    throw 20;
            }
            
            //Can't have any letters in our equations.
            if(isalpha(parsed[i])){
                cout<<"Error! A non-digit / non-operator is in the equation!"<<endl;
                throw 20;
            }
            
            //There is an operator. Need to add temp to vector and clear it.
            else{
                
                //Set insertedOperator to false because the parentheses break the equation up
                if(parsed[i]=='('){
                    insertedOperator = false;
                    splitUp.push_back(temp);//add number to vector
                    
                    //If number before parenthesis, add *
                    //ex. 5(5+5) is 50
                    if(isdigit(parsed[i-1])){
                        splitUp.push_back("*");
                    }
                    
                    temp = parsed[i];
                    splitUp.push_back(temp);//add parenthesis to vector
                    temp="";
                }
                else if(parsed[i] == ')'){
                    //can't have an operator at end of equation.
                    if(insertedOperator){
                        cout<<"Error! Can't have operator at end of parenthesized equation!"<<endl;
                        cout<<"It was: "<<parsed[i-1]<<endl;
                        throw 20;
                    }
                    insertedOperator = false;
                    splitUp.push_back(temp);//add number to vector
                    temp = parsed[i];
                    splitUp.push_back(temp);//add operator to vector
                    temp="";
                }
                //Attempting to have operators back to back, don't allow this
                //Unless it's a negative or positive next, then we're fine.
                    //These just specify if the next # is +/-
                else if(insertedOperator&&parsed[i]!='-'){
                    //Specifies the next operand is positive, can ignore it.
                    if(parsed[i]=='+'){
                        continue;
                    }
                    cout<<"Error! Can't have multiple operators back to back!"<<endl;
                    throw 20;
                }
                
                //Right handed operand should be negative.
                else if(insertedOperator && parsed[i]=='-'){
                    temp+=parsed[i];
                }
                //An operator after numbers. just add it.
                else{
                    //Gotta check for parenthesis stuff
                    if(parsed[i-1]=='('){
                        if(parsed[i]=='-'){
                            temp+=parsed[i];
                            insertedOperator = true;
                            continue;
                        }
                        //Skip, indicates positive.
                        else if(parsed[i] == '+'){
                            continue;
                        }
                        else{
                            cout<<"Error! Non-number at beginning of parenthesized equation!"<<endl;
                            cout<<"It was: "<<parsed[i]<<endl;
                            throw 20;
                        }
                    }
                    //Prevents us from pushing a space into the vector.
                    else if(parsed[i-1]!=')'){
                        splitUp.push_back(temp);//add number to vector
                    }
                    temp = parsed[i];
                    splitUp.push_back(temp);//add operator to vector
                    temp="";
                    insertedOperator = true;
                }
            }
        }
    }
    //At end with no errors, add final number
    splitUp.push_back(temp);
    if(numOpenParentheses != 0){
        cout<<"Error! The equation has mismatched parentheses!"<<endl;
        throw 20;
    }
    
    //Displaying the vector for debugging purposes: 
    /*
    cout<<"What the vector looks like after parsing: "<<endl;
    for (int i = 0; i < splitUp.size(); i++) {
        cout<<i<<". "<<splitUp[i]<<endl;
    }*/
    return splitUp;
    
}

string calc(vector<string> equation){
    /*/ Debugging stuff
    cout<<"CURRENT EQUATION VECTOR: "<<endl;
    for (int i = 0; i < equation.size(); i++) {
        cout<<i<<". "<<equation[i]<<endl;
    }*/
    double result,left, right;
    int firstIndex, lastIndex;
    string temp;
    vector<string> inParentheses;//Will hold the smaller equations that are in the parentheses.
    
    //Iterate through and do all parentheses.
    for (int i = 0; i < equation.size(); i++) {
        if(equation[i] == "("){
            firstIndex = i+1;//first part of smaller equation.
        }
        //At end of smaller equation, copy it to new vector and get result.
        else if(equation[i]==")"){
            lastIndex = i-1;//last part of smaller equation.
            for (int x = firstIndex; x <= lastIndex; x++) {
                inParentheses.push_back(equation[x]);
            }
            result = stod(calc(inParentheses));
            equation.erase(equation.begin()+firstIndex,equation.begin()+lastIndex+3);
            equation[firstIndex-2] = to_string(result);
            i = firstIndex;
            /*  //Debugging stuff
            cout<<"equation after parentheses calculated: "<<endl;
            for (int z = 0; z < equation.size(); z++) {
                cout<<z<<". "<<equation[z]<<endl;
            }
            cout<<endl;*/
        }
    }
    
    //Iterate through and do all exponents
    for (int i = 0; i < equation.size(); i++) {
        if(equation[i] == "^"){
            firstIndex = i-1;
            lastIndex = i +1;
            left =stod(equation[firstIndex]);
            right = stod(equation[lastIndex]);
            result = pow(left, right);
            equation.erase(equation.begin()+i,equation.begin()+i+2);
            equation[i-1] = to_string(result);
            i=firstIndex;
        }
    }

    //Iterate through and do all muliplication/division/modulo
    for (int i = 0; i < equation.size(); i++) {
        //multiply or divide. Place result in left number's index and delete the two after(operator and right number)
        if((equation[i] == "*")||(equation[i]=="/")||(equation[i]=="%")){
            firstIndex = i-1;
            lastIndex = i+1;
            left =stod(equation[firstIndex]);
            right = stod(equation[lastIndex]);
            if(equation[i] == "*"){
                result = left * right;
            }
            else{
                if(right == 0){
                    cout<<"Error: cannot divide by 0!"<<endl;
                    throw 20;
                }
                if(equation[i] == "/"){
                    result = left / right;
                }
                else{
                    checkInt(left);
                    checkInt(right);
                    result = int(left) % int(right);
                }
            }
            
            equation.erase(equation.begin()+i,equation.begin()+i+2);
            equation[i-1] = to_string(result);
            
            i=firstIndex;
        }
    }
    
    //Iterate through and do all addition/subtraction
    for (int i = 0; i < equation.size(); i++) {
        if((equation[i]=="+")||(equation[i]=="-")){
            firstIndex = i-1;
            lastIndex = i+1;
            left =stod(equation[firstIndex]);
            temp = equation[lastIndex];
            right = stod(equation[lastIndex]);
            if(equation[i]=="+"){
                result = left + right;
            }
            else{
                result = left - right;
            }
            equation.erase(equation.begin()+i,equation.begin()+i+2);
            equation[i-1] = to_string(result);
        }
    }
    
    /*
    cout<<"Inside the vector: "<<endl;
    for (int i = 0; i < equation.size(); i++) {
        cout<<stod(equation[i])<<endl;
    }*/
    return equation[0];
}

void checkInt(double Var){
    int temp = Var;
    double remains = Var - temp;
    
    if(remains != 0){
        cout<<"Error! Can't use a noninteger in modulo!"<<endl;
        throw 20;
    }
}