// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_SPVCLIENT_WALLET_H__
#define __ELASTOS_SDK_SPVCLIENT_WALLET_H__

#include <map>
#include <string>
#include <BRWallet.h>
#include <boost/weak_ptr.hpp>
#include <boost/function.hpp>
#include <SDK/ELACoreExt/ELATransaction.h>

#include "BRInt.h"

#include "Wrapper.h"
#include "SDK/Transaction/Transaction.h"
#include "Address.h"
#include "SharedWrapperList.h"
#include "MasterPubKey.h"
#include "SDK/Transaction/TransactionOutput.h"
#include "WrapperList.h"
#include "MasterPrivKey.h"
#include "AddressCache.h"

namespace Elastos {
	namespace ElaWallet {

		struct ELAWallet {
			BRWallet Raw;
			typedef std::map<std::string, std::string> TransactionRemarkMap;
			TransactionRemarkMap TxRemarkMap;

#ifdef TEMPORARY_HD_STRATEGY
			MasterPrivKey PrivKeyRoot;
			std::string TemporaryPassword;
			AddressCache *Cache;
#endif
		};

#ifdef TEMPORARY_HD_STRATEGY
		ELAWallet *ELAWalletNew(BRTransaction *transactions[], size_t txCount, const MasterPrivKey &masterPrivKey,
								const std::string &password, DatabaseManager *databaseManager,
								size_t (*WalletUnusedAddrs)(BRWallet *wallet, BRAddress addrs[], uint32_t gapLimit,
															int internal),
								size_t (*WalletAllAddrs)(BRWallet *wallet, BRAddress addrs[], size_t addrsCount),
								void (*setApplyFreeTx)(void *info, void *tx),
								void (*WalletUpdateBalance)(BRWallet *wallet),
								int (*WalletContainsTx)(BRWallet *wallet, const BRTransaction *tx),
								void (*WalletAddUsedAddrs)(BRWallet *wallet, const BRTransaction *tx),
								BRTransaction *(*WalletCreateTxForOutputs)(BRWallet *wallet,
																		   const BRTxOutput outputs[],
																		   size_t outCount),
								uint64_t (*WalletMaxOutputAmount)(BRWallet *wallet),
								uint64_t (*WalletFeeForTx)(BRWallet *wallet, const BRTransaction *tx),
								int (*TransactionIsSigned)(const BRTransaction *tx),
								size_t (*KeyToAddress)(const BRKey *key, char *addr, size_t addrLen));
#else

		ELAWallet *ELAWalletNew(BRTransaction *transactions[], size_t txCount, BRMasterPubKey mpk,
								size_t (*WalletUnusedAddrs)(BRWallet *wallet, BRAddress addrs[], uint32_t gapLimit,
															int internal),
								size_t (*WalletAllAddrs)(BRWallet *wallet, BRAddress addrs[], size_t addrsCount),
								void (*setApplyFreeTx)(void *info, void *tx),
								void (*WalletUpdateBalance)(BRWallet *wallet),
								int (*WalletContainsTx)(BRWallet *wallet, const BRTransaction *tx),
								void (*WalletAddUsedAddrs)(BRWallet *wallet, const BRTransaction *tx),
								BRTransaction *(*WalletCreateTxForOutputs)(BRWallet *wallet,
																		   const BRTxOutput outputs[],
																		   size_t outCount),
								uint64_t (*WalletMaxOutputAmount)(BRWallet *wallet),
								uint64_t (*WalletFeeForTx)(BRWallet *wallet, const BRTransaction *tx),
								int (*TransactionIsSigned)(const BRTransaction *tx),
								size_t (*KeyToAddress)(const BRKey *key, char *addr, size_t addrLen));

#endif

		void ELAWalletFree(ELAWallet *wallet, bool freeInternal = true);

		std::string ELAWalletGetRemark(ELAWallet *wallet, const std::string &txHash);

		void ELAWalletRegisterRemark(ELAWallet *wallet, const std::string &txHash,
												   const std::string &remark);

		void ELAWalletLoadRemarks(ELAWallet *wallet,
												const SharedWrapperList<Transaction, BRTransaction *> &transaction);

