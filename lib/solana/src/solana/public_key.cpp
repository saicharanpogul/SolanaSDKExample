#include <string>
#include <optional>
#include "solana/public_key.h"
#include "solana/base58.h"
#include "public_key.h"

PublicKey::PublicKey()
{
  std::fill(key, key + PUBLIC_KEY_LEN, 0);
}

PublicKey::PublicKey(const unsigned char value[PUBLIC_KEY_LEN])
{
  std::copy(value, value + PUBLIC_KEY_LEN, this->key);
}

std::string PublicKey::toBase58()
{
  return base58Encode(this->key, PUBLIC_KEY_LEN);
}

void PublicKey::sanitize() {}

std::optional<PublicKey> PublicKey::fromString(const std::string &s)
{
  if (s.length() > PUBLIC_KEY_LEN)
  {
    throw ParsePubkeyError("WrongSize");
  }
  std::vector<unsigned char> publicKeyVec;
  try
  {
    std::vector<int> intVec = base58Decode(reinterpret_cast<const unsigned char *>(s.c_str()), PUBLIC_KEY_LEN);
    std::vector<unsigned char> publicKeyVec(intVec.begin(), intVec.end());
  }
  catch (...)
  {
    throw ParsePubkeyError("Invalid");
  }
  if (publicKeyVec.size() != PUBLIC_KEY_LEN)
  {
    throw ParsePubkeyError("WrongSize");
  }
  return PublicKey(publicKeyVec.data());
}

// Serialize method
std::vector<uint8_t> PublicKey::serialize()
{
  std::string str = this->toBase58();
  std::vector<uint8_t> vec(str.begin(), str.end());
  return vec;
}

// Deserialize method
PublicKey PublicKey::deserialize(const std::vector<uint8_t> &data)
{
  std::string str(data.begin(), data.end());
  auto publicKeyOpt = PublicKey::fromString(str);
  if (publicKeyOpt.has_value())
  {
    return publicKeyOpt.value();
  }
  else
  {
    throw ParsePubkeyError("Invalid");
  }
}
