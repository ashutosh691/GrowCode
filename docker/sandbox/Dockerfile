FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    g++ \
    openjdk-21-jdk-headless \
    python3 \
    coreutils \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN useradd --create-home --shell /usr/sbin/nologin runner

WORKDIR /workspace

USER runner

CMD ["sleep", "infinity"]