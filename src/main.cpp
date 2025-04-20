#include <iostream>
#include <stdio.h>
#include <cstdlib> 
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <filesystem>

std::vector<std::string> splitString(const std::string& str, char delimiter){
  /*
    This funtion creat a stringstream and then loop through the input string until it hits
    the delimiter. Then, it stop and push to the result vector.

    return: a vector of words
  */
  std::vector<std::string> result;
  std::stringstream ss(str);  //create a stringstrem 
  std::string token;

  while(std::getline(ss, token, delimiter)){  //read through the stringstream and stop if meet the delimitere
    if(!token.empty()){
      result.push_back(token);    //add the chunk of characters into the result vector
    }
  }
  return result;
} 

std::vector<std::string> parseInput(const std::string& input){
  /*
    This function take the input and parse them into proper tokens, handling single quotes, double quotes, etc.input
    
    return: a vector of valid tokens
  */
  std::vector<std::string> token;
  std::string current;
  bool hitSingleQuote = false;
  bool hitDoubleQuote = false;
  bool hitBackSplash = false;

  for(size_t i = 0; i < input.length(); i++){
    //order: 
    //inside single quote: double quote and backslash have no special meaning
    //inside double quote: single quote has no special meaning, backslash escape important character only

    char c = input[i];
    if(hitBackSplash){
      if(c == '\\' || c == '$' || c == '\"' || c == '\'' || std::isspace(c) || c == 'n' || c == 't'){
        current += c;
      }
      else {
        //if a character after \ is not special, add the backslash and the char because me 
        //missed the backslash earlier
        current += '\\';
        current += c;
      }
      hitBackSplash = false;
    }
    else if(c == '\\' && !hitSingleQuote){ //backslash only lose functionality inside single quote
      hitBackSplash = !hitBackSplash;
    }
    else if(c == '\'' && !hitDoubleQuote){  
      hitSingleQuote = !hitSingleQuote;
    }
    else if(c == '\"' && !hitSingleQuote){
      hitDoubleQuote = !hitDoubleQuote;
    }
    else if(std::isspace(c) && !hitSingleQuote && !hitDoubleQuote){
      if(!current.empty()){
        token.push_back(current);
        current.erase();  //erase current, prepare for the next token
      }
    }
    else{
      current += c;  //add the character into the string
    }
  }
  //there might the words left after the iteration, push them into the vector
  if(!current.empty()){
    token.push_back(current);
  }
  return token;
}

std::string trimString(const std::string& str, char trim){
/*
    This function trim the first and the last element wanted from a string

    return: trimmed string
  */

   //find the first and last index that is not the character we want to trim
   //e.g  'hello', findFirst return iterator (like pointer) point to h and findLast returns
   //iterator points to o

  //Use lambda function because find_if_not() requires true/false
  //findFirst iterator works forward, so no need to base() it
  auto findFirst = std::find_if_not(str.begin(), str.end(), [trim](char c){
    return c == trim;
  }); 
  //findLast iterator works backward, so we need to base() it to converts the reverse iterator 
  //into a normal (forward) iterator, pointing just after the character wanted.
  auto findLast = std::find_if_not(str.rbegin(), str.rend(), [trim](char c){
    return c == trim;
  }).base();
  if(findFirst >= findLast){
    return "";
  }else{
    return std::string(findFirst, findLast);  //return a new copy of the string using the two iterator  
  }
}  

std::string returnPath(std::string s, std::string path, bool extractPath = false){
  /*
   This fucntion split the path by the ":" to get specific directories
   then it loop through each directory to find the execution path of an input

   return value: valid path 
  */
  std::string result;
  std::vector<std::string> pathFind = splitString(path, ':');
    for(size_t i =0; i < pathFind.size(); i++){
      std::string fullPath = pathFind[i] + "/" + s;
      if(std::filesystem::exists(fullPath)){
        if(extractPath == true){
          return fullPath;
      }
        result = s + " is " + fullPath;
        return result;
      }
    }
    if(extractPath == true){
      return "";
    }
    result = s + ": not found";
    return result;
}

