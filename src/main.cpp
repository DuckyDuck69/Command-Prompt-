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
  std::stringstream ss(str);
  std::string token;

  while(std::getline(ss, token, delimiter)){
    result.push_back(token);
  }
  return result;
} 

std::string returnPath(std::string s, std::string path){
  std::string result;
  std::vector<std::string> pathFind = splitString(path, ':');
    for(int i =0; i < pathFind.size(); i++){
      std::string fullPath = pathFind[i] + "/" + s;
      if(std::filesystem::exists(fullPath)){
        result = s + " is " + fullPath;
        return result;
      }
    }
    result = s + ": not found";
    return result;
}

std::string typeCheck(std::string s, std::string path){
  std::string result;
  if(s == "echo" || s == "exit" || s == "type"){
    result = s + " is a shell builtin";
  }
  else{
    result = returnPath(s, path);
  }
    return result;
}

void returnStatemnt(std::string input, std::string path){
  std::string result = "";
  std::string pathRoute = returnPath(input, path);
  if(access(pathRoute.c_str(), X_OK) == 0){
    system(pathRoute.c_str());
  }
  else{
    std::cout<<"Program either not found or not executable"<<std::endl;
  }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage
  const char* path = std::getenv("PATH");

  bool loop = true;
  while(loop == true){
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);


    std::string echoCommand = "echo"; 
    std::string exitCommand = "exit 0";
    std::string typeCommand = "type";
    size_t findEcho = input.find(echoCommand);   //return a number, which is the index of the first character it found
    size_t findType = input.find(typeCommand);

    int inputLength = splitString(input, ' ').size();
    const char* path = std::getenv("PATH");

    if(input == exitCommand){ //exit if the user type "exit 0"
      loop = false;
    }
    else if( findEcho == 0){   //declare echo command, which is to print out a string
      input.erase(0, echoCommand.length() + 1);
      std::cout<<input<<std::endl;
    }
    else if(findType == 0){
      input.erase(0, typeCommand.length() + 1);
      std::cout<<typeCheck(input, path)<<std::endl;
    }
    else if(inputLength >= 2){
      // std::cout<<"Program was passed "<<inputLength<<" args (including program name)."
      // std::cout<<"Arg"
      returnStatemnt(input, path);
    }
    else{
      std::cout<<input<<": command not found"<<std::endl;
    }
  }
  return 0;
}
