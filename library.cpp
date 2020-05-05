#include "library.h"

#include <iostream>
#include <algorithm>
#include <cmath>

const std::string RSA::key_file_name = "key_file.txt";

mpz_class RSA::generate_prime(int prime_size) const {
    std::string prime_ = "1";
    for(int i = 1; i<prime_size; i++){
        prime_ += char(int(2.0*rand()/(RAND_MAX+1.0)) + 48);
    }
    mpz_class prime;
    prime.set_str(prime_, 2);
    mpz_nextprime(prime.get_mpz_t(),prime.get_mpz_t());

    return prime;
}

mpz_class RSA::decrypt_(mpz_class message) const {
    mpz_class decrypted;
    mpz_powm(decrypted.get_mpz_t(), message.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
    return decrypted;
}

mpz_class RSA::encrypt_(mpz_class message) const {
    mpz_class encrypted;
    mpz_powm(encrypted.get_mpz_t(), message.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
    return encrypted;
}

void RSA::generate_keys(unsigned long e_) {
    e = mpz_class(e_);
    n = p*q;
    mpz_class t = get_totient();
    mpz_invert(d.get_mpz_t(), e.get_mpz_t(), t.get_mpz_t());
}

RSA::RSA(unsigned long int p_, unsigned long int q_, unsigned int key_size_): key_size(key_size_) {
    p = mpz_class(p_);
    q = mpz_class(q_);
    sleep(1);
    srand(time(NULL));
    generate_keys();
}

RSA::RSA(unsigned int key_size_): key_size(key_size_) {
    sleep(1);
    srand(time(NULL));
    p = generate_prime(key_size/2);
    q = generate_prime(key_size/2);
    while(p==q) q = generate_prime(1024);
    generate_keys();
}

std::string RSA::encrypt(std::string message) const{

    // convert message to binary string
    std::string binary_string;
    for (char& _char : message) {
        binary_string +=std::bitset<8>((unsigned long long int)_char).to_string();
    }

    mpz_class message_;
    std::string binary_string_padded, output, buf;
    unsigned int i = 0;
    while(i<binary_string.size()){
        binary_string_padded = add_padding(binary_string.substr(i, std::min(key_size-88, (unsigned int)(binary_string.size()-i))));

        i += (key_size-88);

        message_.set_str(binary_string_padded, 2);
        binary_string_padded = encrypt_(message_).get_str();

        size_t buf_size = binary_string_padded.size()%ciphertex_size() ? ciphertex_size() - binary_string_padded.size()%ciphertex_size() : 0;
        buf = "";
        for(size_t j = 0; j<buf_size; j++) buf += "0";
        buf += binary_string_padded;
        binary_string_padded = buf;

        output += binary_string_padded;
    }

    return output;
}

std::string RSA::decrypt(std::string message) const{

    mpz_class message_;
    message_.set_str(message, 10);

    std::string output, message_part, decrypted;

    unsigned int i = 0;
    while(i<message.size()) {
        message_part = message.substr(i, std::min(ciphertex_size(), (unsigned int)(message.size()-i)));
        message_.set_str(message_part, 10);
        mpz_class decrypted_ = decrypt_(message_);
        decrypted = remove_padding("00000000000000" + decrypted_.get_str(2));

        std::stringstream stream(decrypted);
        while (stream.good()) {
            std::bitset<8> bits;
            stream >> bits;
            char c = char(bits.to_ulong());
            output += c;
        }
        output.pop_back();
        i += ciphertex_size();
    }
    return output;
}

void RSA::share_public_key() const{
    std::ofstream key_file;
    key_file.open (key_file_name);
    key_file << key_size << '\n';
    key_file << n.get_str(10) << '\n';
    key_file << e.get_str(10) << '\n';
    key_file.close();
}

std::string RSA::encrypt_pub(std::string message){
    std::string n_;
    std::string e_;
    std::string key_size_;
    std::ifstream key_file(key_file_name);
    if(!getline(key_file, key_size_)) return "Key is not specified\n";
    getline (key_file, n_);
    getline (key_file, e_);
    key_file.close();

    mpz_class n, e;
    n.set_str(n_, 10);
    e.set_str(e_, 10);
    RSA r(e, n, (unsigned int)(std::stoi(key_size_)));
    return r.encrypt(message);
}

std::string RSA::add_padding(std::string message) const{
    unsigned long padding_len = (key_size - message.size())/8;
    std::string padding = "0000000000000010";
    char next_letter;
    for(unsigned long i = 0; i<padding_len-3; i++){
        next_letter = char((rand()%255)+1);
        padding += std::bitset<8>((unsigned long long int)next_letter).to_string();
    }
    padding += "00000000";
    return padding + message;
}

std::string RSA::remove_padding(std::string message) const{

    std::stringstream stream(message);
    int i = 0;
    int zero_cnt = 0;
    while (stream.good() and zero_cnt<2) {
        std::bitset<8> bits;
        stream >> bits;
        if(bits.to_ulong()==0) zero_cnt++;
        i+=8;
    }

    return message.substr(i, message.size()-i);
}

unsigned int RSA::ciphertex_size() const{
    return (unsigned int)(std::ceil(key_size*std::log10(2)));
}