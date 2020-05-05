//
// Created by werka on 4/29/20.
//
#include <iostream>
#include <fstream>
#include <csignal>
#include <cstdio>

#include "library.h"

std::string file_name;

void handle_sigint(int sig){
    system("killall -SIGINT ./build/sender");
    std::remove(RSA::key_file_name.c_str());
    std::remove(file_name.c_str());
    printf("%s", RSA::key_file_name.c_str());
    exit(0);
}

int main(int argc, char *argv[]){

    if(argc <=2 ){
        std::cout<<"Podaj długość klucza i nazwę pliku wyjściowego\n";
        sleep(4);
        system("killall -SIGKILL ./build/sender");
        return 1;
    }
    std::signal(SIGINT, handle_sigint);
    std::cout<<"-----Server is running-----\nWaiting for message...\n";
    unsigned int key_size = std::stoi(argv[1]);
    file_name = argv[2];

    RSA r(key_size);
    r.share_public_key();

    std::string message, ciphertext;
    while(true){

        std::fstream file(file_name);
        if(getline (file, ciphertext)){
            std::cout<<"Server received ciphertex: "<<ciphertext<<"\nDecrypted message: "<<r.decrypt(ciphertext)<<"\n\n";
            file.close();
            file.open(file_name, std::ofstream::out | std::ofstream::trunc);
        }
        file.close();
    }
}
