ARG UBUNTU_CODENAME="noble"
ARG LLVM_VERSION="20"

FROM ubuntu:${UBUNTU_CODENAME} AS base

ARG UBUNTU_CODENAME
ARG LLVM_VERSION

# Required for adding new package repositories
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    ca-certificates \
    gpg \
    software-properties-common \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Add repository for latest LLVM toolchain
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
    add-apt-repository "deb http://apt.llvm.org/${UBUNTU_CODENAME}/ llvm-toolchain-${UBUNTU_CODENAME}-${LLVM_VERSION} main"

# Add repository for latest CMake
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add - && \
    add-apt-repository "deb https://apt.kitware.com/ubuntu/ ${UBUNTU_CODENAME} main"

# Install build dependencies
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    # LLVM toolchain
    clang-${LLVM_VERSION} \
    libc++-${LLVM_VERSION}-dev \
    libc++abi-${LLVM_VERSION}-dev \
    libc++abi1-${LLVM_VERSION} \
    lld-${LLVM_VERSION} \
    # Build system
    cmake \
    git \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-${LLVM_VERSION} 100 && \
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-${LLVM_VERSION} 100

ENV CC="clang"
ENV CXX="clang++"

FROM base AS release

WORKDIR /workspace

COPY CMakeLists.txt CMakePresets.json ./
COPY src ./src
COPY syntax_samples ./syntax_samples

RUN cmake -S . -G Ninja --preset default && \
    cmake --build --preset default --target install

ENTRYPOINT [ "./out/default/bin/main" ]
CMD [ "./syntax_samples/sample2.lox" ]

FROM base AS devel
ARG LLVM_VERSION

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    clang-${LLVM_VERSION}-doc \
    clang-tools-${LLVM_VERSION} \
    clang-format-${LLVM_VERSION} \
    clangd-${LLVM_VERSION} \
    lldb-${LLVM_VERSION}

RUN update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-${LLVM_VERSION} 100
RUN update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-${LLVM_VERSION} 10
RUN update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-${LLVM_VERSION} 100

WORKDIR /workspace
