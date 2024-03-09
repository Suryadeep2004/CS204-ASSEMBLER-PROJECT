#include <bits/stdc++.h>
#include "assembler.hpp"
using namespace std;

int main(){
    string file1 = "input.asm"; //file name to be input
    ifstream inFile(file1); //creating file object to read the file
    string line; //container for each line of the file
    int programCounter=0; //program counter initialised to 0

    while (getline(inFile, line)) {
        line=removeCommasAndSpaces(line); //removing unnecessary spaces and commas
        if (line==".data") isText=false; //if .data found then the upcoming lines will be part of data segment
        else if (line==".text") isText=true; //if .text found then the upcoming lines will be part of text segment
        else if (line!="" && line[0]!='#'){ //making sure line left is not empty or a comment
            if (isText){ //checking which segment the current line of code belongs to
                if (line.find(':')!=string::npos){ //checking whether there is a label
                    string label = line.substr(0, line.find(':')); //extracting the label
                    label=removeCommasAndSpaces(label); //removing unnecessary spaces between label name and colon 
                    line=line.substr(line.find(':')+1); //extracting the remaining code 
                    line=removeCommasAndSpaces(line); //removing unnecessary spaces between colon and name
                    labels[label]=programCounter; //inserting in hash map the program counter corresponding each label
                    textLines.push_back(line); //putting the text code into a vector for later traversal
                }
                else textLines.push_back(line); //putting the text code into a vector for later traversal
                programCounter+=4; //incrementing the program counter
            }
            else dataLines.push_back(line); //putting the data code into a vector for later traversal
        }
    }
    inFile.close(); //closing the file object
    
     // Print memory allocated in the .data segment
cout << "Memory allocated in the .data segment:" << endl;
 int dataAddress =268435452; // Start address for data segment
 int l=0;
for (const string& data : dataLines) {
   
    // Split the line into tokens
    vector<string> tokens = splitString(data);
   if(tokens[0].find(".word")!=-1){
       if(l==0){
        dataAddress+=0;
        l++;
    }
    for (const string& token : tokens) {
        if(token==tokens[0])continue;
        dataAddress += 4;
        cout << decimalToHex(dataAddress) << " : " << token << endl;
         // Increment address for next data
    }
   }
   else if(tokens[0].find(".asciz")!=-1||tokens[0].find(".asciiz")!=-1){
       if(l==0){
        dataAddress+=0;
        l++;
    }
    for (const string& token : tokens) {
        if(token==tokens[0])continue;
        dataAddress += 4;
        cout << decimalToHex(dataAddress) << " : " << token << endl;
         // Increment address for next data
    }
   }
   else if(tokens[0].find(".dword")!=-1){
       if(l==0){
        dataAddress-=4;
        l++;
    }
    for (const string& token : tokens) {
        if(token==tokens[0])continue;
        dataAddress += 8;
        cout << decimalToHex(dataAddress) << " : " << token << endl;
         // Increment address for next data
    }
   }
   else if(tokens[0].find(".byte")!=-1){
       if(l==0){
        dataAddress+=3;
        l++;
    }
    for (const string& token : tokens) {
        if(token==tokens[0])continue;
        dataAddress += 1;
        cout << decimalToHex(dataAddress) << " : " << token << endl;
         // Increment address for next data
    }
   }
   else if(tokens[0].find(".half")!=-1){
       if(l==0){
        dataAddress+=2;
        l++;
    }
    for (const string& token : tokens) {
        if(token==tokens[0])continue;
        dataAddress += 2;
        cout << decimalToHex(dataAddress) << " : " << token << endl;
         // Increment address for next data
    }
   }
    
    
    
    
    
    
    
    
    
    
}


    //dummy output to check if everything is fine until now
    // for (string str : dataLines) cout<<str<<endl;
    // cout<<endl;
    // for (string str : textLines) cout<<str<<endl;
    // cout<<endl;
    // for (auto x : labels) cout<<x.first<<" "<<x.second<<endl;

   
    //traversing through the text segment of the code to produce it's corresponding machine code
    cout<<endl;
    cout<<"Machine Code for the instructions in .text segment:"<<endl;
    programCounter=0; //program counter initialised to 0
    for (int k = 0; k<textLines.size(); k++){
        vector<string> words=splitString(textLines[k]); //splitting the text line into tokens
        words[0]=toLowercase(words[0]); //converting the instruction to lowercase
        if (r.find(words[0])!=r.end()){ //checking if instruction is R type
            if (words.size()<4){ //at least 3 arguements should be there else error
                cout<<"Error present at text line "<<k<<endl;
                return 0;
            }
            string instruction=words[0]; //obtaining the instruction word
            string rd=registerValue[words[1]]; //obtaining the destination register
            string rs1=registerValue[words[2]]; //obtaining the source register 1
            string rs2=registerValue[words[3]]; //obtaining the source register 2
            string opcode=opcodeValue[instruction]; //obtaining the opcode
            string funct3=funct3Value[instruction]; //obtaining the funct3
            string funct7=funct7Value[instruction]; //obtaining the funct7
            string binInstruction=funct7+rs2+rs1+funct3+rd+opcode; //combining all pieces of binary strings together
            string hexInstruction=binaryToHex(binInstruction); //converting the binary machine code to hexadecimal machine code
            cout<<decimalToHex(programCounter)<<" "<<hexInstruction<<endl;
        }
        else if (i.find(words[0])!=i.end()){ //checking if instruction is I type
            if (words.size()<4 || !(-2048<=stoi(words[3]) && stoi(words[3])<2048)){ //at least 3 arguements should be there and the immediate value should be bounded else error
                cout<<"Error present at text line "<<k<<endl;
                return 0;
            }
            string instruction=words[0]; //obtaining the instruction word
            string rd=registerValue[words[1]]; //obtaining the destination register
            string rs1=registerValue[words[2]]; //obtaining the source register 
            int immVal=stoi(words[3]); //obtaining the immediate value integer
            string imm;
            if (immVal>=0) imm=decimalToBinary(immVal); //converting the immediate integer to binary string
            else imm=decimalToBinary(4096+immVal);//if the immediate is negative then take 2's complement
            string opcode=opcodeValue[instruction]; //obtaining the opcode
            string funct3=funct3Value[instruction]; //obtaining the funct3
            string binInstruction=imm+rs1+funct3+rd+opcode; //combining all pieces of binary strings together
            string hexInstruction=binaryToHex(binInstruction); //converting the binary machine code to hexadecimal machine code
            cout<<decimalToHex(programCounter)<<" "<<hexInstruction<<endl;
        }
        else if (s.find(words[0])!=s.end()){ //checking if instruction is S type
            if (words.size()<3){ //at least 2 arguements should be there else error
                cout<<"Error present at text line "<<k<<endl;
                return 0;
            }
            string instruction=words[0]; //obtaining the instruction word
            string rs2=registerValue[words[1]]; //obtaining the source register 2
            string str=words[2]; //obtaining the second arguement 
            string opcode=opcodeValue[instruction]; //obtaining the opcode
            string funct3=funct3Value[instruction]; //obtaining the funct3
            string rs1, imm, imm1, imm2;
            extractImmediate(str, imm, rs1); //extracting the immediate and source register 1 from second arguement
            if (imm=="" || rs1==""){ //if any of the immediate or source register is empty then error
                cout<<"Error present at text line "<<k<<endl;
                return 0;
            }
            rs1=registerValue[rs1]; //obtaining the source register 1
            int immVal=stoi(imm); //converting the immediate string to integer
            if (immVal<-2048 || immVal>2047){ //the integer should be bounded else error
                cout<<"Error present at text line "<<k<<endl;
                return 0;
            }
            if (immVal>=0) imm=decimalToBinary(immVal); //converting the immediate integer to binary string
            else imm=decimalToBinary(4096+immVal); //if the immediate is negative then take 2's complement
            while (imm.size()<12) imm="0"+imm; 
            imm2=imm.substr(0, 7); //extracting the second part of immediate
            imm1=imm.substr(7); //extracting the first part of immediate
            string binInstruction=imm2+rs2+rs1+funct3+imm1+opcode; //combining all pieces of binary strings together
            string hexInstruction=binaryToHex(binInstruction); //converting the binary machine code to hexadecimal machine code
            cout<<decimalToHex(programCounter)<<" "<<hexInstruction<<endl;
        }
        else if (sb.find(words[0])!=sb.end()){ //checking if instruction is SB type
            if (words.size()<4){ //at least 3 arguements should be there else error
                cout<<"Error present at text line "<<k<<endl;
                return 0;
            }
            string instruction=words[0]; //obtaining the instruction word
            string rs1=registerValue[words[1]]; //obtaining the source register 1
            string rs2=registerValue[words[2]]; //obtaining the source register 2
            string label=words[3]; //obtaining the label
            string opcode=opcodeValue[instruction]; //obtaining the opcode
            string funct3=funct3Value[instruction]; //obtaining the funct3
            int immVal;
            if (isInteger(label)) immVal=stoi(label); //storing the immediate value if no label present
            else immVal=labels[label]-programCounter; //calculating the immediate value from label
            string imm;
            if (immVal>=0) imm=decimalToBinary(immVal); //converting the immediate integer to binary string
            else imm=decimalToBinary(4096+immVal);//if the immediate is negative then take 2's complement
            while (imm.size()<12) imm="0"+imm; 
            string binInstruction=imm[0]+imm.substr(2, 6)+rs2+rs1+funct3+imm.substr(7, 4)+imm[1]+opcode; //combining all pieces of binary strings together
            string hexInstruction=binaryToHex(binInstruction); //converting the binary machine code to hexadecimal machine code
            cout<<decimalToHex(programCounter)<<" "<<hexInstruction<<endl;
        }
        else if (u.find(words[0])!=u.end()){ //checking if instruction is U type
            if (words.size()<3){ //at least 2 arguements should be there else error
                cout<<"Error present at text line "<<k<<endl;
                return 0;
            }
            string instruction=words[0]; //obtaining the instruction word
            string opcode=opcodeValue[instruction]; //obtaining the opcode
            string rd=registerValue[words[1]]; //obtaining the destination register
            int immVal=stoi(words[2]);
            if (immVal<0 || immVal>1048575){ //the integer should be bounded else error
                cout<<"Error present at text line "<<k<<endl;
                return 0;
            }
            string imm=decimalToBinary(immVal); //converting immediate from integer decimal to string binary
            while (imm.size()<20) imm="0"+imm;
            string binInstruction=imm+rd+opcode; //combining all pieces of binary strings together
            string hexInstruction=binaryToHex(binInstruction); //converting the binary machine code to hexadecimal machine code
            cout<<decimalToHex(programCounter)<<" "<<hexInstruction<<endl;
        }
        else if (uj.find(words[0])!=uj.end()){ //checking if instruction is UJ type
            if (words.size()<3){ //at least 2 arguements should be there else error
                cout<<"Error present at text line "<<k<<endl;
                return 0;
            }
            string instruction=words[0]; //obtaining the instruction word
            string rd=registerValue[words[1]]; //obtaining the destination register 
            string label=words[2]; //obtaining the label
            string opcode=opcodeValue[instruction]; //obtaining the opcode
            int immVal;
            if (isInteger(label)) immVal=stoi(label); //storing the immediate value if no label present
            else immVal=labels[label]-programCounter; //calculating the immediate value from label
            string imm;
            if (immVal>=0) imm=decimalToBinary(immVal); //converting the immediate integer to binary string
            else imm=decimalToBinary(2097152+immVal); //if the immediate is negative then take 2's complement
            while (imm.size()<20) imm="0"+imm; 
            string binInstruction=imm[0]+imm.substr(10, 10)+imm[9]+imm.substr(1, 8)+rd+opcode; //combining all pieces of binary strings together
            string hexInstruction=binaryToHex(binInstruction); //converting the binary machine code to hexadecimal machine code
            cout<<decimalToHex(programCounter)<<" "<<hexInstruction<<endl;
        }
        else{ //if none then obviously an error
            cout<<"Error present at text line "<<k<<endl;
            return 0;
        }
        programCounter+=4;
    }
   

    return 0;
}
