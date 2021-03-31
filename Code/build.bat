@echo off

mkdir ..\build
pushd ..\build
cl -MT -GR- -EHa- -Oi -W4 -FC -Z7 -O2 D:\gameengine\code\win32_game.cpp /link -opt:ref user32.lib gdi32.lib winmm.lib Ws2_32.lib wsock32.lib
popd