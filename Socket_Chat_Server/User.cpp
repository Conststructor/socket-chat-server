#include "User.h"




//void HashPassword(const std::string& password, std::string& hashedPassword) {
//    unsigned char hash[SHA256_DIGEST_LENGTH];
//    SHA256_CTX sha256;
//    SHA256_Init(&sha256);
//    SHA256_Update(&sha256, password.c_str(), password.length());
//    SHA256_Final(hash, &sha256);
//
//    std::stringstream ss;
//    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
//        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
//    }
//
//    hashedPassword = ss.str();
//}