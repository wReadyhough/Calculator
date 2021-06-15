
#include "Calculator.h"


//Available operators so far: +,-,*,/,(,),%,^


int main(){
    string equation;
    vector<string> parsed;
    string result;
    while(true){
        cout<<"Enter your equation or QUIT: ";
        getline(cin,equation);
        if(equation == "QUIT"){
            break;
        }
        try{
            parsed = parser(equation, result);
            result = calc(parsed);
            cout<<"The result of your equation is: "<<result<<endl;
        }
        catch(int errorCode){
            equation ="";
            parsed.clear();
        }
    }
    return 0;
}



