#ifndef PROJECT_LIBRARY_H
#define PROJECT_LIBRARY_H

#include <gmp.h>
#include <string>
#include <gmpxx.h>
#include <unistd.h>
#include <time.h>
#include <bitset>
#include <sstream>
#include <iostream>
#include <fstream>


class RSA{

    mpz_class p;
    mpz_class q;
    mpz_class n;
    mpz_class e;
    mpz_class d;

    unsigned int key_size;

public:

    static const std::string key_file_name;

    RSA(unsigned long int p_, unsigned long int q_, unsigned int key_size_=2048);
    RSA(unsigned int key_size_=2048);
    RSA(mpz_class e_, mpz_class n_, unsigned int key_size_=2048): n(n_), e(e_), key_size(key_size_){sleep(1); srand(time(NULL));};

    std::string encrypt(std::string message) const;
    std::string decrypt(std::string message) const;
    void share_public_key() const;
    static std::string encrypt_pub(std::string message);

private:

    void generate_keys(unsigned long int e_=65537);
    mpz_class get_totient() const{ return (p-1_mpz)*(q-1_mpz);}
    mpz_class encrypt_(mpz_class message) const;
    mpz_class decrypt_ (mpz_class message) const;
    mpz_class generate_prime(int prime_size) const;
    std::string add_padding(std::string message) const;
    std::string remove_padding(std::string message) const;
    unsigned int ciphertex_size() const;

};

#endif