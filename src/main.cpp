#include <iostream>
#include <stdio.h>

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
    size_t findEcho = input.find(echoCommand);

    if(input == "exit 0"){
      loop = false;
    }
    else if( findEcho != std::string::npos){
      input.erase(0, echoCommand.length());
      std::cout<<input<<std::endl;
    }
    else{
      std::cout<<input<<": command not found"<<std::endl;
    }
  }
  return 0;
}
