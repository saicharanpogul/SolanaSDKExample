#include <sodium/crypto_sign_ed25519.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include "solana/keypair.h"
#include "solana/base58.h"
#include "solana/public_key.h"
#include "esp_random.h"

// Default constructor for zero-initialized keys
Keypair::Keypair()
{
    std::fill(secretKey, secretKey + SECRET_KEY_LEN, 0);
}

// Generate keypair from a secure seed
Keypair::Keypair(const std::vector<unsigned char> &seed)
{
    assert(seed.size() == SECRET_KEY_LEN); // Enforce valid seed size
    unsigned char publicKey[PUBLIC_KEY_LEN];
    crypto_sign_ed25519_seed_keypair(publicKey, this->secretKey, seed.data());
    this->publicKey = PublicKey(publicKey);
}

// Generate keypair from a secure seed (convenience for C arrays)
Keypair::Keypair(const unsigned char seed[SECRET_KEY_LEN]) : Keypair(std::vector<unsigned char>(seed, seed + SECRET_KEY_LEN)) {}

// Access secret key with authorization
const unsigned char *Keypair::getSecretKey()
{
    return this->secretKey;
}

// Destructor to securely clear secret key
Keypair::~Keypair()
{
    std::fill(secretKey, secretKey + SECRET_KEY_LEN, 0);
}

// Generate a new Keypair with a random seed
Keypair Keypair::generate()
{
    std::vector<unsigned char> seed(SECRET_KEY_LEN);
    esp_fill_random(&seed, SECRET_KEY_LEN);
    return Keypair(seed);
}
