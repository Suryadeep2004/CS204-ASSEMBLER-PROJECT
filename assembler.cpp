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
    
    ofstream outfile("output.mc"); // Open output.mc file for writing

    // Print memory allocated in the .data segment
    outputLines.push_back("Memory allocated in the .data segment:");
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
                outputLines.push_back(decimalToHex(dataAddress) + " : " + token);
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
                outputLines.push_back(decimalToHex(dataAddress) + " : " + token);
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
                outputLines.push_back(decimalToHex(dataAddress) + " : " + token);
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
                outputLines.push_back(decimalToHex(dataAddress) + " : " + token);
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
                outputLines.push_back(decimalToHex(dataAddress) + " : " + token);
                // Increment address for next data
            }
        }
    }

    //traversing through the text segment of the code to produce it's corresponding machine code
    outputLines.push_back("\nMachine Code for the instructions in .text segment:");
    programCounter=0; //program counter initialised to 0
    for (int k = 0; k<textLines.size(); k++){
        vector<string> words=splitString(textLines[k]); //splitting the text line into tokens
        string instruction=words[0]; //obtaining the instruction word
        instruction=toLowercase(instruction); //converting the instruction to lowercase
        if (r.find(instruction)!=r.end()){ //checking if instruction is R type
            if (words.size()<4){ //at least 3 arguements should be there else error
                cout<<"Error present at text line "<<k<<endl;
                cout<<"R instruction should have 3 arguements"<<endl;
                anyError=true;
                break;
            }
            if (registerValue.find(words[1])==registerValue.end() || registerValue.find(words[2])==registerValue.end() || registerValue.find(words[3])==registerValue.end()){
                cout<<"Error present at text line "<<k<<endl;
                cout<<"Incorrect register value"<<endl;
                anyError=true;
                break;
            }
            string rd, rs1, rs2, opcode, funct3, funct7, binInstruction, hexInstruction;
            rd=registerValue[words[1]]; //obtaining the destination register
            rs1=registerValue[words[2]]; //obtaining the source register 1
            rs2=registerValue[words[3]]; //obtaining the source register 2
            opcode=opcodeValue[instruction]; //obtaining the opcode
            funct3=funct3Value[instruction]; //obtaining the funct3
            funct7=funct7Value[instruction]; //obtaining the funct7
            binInstruction=funct7+rs2+rs1+funct3+rd+opcode; //combining all pieces of binary strings together
            hexInstruction=binaryToHex(binInstruction); //converting the binary machine code to hexadecimal machine code
            outputLines.push_back(decimalToHex(programCounter)+" "+hexInstruction);
        }
        else if (i.find(instruction)!=i.end()){ //checking if instruction is I type
            if (instruction=="lb" || instruction=="ld" || instruction=="lh" || instruction=="lw"){
                if (words.size()<3){ //at least 3 arguements should be there else error
                    cout<<"Error present at text line "<<k<<endl;
                    cout<<"I instruction should have 3 arguements"<<endl;
                    anyError=true;
                    break;
                }
                string rs, rd, imm, opcode, funct3, str, binInstruction, hexInstruction;
                if (registerValue.find(words[1])==registerValue.end()){
                    cout<<"Error present at text line "<<k<<endl;
                    cout<<"Incorrect register value"<<endl;
                    anyError=true;
                    break;
                }
                rd=registerValue[words[1]]; //obtaining the source register 2
                opcode=opcodeValue[instruction]; //obtaining the opcode
                funct3=funct3Value[instruction]; //obtaining the funct3
                str=words[2]; //obtaining the second arguement 
                extractImmediate(str, imm, rs); //extracting the immediate and source register 1 from second arguement
                if (imm=="" || rs==""){ //if any of the immediate or source register is empty then error
                    cout<<"Error present at text line "<<k<<endl;
                    cout<<"I instruction should have 3 arguements"<<endl;
                    anyError=true;
                    break;
                }
                if (registerValue.find(rs)==registerValue.end()){
                    cout<<"Error present at text line "<<k<<endl;
                    cout<<"Incorrect register value"<<endl;
                    anyError=true;
                    break;
                }
                rs=registerValue[rs]; //obtaining the source register 1
                if (isHexadecimal(imm)) imm=hexToBinary(imm, 12);
                else{
                    int immVal=stoi(imm); //converting the immediate string to integer
                    if (immVal<-2048 || immVal>2047){ //the immediate value should be bounded else error
                        cout<<"Error present at text line "<<k<<endl;
                        cout<<"Immediate value is out of range"<<endl;
                        anyError=true;
                        break;
                    }
                    if (immVal>=0) imm=decimalToBinary(immVal); //converting the immediate integer to binary string
                    else imm=decimalToBinary(4096+immVal); //if the immediate is negative then take 2's complement
                    while (imm.size()<12) imm="0"+imm; 
                }
                binInstruction=imm+rs+funct3+rd+opcode; //combining all pieces of binary strings together
                hexInstruction=binaryToHex(binInstruction); //converting the binary machine code to hexadecimal machine code
                outputLines.push_back(decimalToHex(programCounter)+" "+hexInstruction);
            }
            else{
                if (words.size()<4){ //at least 3 arguements should be there
                    cout<<"Error present at text line "<<k<<endl;
                    cout<<"I instruction should have 3 arguements"<<endl;
                    cout<<textLines[k]<<endl;
                    anyError=true;
                    break;
                }
                if (registerValue.find(words[1])==registerValue.end() || registerValue.find(words[2])==registerValue.end()){
                    cout<<"Error present at text line "<<k<<endl;
                    cout<<"Incorrect register value"<<endl;
                    anyError=true;
                    break;
                }
                string rd, rs1, imm, opcode, funct3, binInstruction, hexInstruction;
                if (isHexadecimal(words[3])) imm=hexToBinary(words[3], 12);
                else{
                    int immVal=stoi(words[3]); //obtaining the immediate value integer
                    if (immVal<-2048 || immVal>2047){ //the immediate value should be bounded else error
                        cout<<"Error present at text line "<<k<<endl;
                        cout<<"Immediate value is out of range"<<endl;
                        anyError=true;
                        break;
                    }
                    if (immVal>=0) imm=decimalToBinary(immVal); //converting the immediate integer to binary string
                    else imm=decimalToBinary(4096+immVal);//if the immediate is negative then take 2's complement
                }
                rd=registerValue[words[1]]; //obtaining the destination register
                rs1=registerValue[words[2]]; //obtaining the source register 
                opcode=opcodeValue[instruction]; //obtaining the opcode
                funct3=funct3Value[instruction]; //obtaining the funct3
                binInstruction=imm+rs1+funct3+rd+opcode; //combining all pieces of binary strings together
                hexInstruction=binaryToHex(binInstruction); //converting the binary machine code to hexadecimal machine code
                outputLines.push_back(decimalToHex(programCounter)+" "+hexInstruction);
            }
        }
        else if (s.find(instruction)!=s.end()){ //checking if instruction is S type
            if (words.size()<3){ //at least 3 arguements should be there else error
                cout<<"Error present at text line "<<k<<endl;
                cout<<"S instruction should have 3 arguements"<<endl;
                anyError=true;
                break;
            }
            string rs1, rs2, imm, imm1, imm2, opcode, funct3, str, binInstruction, hexInstruction;
            if (registerValue.find(words[1])==registerValue.end()){
                cout<<"Error present at text line "<<k<<endl;
                cout<<"Incorrect register value"<<endl;
                anyError=true;
                break;
            }
            rs2=registerValue[words[1]]; //obtaining the source register 2
            opcode=opcodeValue[instruction]; //obtaining the opcode
            funct3=funct3Value[instruction]; //obtaining the funct3
            str=words[2]; //obtaining the second arguement 
            extractImmediate(str, imm, rs1); //extracting the immediate and source register 1 from second arguement
            if (imm=="" || rs1==""){ //if any of the immediate or source register is empty then error
                cout<<"Error present at text line "<<k<<endl;
                cout<<"S instruction should have 3 arguements"<<endl;
                anyError=true;
                break;
            }
            if (registerValue.find(rs1)==registerValue.end()){
                cout<<"Error present at text line "<<k<<endl;
                cout<<"Incorrect register value"<<endl;
                anyError=true;
                break;
            }
            rs1=registerValue[rs1]; //obtaining the source register 1
            if (isHexadecimal(imm)) imm=hexToBinary(imm, 12);
            else{
                int immVal=stoi(imm); //converting the immediate string to integer
                if (immVal<-2048 || immVal>2047){ //the immediate value should be bounded else error
                    cout<<"Error present at text line "<<k<<endl;
                    cout<<"Immediate value is out of range"<<endl;
                    anyError=true;
                    break;
                }
                if (immVal>=0) imm=decimalToBinary(immVal); //converting the immediate integer to binary string
                else imm=decimalToBinary(4096+immVal); //if the immediate is negative then take 2's complement
                while (imm.size()<12) imm="0"+imm; 
            }
            imm2=imm.substr(0, 7); //extracting the second part of immediate
            imm1=imm.substr(7); //extracting the first part of immediate
            binInstruction=imm2+rs2+rs1+funct3+imm1+opcode; //combining all pieces of binary strings together
            hexInstruction=binaryToHex(binInstruction); //converting the binary machine code to hexadecimal machine code
            outputLines.push_back(decimalToHex(programCounter)+" "+hexInstruction);
        }
        else if (sb.find(instruction)!=sb.end()){ //checking if instruction is SB type
            if (words.size()<4){ //at least 3 arguements should be there else error
                cout<<"Error present at text line "<<k<<endl;
                cout<<"SB instruction should have 3 arguements"<<endl;
                anyError=true;
                break;
            }
            if (registerValue.find(words[1])==registerValue.end() || registerValue.find(words[2])==registerValue.end()){
                cout<<"Error present at text line "<<k<<endl;
                cout<<"Incorrect register value"<<endl;
                anyError=true;
                break;
            } 
            int immVal;
            string rs1, rs2, label, opcode, funct3, imm, binInstruction, hexInstruction;
            rs1=registerValue[words[1]]; //obtaining the source register 1
            rs2=registerValue[words[2]]; //obtaining the source register 2
            label=words[3]; //obtaining the label
            opcode=opcodeValue[instruction]; //obtaining the opcode
            funct3=funct3Value[instruction]; //obtaining the funct3
            if (isHexadecimal(label)) imm=hexToBinary(label, 12);
            else{
                if (isInteger(label)) immVal=stoi(label); //storing the immediate value if no label present
                else immVal=labels[label]-programCounter; //calculating the immediate value from label
                if (immVal>=0) imm=decimalToBinary(immVal); //converting the immediate integer to binary string
                else imm=decimalToBinary(4096+immVal);//if the immediate is negative then take 2's complement
                while (imm.size()<13) imm="0"+imm; 
            }
            binInstruction=imm[0]+imm.substr(2, 6)+rs2+rs1+funct3+imm.substr(8, 4)+imm[1]+opcode; //combining all pieces of binary strings together
            hexInstruction=binaryToHex(binInstruction); //converting the binary machine code to hexadecimal machine code
            outputLines.push_back(decimalToHex(programCounter)+" "+hexInstruction);
        }
        else if (u.find(instruction)!=u.end()){ //checking if instruction is U type
            if (words.size()<3){ //at least 2 arguements should be there else error
                cout<<"Error present at text line "<<k<<endl;
                cout<<"U instruction should have 2 arguements"<<endl;
                anyError=true;
                break;
            }
            if (registerValue.find(words[1])==registerValue.end()){
                cout<<"Error present at text line "<<k<<endl;
                cout<<"Incorrect register value"<<endl;
                anyError=true;
                break;
            }
            string opcode, rd, imm, binInstruction, hexInstruction;
            if (isHexadecimal(words[2])) imm=hexToBinary(words[2], 20);
            else{
                int immVal=stoi(words[2]);
                if (immVal<0 || immVal>1048575){ //the immediate value should be bounded else error
                    cout<<"Error present at text line "<<k<<endl;
                    cout<<"Immediate value is out of range"<<endl;
                    anyError=true;
                    break;
                }
                imm=decimalToBinary(immVal); //converting immediate from integer decimal to string binary
                while (imm.size()<20) imm="0"+imm;
            }
            opcode=opcodeValue[instruction]; //obtaining the opcode
            rd=registerValue[words[1]]; //obtaining the destination register
            binInstruction=imm+rd+opcode; //combining all pieces of binary strings together
            hexInstruction=binaryToHex(binInstruction); //converting the binary machine code to hexadecimal machine code
            outputLines.push_back(decimalToHex(programCounter)+" "+hexInstruction);
        }
        else if (uj.find(instruction)!=uj.end()){ //checking if instruction is UJ type
            if (words.size()<3){ //at least 2 arguements should be there else error
                cout<<"Error present at text line "<<k<<endl;
                cout<<"UJ instruction should have 2 arguements"<<endl;
                anyError=true;
                break;
            }
            if (registerValue.find(words[1])==registerValue.end()){
                cout<<"Error present at text line "<<k<<endl;
                cout<<"Incorrect register value"<<endl;
                anyError=true;
                break;
            }
            int immVal;
            string rd, label, opcode, imm, binInstruction, hexInstruction;
            label=words[2]; //obtaining the label
            if (isHexadecimal(label)) imm=hexToBinary(label, 20);
            else{
                if (isInteger(label)) immVal=stoi(label); //storing the immediate value if no label present
                else immVal=labels[label]-programCounter; //calculating the immediate value from label
                if (immVal>=0) imm=decimalToBinary(immVal); //converting the immediate integer to binary string
                else imm=decimalToBinary(2097152+immVal); //if the immediate is negative then take 2's complement
                while (imm.size()<20) imm="0"+imm; 
            }
            rd=registerValue[words[1]]; //obtaining the destination register 
            opcode=opcodeValue[instruction]; //obtaining the opcode
            binInstruction=imm[0]+imm.substr(10, 10)+imm[9]+imm.substr(1, 8)+rd+opcode; //combining all pieces of binary strings together
            hexInstruction=binaryToHex(binInstruction); //converting the binary machine code to hexadecimal machine code
            outputLines.push_back(decimalToHex(programCounter)+" "+hexInstruction);
        }
        else{ //if none then obviously an error
            cout<<"Error present at text line "<<k<<endl;
            cout<<"Instruction format unknown"<<endl;
            anyError=true;
            break;
        }
        programCounter+=4;
    }
    if (!anyError){
        cout<<"Program executed successfully"<<endl;
        for (string line : outputLines) outfile<<line<<endl;
    }
    outfile.close(); //closing the file
    return 0;
}
