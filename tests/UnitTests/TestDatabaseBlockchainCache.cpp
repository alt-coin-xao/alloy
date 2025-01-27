/*
 * Copyright (c) 2017-2018, The Alloy Developers.
 * Portions Copyright (c) 2012-2017, The CryptoNote Developers, The Bytecoin Developers.
 *
 * This file is part of Alloy.
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 */

#include "gtest/gtest.h"

#include "Crypto/crypto.h"

#include "CryptoNoteCore/BlockchainCache.h"
#include <CryptoNoteCore/DatabaseBlockchainCache.h>
#include "CryptoNoteCore/CryptoNoteTools.h"
#include "CryptoNoteCore/TransactionValidatiorState.h"
#include "DataBaseMock.h"
#include <CryptoNoteCore/DBUtils.h>
#include "CryptoNoteCore/MemoryBlockchainCacheFactory.h"
#include "Logging/FileLogger.h"
#include "TestBlockchainGenerator.h"

using namespace CryptoNote;
using namespace Crypto;

namespace {

Hash randomBlockHash() {
  Hash hash;
  for (auto& b : hash.data) {
    b = rand();
  }
  return hash;
}

class DatabaseBlockchainCacheTests : public ::testing::Test {
public:
  DatabaseBlockchainCacheTests()
      : currency(CurrencyBuilder(logger).currency()), blockchainCacheFactory("", logger), blockchain(currency, database, blockchainCacheFactory, logger), generator(currency) {
  }

   void SetUp() override {
    generator.generateEmptyBlocks(currency.minedMoneyUnlockWindow() + 1);
    for (auto& block : generator.getBlockchain()) {
      TransactionValidatorState state;
      auto cached = CachedBlock{block};
      generatedBlockHashes.push_back(cached.getBlockHash());
      blockchain.pushBlock(cached, {}, state, 0, 0, 0, { toBinaryArray(block), {} }); // TODO: add coins, block sizes, etc
    }
    count = generatedBlockHashes.size();
   }

   void TearDown() override {
   }

   std::unordered_map<BlockchainCache::Amount, uint32_t> countOutputsForAmount() {
    std::unordered_map<BlockchainCache::Amount, uint32_t> cnt;
    auto& bc = generator.getBlockchain();
    for (auto& b : bc) {
      auto& outs = b.baseTransaction.outputs;
      for (auto& out : outs) {
        cnt[out.amount] += 1;
      }
    }
    return cnt;
   }

   Currency currency;
   DataBaseMock database;
   Logging::FileLogger logger;
   MemoryBlockchainCacheFactory blockchainCacheFactory;
   DatabaseBlockchainCache blockchain;
   TestBlockchainGenerator generator;
   std::vector<Hash> generatedBlockHashes;
   size_t count = 0;
 };
}

TEST_F(DatabaseBlockchainCacheTests, DefaultParentIsNullptr) {
  ASSERT_EQ(nullptr, blockchain.getParent());
}

TEST_F(DatabaseBlockchainCacheTests, CheckParentAfterUpdate) {
  DatabaseBlockchainCache local(currency, database, blockchainCacheFactory, logger);
#ifndef NDEBUG
  ASSERT_DEATH(blockchain.setParent(&local), "");
#endif
}

TEST_F(DatabaseBlockchainCacheTests, DeleteEmptyChild) {
  DatabaseBlockchainCache local(currency, database, blockchainCacheFactory, logger);
  ASSERT_FALSE(blockchain.deleteChild(&local));
}

TEST_F(DatabaseBlockchainCacheTests, DeleteChild) {
  DatabaseBlockchainCache local(currency, database, blockchainCacheFactory, logger);
  blockchain.addChild(&local);
  ASSERT_TRUE(blockchain.deleteChild(&local));
}

//TEST_F(DatabaseBlockchainCacheTests, DISABLE_StorageAlwaysThrows) {
//  ASSERT_DEATH(blockchain.getStorage(), "");
//}

TEST_F(DatabaseBlockchainCacheTests, GetTopBlockIndex) {
  ASSERT_EQ(generator.getBlockchain().size(), blockchain.getTopBlockIndex());
}

