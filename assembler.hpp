using namespace std;

//Global variables
vector<string> textLines; //array to contain all the text lines of assembly code
vector<string> dataLines; //array to contain all the data lines of assembly code
vector<string> outputLines; //array to contain all the output lines of machine code
unordered_map<string, int> labels; //hash map to store labels and their program counter
bool isText=true, anyError=false; //variable to check which segment of the code does current line belongs to (initialised to text segment)
//sets to check which format of instruction does current instruction belong to in O(1)
unordered_set<string> r = {"add", "and", "or", "sll", "slt", "sra", "srl", "sub", "xor", "mul", "div", "rem"};
unordered_set<string> i = {"addi", "andi", "ori", "lb", "ld", "lh", "lw", "jalr"};
unordered_set<string> s = {"sb", "sw", "sd", "sh"};
unordered_set<string> sb = {"beq", "bne", "bge", "blt"};
unordered_set<string> u = {"auipc", "lui"};
unordered_set<string> uj = {"jal"};
unordered_map<string, string> opcodeValue={ //hash map of instructions and their corresponding opcodes
    {"add",  "0110011"},
    {"and",  "0110011"},
    {"or",   "0110011"},
    {"sll",  "0110011"},
    {"slt",  "0110011"},
    {"sra",  "0110011"},
    {"srl",  "0110011"},
    {"sub",  "0110011"},
    {"xor",  "0110011"},
    {"mul",  "0110011"},
    {"div",  "0110011"},
    {"rem",  "0110011"},
    {"addi", "0010011"},
    {"andi", "0010011"},
    {"ori",  "0010011"},
    {"lb",   "0000011"},
    {"ld",   "0000011"},
    {"lh",   "0000011"},
    {"lw",   "0000011"},
    {"jalr", "1100111"},
    {"sb",   "0100011"},
    {"sw",   "0100011"},
    {"sd",   "0100011"},
    {"sh",   "0100011"},
    {"beq",  "1100011"},
    {"bne",  "1100011"},
    {"bge",  "1100011"},
    {"blt",  "1100011"},
    {"auipc","0010111"},
    {"lui",  "0110111"},
    {"jal",  "1101111"}
};
unordered_map<string, string> funct3Value={ //hash map of instructions and their corresponding funct3
    {"add",  "000"},
    {"and",  "111"},
    {"or",   "110"},
    {"sll",  "001"},
    {"slt",  "010"},
    {"sra",  "101"},
    {"srl",  "101"},
    {"sub",  "000"},
    {"xor",  "100"},
    {"mul",  "000"},
    {"div",  "100"},
    {"rem",  "110"},
    {"addi", "000"},
    {"andi", "111"},
    {"ori",  "110"},
    {"lb",   "000"},
    {"ld",   "011"},
    {"lh",   "001"},
    {"lw",   "010"},
    {"jalr", "000"},
    {"sb",   "000"},
    {"sw",   "010"},
    {"sd",   "011"},
    {"sh",   "001"},
    {"beq",  "000"}, 
    {"bne",  "001"},
    {"bge",  "101"},
    {"blt",  "100"}
};
unordered_map<string, string> funct7Value={ //hash map of instructions and their corresponding funct7
    {"add",  "0000000"},
    {"and",  "0000000"},
    {"or",   "0000000"},
    {"sll",  "0000000"},
    {"slt",  "0000000"},
    {"sra",  "0100000"},
    {"srl",  "0000000"},
    {"sub",  "0100000"},
    {"xor",  "0000000"},
    {"mul",  "0000001"},
    {"div",  "0000001"},
    {"rem",  "0000001"}
};
unordered_map<string, string> registerValue={ //hash map of registers and their corresponding codes
    {"x0",  "00000"},
    {"x1",  "00001"},
    {"x2",  "00010"},
    {"x3",  "00011"},
    {"x4",  "00100"},
    {"x5",  "00101"},
    {"x6",  "00110"},
    {"x7",  "00111"},
    {"x8",  "01000"},
    {"x9",  "01001"},
    {"x10", "01010"},
    {"x11", "01011"},
    {"x12", "01100"},
    {"x13", "01101"},
    {"x14", "01110"},
    {"x15", "01111"},
    {"x16", "10000"},
    {"x17", "10001"},
    {"x18", "10010"},
    {"x19", "10011"},
    {"x20", "10100"},
    {"x21", "10101"},
    {"x22", "10110"},
    {"x23", "10111"},
    {"x24", "11000"},
    {"x25", "11001"},
    {"x26", "11010"},
    {"x27", "11011"},
    {"x28", "11100"},
    {"x29", "11101"},
    {"x30", "11110"},
    {"x31", "11111"},
    {"zero","00000"},
    {"ra",  "00001"},
    {"sp",  "00010"},
    {"gp",  "00011"},
    {"tp",  "00100"},
    {"t0",  "00101"},
    {"t1",  "00110"},
    {"t2",  "00111"},
    {"s0",  "01000"},
    {"s1",  "01001"},
    {"a0",  "01010"},
    {"a1",  "01011"},
    {"a2",  "01100"},
    {"a3",  "01101"},
    {"a4",  "01110"},
    {"a5",  "01111"},
    {"a6",  "10000"},
    {"a7",  "10001"},
    {"s2",  "10010"},
    {"s3",  "10011"},
    {"s4",  "10100"},
    {"s5",  "10101"},
    {"s6",  "10110"},
    {"s7",  "10111"},
    {"s8",  "11000"},
    {"s9",  "11001"},
    {"s10", "11010"},
    {"s11", "11011"},
    {"t3",  "11100"},
    {"t4",  "11101"},
    {"t5",  "11110"},
    {"t6",  "11111"}
};

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
vector<string> splitString(const string& str) {
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

//Function to convert a binary string to hexadecimal string
string binaryToHex(string binary) {
    bitset<64> bits(binary);
    unsigned long long num = bits.to_ullong();
    stringstream ss;
    ss << hex << num;
    string hexStr = ss.str();
    while (hexStr.size()<8) hexStr="0"+hexStr;
    hexStr="0x"+hexStr;
    return hexStr;
}

//Function to convert decimal integer to hexadecimal string
string decimalToHex(int decimal) {
    stringstream ss;
    ss << "0x" << hex << decimal;
    return ss.str();
}

//Function to convert decimal integer to binary string
string decimalToBinary(int decimal) {
    if (decimal == 0) return "0";
    string binary = "";
    while (decimal > 0) {
        binary = (decimal % 2 == 0 ? "0" : "1") + binary;
        decimal /= 2;
    }
    return binary;
}

//Function to extract immediate value from S type instructions
void extractImmediate(string& input, string& X, string& Y) {
    size_t leftParenthesisPos = input.find_first_of('(');
    size_t rightParenthesisPos = input.find_last_of(')');

    if (leftParenthesisPos!=string::npos && rightParenthesisPos!=string::npos && leftParenthesisPos < rightParenthesisPos) {
        X = input.substr(0, leftParenthesisPos);
        Y = input.substr(leftParenthesisPos + 1, rightParenthesisPos - leftParenthesisPos - 1);
    } 
    else{
        X = "";
        Y = "";
    }
}

//Function to check whether immediate value is integer or a label
bool isInteger(string str) {
    try{
        stoi(str);
        return true;
    } 
    catch (const invalid_argument& e){
        return false;
    } 
    catch (const out_of_range& e){
        return false;
    }
}

//Function to check whether immediate value is hexadecimal or decimal
bool isHexadecimal(string str) {
    if (str.length() < 3 || str.substr(0, 2) != "0x") return false;
    for (size_t i = 2; i < str.length(); ++i) {
        if (!isxdigit(str[i]))
            return false;
    }
    return true;
}

//Function to convert hexadecimal string to binary string
string hexToBinary(string hexString, size_t n) {
    unsigned long long intVal = stoull(hexString, nullptr, 16);
    bitset<64> bits(intVal); 
    return bits.to_string().substr(64 - n);
}