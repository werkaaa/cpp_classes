//
// Created by werka on 4/29/20.
//
#include <iostream>
#include <csignal>

#include "library.h"

void handle_sigint(int sig){
    system("killall -SIGINT ./build/server");
    exit(0);
}

int main(int argc, char *argv[]){

    if(argc <=1 ){
        std::cout<<"Podaj nazwę pliku wyjściowego\n";
        sleep(4);
        system("killall -SIGKILL ./build/server");
        return 1;
    }
    std::signal(SIGINT, handle_sigint);
    std::string file_name = argv[1];
    std::string message, ciphertext;
    std::cout<<"-----Sender is running-----\nEnter message to encrypt: ";
    while(true){

        if(std::getline(std::cin, message)) {
            ciphertext = RSA::encrypt_pub(message);
            if(ciphertext=="Key is not specified\n"){
                std::cout<<ciphertext<<"Enter message to encrypt: ";
                continue;
            }

            std::ofstream file;
            file.open(file_name);
            file << ciphertext << '\n';
            file.close();
            std::cout<<"\nEnter message to encrypt: ";
        }
    }
}
