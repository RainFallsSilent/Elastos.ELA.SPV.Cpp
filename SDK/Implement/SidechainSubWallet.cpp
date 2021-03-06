// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <vector>
#include <map>
#include <boost/scoped_ptr.hpp>

#include "SidechainSubWallet.h"
#include "ELACoreExt/ELATxOutput.h"
#include "ELACoreExt/Payload/PayloadTransferCrossChainAsset.h"
#include "ParamChecker.h"
#include "Transaction/SidechainTransactionChecker.h"
#include "Transaction/SidechainTransactionCompleter.h"

namespace Elastos {
	namespace ElaWallet {

		SidechainSubWallet::SidechainSubWallet(const CoinInfo &info, const ChainParams &chainParams,
											   const std::string &payPassword, const PluginTypes &pluginTypes,
											   MasterWallet *parent) :
				SubWallet(info, chainParams, payPassword, pluginTypes, parent) {

		}

		SidechainSubWallet::~SidechainSubWallet() {

		}

		nlohmann::json SidechainSubWallet::CreateWithdrawTransaction(const std::string &fromAddress,
																	 const std::string &toAddress,
																	 const uint64_t amount,
																	 const nlohmann::json &mainchainAccounts,
																	 const nlohmann::json &mainchainAmounts,
																	 const nlohmann::json &mainchainIndexs,
																	 uint64_t fee,
																	 const std::string &memo,
																	 const std::string &remark) {
			boost::scoped_ptr<TxParam> txParam(
					TxParamFactory::createTxParam(Sidechain, fromAddress, toAddress, amount, fee, memo, remark));

			ParamChecker::checkJsonArrayNotEmpty(mainchainAccounts);
			ParamChecker::checkJsonArrayNotEmpty(mainchainAmounts);
			ParamChecker::checkJsonArrayNotEmpty(mainchainIndexs);

			std::vector<std::string> accounts = mainchainAccounts.get<std::vector<std::string>>();
			std::vector<uint64_t> amounts = mainchainAmounts.get<std::vector<uint64_t>>();
			std::vector<uint64_t> indexs = mainchainIndexs.get<std::vector<uint64_t>>();
			assert(accounts.size() == amounts.size());
			assert(accounts.size() == indexs.size());

			WithdrawTxParam *withdrawTxParam = dynamic_cast<WithdrawTxParam *>(txParam.get());
			assert(withdrawTxParam != nullptr);
			withdrawTxParam->setMainchainDatas(accounts, indexs, amounts);

			//todo read main chain address from config
			std::string mainchainAddress;
			withdrawTxParam->setMainchainAddress(mainchainAddress);

			TransactionPtr transaction = createTransaction(txParam.get());
			if (transaction == nullptr) {
				throw std::logic_error("Create transaction error.");
			}
			return transaction->toJson();
		}

		std::string SidechainSubWallet::GetGenesisAddress() const {
			return _info.getGenesisAddress();
		}

		boost::shared_ptr<Transaction>
		SidechainSubWallet::createTransaction(TxParam *param) const {
			WithdrawTxParam *withdrawTxParam = dynamic_cast<WithdrawTxParam *>(param);
			if (withdrawTxParam != nullptr) {
				TransactionPtr ptr = _walletManager->getWallet()->
						createTransaction(param->getFromAddress(), param->getFee(), param->getAmount(),
										  param->getToAddress(), param->getRemark());
				if (!ptr) return nullptr;

				ptr->setTransactionType(ELATransaction::TransferCrossChainAsset);
				const std::vector<TransactionOutput *> &outList = ptr->getOutputs();

				std::for_each(outList.begin(), outList.end(),
							  [&param](TransactionOutput *output) {
								  ((ELATxOutput *) output->getRaw())->assetId = param->getAssetId();
							  });

				PayloadTransferCrossChainAsset *payloadTransferCrossChainAsset =
						static_cast<PayloadTransferCrossChainAsset *>(ptr->getPayload());
				payloadTransferCrossChainAsset->setCrossChainData(withdrawTxParam->getCrossChainAddress(),
																  withdrawTxParam->getCrossChainOutputIndexs(),
																  withdrawTxParam->getCrosschainAmouts());
				return ptr;
			} else
				return SubWallet::createTransaction(param);
		}

		void SidechainSubWallet::verifyRawTransaction(const TransactionPtr &transaction) {
			if (transaction->getTransactionType() == ELATransaction::TransferCrossChainAsset) {
				SidechainTransactionChecker checker(transaction, _walletManager->getWallet());
				checker.Check();
			} else
				SubWallet::verifyRawTransaction(transaction);
		}

		TransactionPtr SidechainSubWallet::completeTransaction(const TransactionPtr &transaction, uint64_t actualFee) {
			if (transaction->getTransactionType() == ELATransaction::TransferCrossChainAsset) {
				SidechainTransactionCompleter completer(transaction, _walletManager->getWallet());
				return completer.Complete(actualFee);
			} else
				return SubWallet::completeTransaction(transaction, actualFee);
		}
	}
}