#include <iostream>
#include <stdio.h>

std::string typeCheck(std::string s){
  std::string result;
  if(s == "echo" || s == "exit" || s == "type"){
    result = s + " is a shell builtin";
  }
  else{
    result = s + ": not found";
  }
  return result;
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage
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

    if(input == exitCommand){ //exit if the user type "exit 0"
      loop = false;
    }
    else if( findEcho == 0){   //declare echo command, which is to print out a string
      input.erase(0, echoCommand.length() + 1);
      std::cout<<input<<std::endl;
    }
    else if(findType == 0){
      input.erase(0, typeCommand.length() + 1);
      std::cout<<typeCheck(input)<<std::endl;
    }
    else{
      std::cout<<input<<": command not found"<<std::endl;
    }
  }
  return 0;
}
