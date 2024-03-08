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

    //dummy output to check if everything is fine until now
    // for (string str : dataLines) cout<<str<<endl;
    // cout<<endl;
    // for (string str : textLines) cout<<str<<endl;
    // cout<<endl;
    // for (auto x : labels) cout<<x.first<<" "<<x.second<<endl;

    //traversing through the text segment of the code to produce it's corresponding machine code
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
        }
        else if (s.find(words[0])!=s.end()){ //checking if instruction is S type
        }
        else if (sb.find(words[0])!=sb.end()){ //checking if instruction is SB type
        }
        else if (u.find(words[0])!=u.end()){ //checking if instruction is U type
        }
        else if (uj.find(words[0])!=uj.end()){ //checking if instruction is UJ type
        }
        else{ //if none then obviously an error
            cout<<"Error present at text line "<<k<<endl;
            return 0;
        }
        programCounter+=4;
    }
    return 0;
}