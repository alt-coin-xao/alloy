# Alloy

Copyright (C) 2021-Now, alloy.hopto Developers.   
Copyright (c) 2017-2018, The Alloy Developers.   
Portions Copyright (c) 2012-2017, The CryptoNote Developers, The Bytecoin Developers.

# Download Releases
http://www.alloy.hopto.org

# Source Code
https://github.com/alt-coin-xao

# GUI Wallet
Located Here: http://www.alloy.hopto.org

# Discord
https://discord.gg/gmvM5KEHFz

# How to Compile on Linux (It will download what it needs).

## Release 

```git clone ```

```cd alloy```

```mkdir build && cd build```

```cmake ..```

```make  # (or more generic all platforms cmake --build . --config Release )```

## Debug

```Debug is a little bit more work at the moment for cmake, some cmake versions have a bug or known feature.```
```It still works but need to use ccmake to configure the build and forec generation of Makefile```

```ccmake -DCMAKE_BUILD_TYPE=Debug ..  (from reading this depends on version of cmake)```

```make  # (or more generic all platforms cmake --build . --config Debug )```

## Compiler Info

Unlike the previous devs, I'll be taking the path of least resistance.

Windows visual studio Community edition of Windows seems fine, and is easier to support.
Linux gnu cpp 10.3.1 works fine, 11.3.0 needs more work finding the compiler settings for this code base.
Mac is till in the pipline, but it will be the commercial compiler (maybe open source).


## Introduction

Alloy is a private, secure, untraceable, decentralised digital currency. You are your bank, you control your funds, and nobody can trace your transfers unless you allow them to do so.

**Privacy:** Alloy uses a cryptographically sound system to allow you to send and receive funds without your transactions being easily revealed on the blockchain (the ledger of transactions that everyone has). This ensures that your purchases, receipts, and all transfers remain absolutely private by default.

**Security:** Using the power of a distributed peer-to-peer consensus network, every transaction on the network is cryptographically secured. Individual wallets have a 25 word mnemonic seed that is only displayed once, and can be written down to backup the wallet. Wallet files are encrypted with a passphrase to ensure they are useless if stolen.

**Untraceability:** By taking advantage of ring signatures, a special property of a certain type of cryptography, Alloy is able to ensure that transactions are not only untraceable, but have an optional measure of ambiguity that ensures that transactions cannot easily be tied back to an individual user or computer.

## About this Project

This is the core implementation of Alloy. It is open source and completely free to use without restrictions, except for those specified in the license agreement below. There are no restrictions on anyone creating an alternative implementation of Alloy that uses the protocol and network in a compatible manner.

As with many development projects, the repository on Github is considered to be the "staging" area for the latest changes. Before changes are merged into that branch on the main repository, they are tested by individual developers in their own branches, submitted as a pull request, and then subsequently tested by contributors who focus on testing and code reviews. That having been said, the repository should be carefully considered before using it in a production environment, unless there is a patch in the repository for a particular show-stopping issue you are experiencing. It is generally a better idea to use a tagged release for stability.

**Anyone is welcome to contribute to Alloy's codebase!** If you have a fix or code change, feel free to submit it as a pull request directly to the "master" branch. In cases where the change is relatively small or does not affect other parts of the codebase it may be merged in immediately by any one of the collaborators. On the other hand, if the change is particularly large or complex, it is expected that it will be discussed at length either well in advance of the pull request being submitted, or even directly on the pull request.

# License

Alloy is licensed under the "MIT License", see [LICENSE](LICENSE) for more info.
