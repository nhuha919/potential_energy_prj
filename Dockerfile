# Dockerfile
FROM arm64v8/ros:humble

# Setup environment
ENV DEBIAN_FRONTEND=noninteractive

# Install developer tools
RUN apt update && apt install -y \
    build-essential \
    python3-colcon-common-extensions \
    python3-pip \
    git \
    curl \
    vim \
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*

# Set default shell to bash
SHELL ["/bin/bash", "-c"]

# Source ROS 2 automatically
RUN echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
