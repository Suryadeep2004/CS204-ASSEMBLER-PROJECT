using namespace std;

//Global variables
vector<string> textLines; //array to contain all the text lines of assembly code
vector<string> dataLines; //array to contain all the data lines of assembly code
vector<string> outLines; //array to contain all the output lines of machine code
unordered_map<string, int> labels; //hash map to store labels and their program counter
bool isText=true; //variable to check which segment of the code does current line belongs to (initialised to text segment)
//sets to check which format of instruction does current instruction belong to in O(1)
unordered_set<string> r = {"add", "and", "or", "sll", "slt", "sra", "srl", "sub", "xor", "mul", "div", "rem"};
unordered_set<string> i = {"addi", "andi", "ori", "lb", "ld", "lh", "lw", "jalr"};
unordered_set<string> s = {"sb", "sw", "sd", "sh"};
unordered_set<string> sb = {"beq", "bne", "bge", "blt"};
unordered_set<string> u = {"auipc", "lui"};
unordered_set<string> uj = {"jal"};

//Function to remove unnecessary space and commas 
string removeCommasAndSpaces(string& str) {
    string result;
    bool lastWasSpaceOrComma = false;
    for (char c : str){
        if (c == ' ' || c == ','){
            if (!lastWasSpaceOrComma){
                result += ' ';
                lastWasSpaceOrComma = true;
            }
        } 
        else{
            result += c;
            lastWasSpaceOrComma = false;
        }
    }
    while (result.back()==' ') result.pop_back();
    return result[0]==' '?result.substr(1):result;
}

//Function to split the line obtained into tokens
vector<string> splitString(string& str) {
    vector<string> words;
    istringstream iss(str);
    string word;
    while (iss >> word) words.push_back(word);
    return words;
}

//Function to convert a string in lowercase format
string toLowercase(const std::string& str) {
    string result = str;
    for (char& c : result) {
        if (isalpha(c)) {
            c = tolower(c);
        }
    }
    return result;
}