		class Wallet :
				public Wrapper<BRWallet> {

		public:
			class Listener {
			public:
				// func balanceChanged(_ balance: UInt64)
				virtual void balanceChanged(uint64_t balance) = 0;

				// func txAdded(_ tx: BRTxRef)
				virtual void onTxAdded(const TransactionPtr &transaction) = 0;

				// func txUpdated(_ txHashes: [UInt256], blockHeight: UInt32, timestamp: UInt32)
				virtual void onTxUpdated(const std::string &hash, uint32_t blockHeight, uint32_t timeStamp) = 0;

				// func txDeleted(_ txHash: UInt256, notifyUser: Bool, recommendRescan: Bool)
				virtual void onTxDeleted(const std::string &hash, bool notifyUser, bool recommendRescan) = 0;
			};

		public:

#ifdef TEMPORARY_HD_STRATEGY
			Wallet(const SharedWrapperList<Transaction, BRTransaction *> &transactions,
				   const MasterPrivKey &masterPrivKey,
				   const std::string &payPassword,
				   DatabaseManager *databaseManager,
				   const boost::shared_ptr<Listener> &listener);
#else

			Wallet(const SharedWrapperList<Transaction, BRTransaction *> &transactions,
				   const MasterPubKeyPtr &masterPubKey,
				   const boost::shared_ptr<Listener> &listener);

#endif

			virtual ~Wallet();

			virtual std::string toString() const;

			virtual BRWallet *getRaw() const;

			uint32_t getBlockHeight() const;

			void resetAddressCache(const std::string &payPassword);

			nlohmann::json GetBalanceInfo();

			void RegisterRemark(const TransactionPtr &transaction);

			std::string GetRemark(const std::string &txHash);

			uint64_t GetBalanceWithAddress(const std::string &address);

			// returns the first unused external address
			std::string getReceiveAddress() const;

			// writes all addresses previously generated with BRWalletUnusedAddrs() to addrs
			// returns the number addresses written, or total number available if addrs is NULL
			std::vector<std::string> getAllAddresses();

			// true if the address was previously generated by BRWalletUnusedAddrs() (even if it's now used)
			// int BRWalletContainsAddress(BRWallet *wallet, const char *addr);
			bool containsAddress(const std::string &address);

			// true if the address was previously used as an input or output in any wallet transaction
			// int BRWalletAddressIsUsed(BRWallet *wallet, const char *addr);
			bool addressIsUsed(const std::string &address);

			SharedWrapperList<Transaction, BRTransaction *> getTransactions() const;

			SharedWrapperList<Transaction, BRTransaction *> getTransactionsConfirmedBefore(uint32_t blockHeight) const;

			uint64_t getBalance() const;

			uint64_t getTotalSent();

			uint64_t getTotalReceived();

			uint64_t getFeePerKb();

			void setFeePerKb(uint64_t feePerKb);

			uint64_t getMaxFeePerKb();

			uint64_t getDefaultFeePerKb();

			TransactionPtr
			createTransaction(const std::string &fromAddress, uint64_t fee, uint64_t amount,
							  const std::string &toAddress, const std::string &remark, bool isShuffle = true);

			bool containsTransaction(const TransactionPtr &transaction);

			bool inputFromWallet(const BRTxInput *in);

			bool registerTransaction(const TransactionPtr &transaction);

			void removeTransaction(const UInt256 &transactionHash);

			void updateTransactions(const std::vector<UInt256> &transactionsHashes, uint32_t blockHeight,
									uint32_t timestamp);

			/**
			 * Returns the BRCoreTransaction with the provided `transactionHash` if it exists; otherwise
			 * NULL is returned.
			 *
			 * If BRCoreTransaction.JNI_COPIES_TRANSACTIONS is true, then the returned transaction is
			 * a copy of the Core BRTransaction - TBD caution when passed back into Core
			 *
			 * @param transactionHash
			 * @return
			 */
			TransactionPtr transactionForHash(const UInt256 &transactionHash);

			/**
			 * Check if a transaction is valid - THIS METHOD WILL FATAL if the transaction is not signed.
			 * You must call transaction.isSigned to avoid the FATAL.
		 	 *
			 * @param transaction
			 * @return
			 */
			bool transactionIsValid(const TransactionPtr &transaction);

			bool transactionIsPending(const TransactionPtr &transaction);

			bool transactionIsVerified(const TransactionPtr &transaction);

			/**
			 * Return the net amount received by this wallet.  If the amount is positive, then the balance
			 * of this wallet increased; if the amount is negative, then the balance decreased.
			 *
			 * @param tx
			 * @return
			 */
			uint64_t getTransactionAmount(const TransactionPtr &tx);

			uint64_t getTransactionFee(const TransactionPtr &tx);

			uint64_t getTransactionAmountSent(const TransactionPtr &tx);

			uint64_t getTransactionAmountReceived(const TransactionPtr &tx);

			uint64_t getBalanceAfterTransaction(const TransactionPtr &transaction);

			/**
			 * Return a BRCoreAddress for a) the receiver (if we sent an amount) or b) the sender (if
			 * we received an amount).  The returned address will be the first address that is not in
			 * this wallet from the outputs or the inputs, respectively.
			 *
			 * @param transaction
			 * @return
			 */
			std::string getTransactionAddress(const TransactionPtr &transaction);

			/**
			 * Return the first BRCoreAddress from the `transaction` inputs that is not an address
			 * in this wallet.
			 *
			 * @param transaction
			 * @return The/A BRCoreAddress that received an amount from us (that we sent to)
			 */
			std::string getTransactionAddressInputs(const TransactionPtr &transaction);

			/**
			 * Return the first BRCoreAddress from the `transaction` outputs this is not an address
			 * in this wallet.
			 *
			 * @param transaction
			 * @return The/A BRCoreAddress that sent to us.
			 */
			std::string getTransactionAddressOutputs(const TransactionPtr &transaction);

			uint64_t getFeeForTransactionSize(size_t size);

			uint64_t getMinOutputAmount();

			uint64_t getMaxOutputAmount();

		protected:
			Wallet();

			static bool AddressFilter(const std::string &fromAddress, const std::string &filterAddress);

			static BRTransaction *CreateTxForOutputs(BRWallet *wallet, const BRTxOutput outputs[], size_t outCount,
													 uint64_t fee, const std::string &fromAddress,
													 bool(*filter)(const std::string &fromAddress,
																   const std::string &addr));

			static BRTransaction *
			WalletCreateTxForOutputs(BRWallet *wallet, const BRTxOutput outputs[], size_t outCount);

			bool WalletSignTransaction(const TransactionPtr &transaction, int forkId, const void *seed, size_t seedLen);

			static uint64_t WalletMaxOutputAmount(BRWallet *wallet);

			static uint64_t WalletFeeForTx(BRWallet *wallet, const BRTransaction *tx);

			static void WalletUpdateBalance(BRWallet *wallet);

			static int WalletContainsTx(BRWallet *wallet, const BRTransaction *tx);

			static void WalletAddUsedAddrs(BRWallet *wallet, const BRTransaction *tx);

			static int TransactionIsSigned(const BRTransaction *tx);

			static void setApplyFreeTx(void *info, void *tx);

			static void balanceChanged(void *info, uint64_t balance);

			static void txAdded(void *info, BRTransaction *tx);

			static void txUpdated(void *info, const UInt256 txHashes[], size_t count, uint32_t blockHeight,
								  uint32_t timestamp);

			static void txDeleted(void *info, UInt256 txHash, int notifyUser, int recommendRescan);

			static size_t KeyToAddress(const BRKey *key, char *addr, size_t addrLen);

			static size_t WalletUnusedAddrs(BRWallet *wallet, BRAddress addrs[], uint32_t gapLimit, int internal);

		protected:
			ELAWallet *_wallet;

			boost::weak_ptr<Listener> _listener;
		};

		typedef boost::shared_ptr<Wallet> WalletPtr;

	}
}

#endif //__ELASTOS_SDK_SPVCLIENT_WALLET_H__
