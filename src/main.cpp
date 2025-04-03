#include <iostream>
#include <stdio.h>
#include <cstdlib> 
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <unistd.h>

std::vector<std::string> splitString(const std::string& str, char delimiter){

  std::vector<std::string> result;
  std::stringstream ss(str);  //create a stringstrem 
  std::string token;

  while(std::getline(ss, token, delimiter)){  //read through the stringstream and stop if meet the delimitere
    result.push_back(token);    //add the chunk of characters into the result vector
  }
  return result;
} 

std::string returnPath(std::string s, std::string path, bool extractPath = false){
  std::string result;
  std::vector<std::string> pathFind = splitString(path, ':');
    for(int i =0; i < pathFind.size(); i++){
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
  std::string program = inputVect[0];   //choose only the first input to execute 
  std::string pathRoute = returnPath(program, path, true);
  if(pathRoute.empty()){
    std::cout<<program<<": command not found"<< std::endl;
  }else{
    system(input.c_str());   //run the full command with all the arguments
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
    std::string echoCommand = "echo"; 
    std::string exitCommand = "exit 0";
    size_t findEcho = input.find(echoCommand);   //return a number, which is the index of the first character it found

    //this block below is for executing files condition
    std::vector<std::string> inputVect = splitString(input, ' ');
    int inputLength = inputVect.size();


    //get the input path
    const char* pathFind = std::getenv("PATH");   //the os ma
    std::string path = pathFind;

    std::string command = inputVect[0]; //get the first string as a command
    
    if(command == exitCommand){ //exit if the user type "exit 0"
      loop = false;
    }
    else if(command == "pwd"){   //print current working directory
      std::cout<<std::filesystem::current_path().string()<<std::endl;
    }
    else if( findEcho == 0){   //declare echo command, which is to print out a string
      input.erase(0, echoCommand.length() + 1);
      std::cout<<input<<std::endl;
    }
    else if(command == "type" && inputLength > 1 ){
      std::cout<<typeCheck(inputVect[1], path)<<std::endl;
    }
    else{
      executeCommand(inputVect, input, path);
    }
  }
  return 0;
}
