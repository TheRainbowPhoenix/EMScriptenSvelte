#!/bin/sh

echo ". /usr/local/emsdk/emsdk_env.sh" >> /home/codespace/.bashrc

cd /workspace/cp_sdk
sudo chmod 777 .
mkdir build
mkdir vscode

pnpm i
pnpm run build
