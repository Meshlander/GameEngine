@echo off

mkdir ..\build
pushd ..\build
cl  D:\gameengine\code\win32_server.cpp /link -opt:ref user32.lib gdi32.lib winmm.lib Ws2_32.lib wsock32.lib
popd