// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_KEY_H__
#define __ELASTOS_SDK_KEY_H__

#include <boost/shared_ptr.hpp>

#include "openssl/obj_mac.h"

#include "BRKey.h"
#include "BRAddress.h"
#include "Wrapper.h"
#include "CMemBlock.h"

namespace Elastos {
	namespace ElaWallet {

		class Key :
				public Wrapper<BRKey> {
		public:
			Key();

			Key(BRKey *brkey);

			Key(const BRKey &brkey);

			Key(const std::string &privKey);

			Key(const CMBlock &privKey);

			Key(const UInt256 &secret, bool compressed);

			Key(const CMBlock &seed, uint32_t chain, uint32_t index);

			virtual std::string toString() const;

			virtual BRKey *getRaw() const;

			UInt256 getSecret() const;

			CMBlock getPubkey() const;

			bool setPubKey(const CMBlock pubKey);

			bool getCompressed() const;

			std::string getPrivKey() const;

			bool setSecret(const UInt256 &data, bool compressed);

			bool setPrivKey(const std::string &privKey);

			CMBlock compactSign(const CMBlock &data) const;

			std::string compactSign(const std::string &message) const;

			CMBlock encryptNative(const CMBlock &data, const CMBlock &nonce) const;

			CMBlock decryptNative(const CMBlock &data, const CMBlock &nonce) const;

			std::string address() const;

			bool verify(const UInt256 &messageDigest, const CMBlock &signature) const;

			std::string keyToAddress(const int signType) const;

			UInt168 keyToUInt168BySignType(const int signType);

			std::string keyToRedeemScript(int signType) const;

			void deriveKeyAndChain(UInt256 &chainCode, const void *seed, size_t seedLen, int depth, ...);

			const UInt160 hashTo160();

			const UInt168 hashTo168();

		public:
			static CMBlock getSeedFromPhrase(const CMBlock &phrase, const std::string &phrasePass = "");

			static CMBlock getAuthPrivKeyForAPI(const CMBlock &seed);

			static void getAuthPrivKeyAndChainCode(const CMBlock &seed, CMBlock &key, UInt256 &chainCode);

			static std::string getAuthPublicKeyForAPI(const CMBlock &privKey);

			static CMBlock getPublicKeyByKey(const Key &key);

			static std::string decryptBip38Key(const std::string &privKey, const std::string &pass);

			static bool isValidBitcoinPrivateKey(const std::string &key);

			static bool isValidBitcoinBIP38Key(const std::string &key);

			static UInt256 encodeSHA256(const std::string &message);

			static void
			calculatePrivateKeyList(BRKey keys[], size_t keysCount, UInt256 *secret, UInt256 *chainCode,
			                        uint32_t chain, const uint32_t indexes[]);

			static bool verifyByPublicKey(const std::string &publicKey, const std::string &message,
										  const std::string &signature);

			static bool verifyByPublicKey(const std::string &publicKey, const UInt256 &messageDigest,
			                              const CMBlock &signature);

			static const UInt256 getSystemAssetId();

			static CMBlock getPubKeyFromPrivKey(const CMBlock &privKey,
			                                               int nid = NID_X9_62_prime256v1);

		private:
			void setPublicKey();

		private:
			boost::shared_ptr<BRKey> _key;
		};

		typedef boost::shared_ptr<Key> KeyPtr;

	}
}

#endif //__ELASTOS_SDK_KEY_H__