std::string typeCheck(std::string s, std::string path){
  /*
    This function check if a command is a builtin command or not, if not then return the path to the comamnd
    if it is executable

    return: a string/path
  */
  std::string result;
  if(s == "echo" || s == "exit" || s == "type" || s == "pwd"){
    result = s + " is a shell builtin";
  }
  else{
    result = returnPath(s, path);
  }
    return result;
}

void executeCommand(std::vector<std::string> inputVect, std::string input, std::string path){
  /*
    This function take the first input in a sentence as the command e.g. "echo" for "echo Hello World".
    Then it return the system executable path of the command and then execute the command.

    return: None, just executing the command
  */
 if(inputVect.empty()) return;

  std::string program = inputVect[0];   //choose only the first input to execute 

  std::string commandLine;

  //if the executable name (program) contains at least one of the following: space,
  //single quote, doule quote, backslash, then it need to be quoted
  bool needToBeQuoted = (program.find(' ') != std::string::npos || 
                        program.find('\'') != std::string::npos || 
                        program.find('\"') != std::string::npos ||
                        program.find('\\') != std::string::npos);
        
  if(needToBeQuoted){
    //escape any double quote in the program 
    std::string escapeArg = "";
    for(char c: program){
      if (c == '\"') escapeArg += "\\\"";
      else escapeArg += c;
    }
    commandLine += "\"" + escapeArg + "\"";
  }
  else{
    commandLine += program;
  }
  
  //once we have the executable command, add the arguments to run
  for(size_t i =1; i<inputVect.size(); i++){
    commandLine += " ";
    commandLine += "\"" + inputVect[i] + "\"";   //safely quote arguments even if they include quotes
  }
  //convert to c_str( because system only deals with c string 
  system(commandLine.c_str());   //run the full command with all the arguments
}

void changeDir(std::string path){
  /*
    This function change the current directory to the designated one
    
    return: none, only change dir
  */
  if(path == "~"){
    std::string home = getenv("HOME");  //get home directory
    std::filesystem::current_path(home);   //change to home
  }
  else if(std::filesystem::exists(path)){   //filesystem also handle relative paths eg ./local/bin => user/local/bin
    std::filesystem::current_path(path);  //change to that directory 
  }
  else{
    std::cout<<"cd: "<<path<<": No such file or directory"<<std::endl;
  }
}


int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  bool loop = true;
  while(loop == true){
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);

    //declare different type of bultin
    std::string exitCommand = "exit 0";    

    //this block below is for executing files condition
    std::vector<std::string> inputVect = parseInput(input);
    int inputLength = inputVect.size();


    //get the input path
    const char* pathFind = std::getenv("PATH");   //get the path from the OS
    std::string path = pathFind;

    std::string command = inputVect[0]; //get the first string as a command
    
    if(input == exitCommand){ //exit if the user type "exit 0" 
      loop = false;
    }
    else if(command == "cd"){
      changeDir(inputVect[1]);
    }
    else if(command == "pwd"){   //print current working directory
      std::cout<<std::filesystem::current_path().string()<<std::endl;   //convert to a string to avoid formatting output
    }
    else if(command == "echo"){   //declare echo command, which is to print out a string
      std::string printOut = "";
      for(size_t i = 1; i < inputVect.size(); i++){
        printOut += inputVect[i];
        if(i < inputVect.size() - 1){
          printOut += " ";
        }
      }
      std::cout<<printOut<<std::endl;
    }
    else if(command == "type" && inputLength > 1 ){  //search if the command is executable 
      std::cout<<typeCheck(inputVect[1], path)<<std::endl;
    }
    else{
      executeCommand(inputVect, input, path);
    }
  }
  return 0;
}