TEST_F(DatabaseBlockchainCacheTests, GetStartBlockIndexFromContructor) {
  ASSERT_EQ(0, DatabaseBlockchainCache(currency, database, blockchainCacheFactory, logger).getStartBlockIndex());
}

TEST_F(DatabaseBlockchainCacheTests, GetTopBlockIndexForEmptyCache) {
  ASSERT_EQ(generator.getBlockchain().size(), DatabaseBlockchainCache(currency, database, blockchainCacheFactory, logger).getTopBlockIndex());
}

TEST_F(DatabaseBlockchainCacheTests, GetStartBlockIndex) {
  ASSERT_EQ(0, blockchain.getStartBlockIndex());
}

TEST_F(DatabaseBlockchainCacheTests, GetTopBlockHash) {
  ASSERT_EQ(generatedBlockHashes.back(), blockchain.getTopBlockHash());
}

TEST_F(DatabaseBlockchainCacheTests, BlockCount) {
  ASSERT_EQ(generator.getBlockchain().size() + 1, blockchain.getBlockCount());
}

TEST_F(DatabaseBlockchainCacheTests, HasBlockFromBlockchain) {
   ASSERT_TRUE(blockchain.hasBlock(generatedBlockHashes[0]));
   ASSERT_TRUE(blockchain.hasBlock(generatedBlockHashes[generatedBlockHashes.size() / 2]));
   ASSERT_TRUE(blockchain.hasBlock(generatedBlockHashes.back()));
   ASSERT_FALSE(blockchain.hasBlock(randomBlockHash()));
}

TEST_F(DatabaseBlockchainCacheTests, RawBlocksWereInserted) {
   ASSERT_EQ(generatedBlockHashes.size() + 1, database.blocks().size());
   uint32_t i = static_cast<uint32_t>(count);
   auto b = database.blocks();
   while (i--) {
     BlockTemplate blockTemplate;
     bool result = fromBinaryArray(blockTemplate, database.blocks().at(i + 1).block);
     ASSERT_TRUE(result);

     CachedBlock cachedBlock(blockTemplate);
     auto p = cachedBlock.getBlockIndex();
     ASSERT_EQ(cachedBlock.getBlockHash(), generatedBlockHashes.at(i));
  }
}

TEST_F(DatabaseBlockchainCacheTests, RawBlocksWithTxsSerialization) {
  const std::string RANDOM_ADDRESS = "2634US2FAz86jZT73YmM8u5GPCknT2Wxj8bUCKivYKpThFhF2xsjygMGxbxZzM42zXhKUhym6Yy6qHHgkuWtruqiGkDpX6m";
  const std::string SERIALIZATION_NAME = "name";
  const size_t TXS_COUNT = 10;

  CryptoNote::AccountPublicAddress pubAddr;
  ASSERT_TRUE(currency.parseAccountAddressString(RANDOM_ADDRESS, pubAddr));
  generator.generateTransactionsInOneBlock(pubAddr, TXS_COUNT);
  std::vector<CryptoNote::BlockTemplate> blocks = generator.getBlockchainCopy();
  ASSERT_LT(0, blocks.size());

  BlockTemplate lastBlock = blocks.back();
  ASSERT_EQ(TXS_COUNT, lastBlock.transactionHashes.size());

  CryptoNote::RawBlock rawBlock{ toBinaryArray(lastBlock), {} };
  ASSERT_NO_THROW(std::transform(std::begin(lastBlock.transactionHashes), std::end(lastBlock.transactionHashes), std::back_inserter(rawBlock.transactions),
  [&](const Crypto::Hash& txHash) {
    return toBinaryArray(generator.getTransactionByHash(txHash));
  }));

  ASSERT_EQ(TXS_COUNT, rawBlock.transactions.size());

  std::string serializedRawBlock = CryptoNote::DB::serialize(rawBlock, SERIALIZATION_NAME);
  CryptoNote::RawBlock deserializedRawBlock;
  CryptoNote::DB::deserialize(serializedRawBlock, deserializedRawBlock, SERIALIZATION_NAME);

  ASSERT_EQ(deserializedRawBlock.block, rawBlock.block);
  ASSERT_EQ(deserializedRawBlock.transactions, rawBlock.transactions);
}
