FROM ubuntu:24.04

# Toolchain + cmake/ninja
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential git curl python3-venv ca-certificates \
    cmake ninja-build pkg-config \
 && rm -rf /var/lib/apt/lists/*

# Bazelisk (Bazel 8.3 via .bazelversion)
RUN curl -L https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-linux-amd64 -o /usr/local/bin/bazel \
 && chmod +x /usr/local/bin/bazel

# Conan in venv (PEP 668-safe)
RUN python3 -m venv /opt/py \
 && /opt/py/bin/pip install --upgrade pip \
 && /opt/py/bin/pip install conan

# ⬅️ make sure PATH is set *before* COPY+RUN
ENV PATH="/opt/py/bin:${PATH}"

WORKDIR /work
COPY . /work

# Normalize endings, ensure exec perms, then run with bash
RUN set -eux; \
    chmod +x /work/tools/conan/install.sh /work/tools/conan/gen_build_wrapper.py || true; \
    sed -i 's/\r$//' /work/tools/conan/install.sh /work/tools/conan/gen_build_wrapper.py; \
    bash /work/tools/conan/install.sh

# Sanity: the Bazel package must now exist
RUN test -f /work/third_party/conan/BUILD.bazel

# Entrypoint (self-heal if needed)
COPY docker/entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]




