@echo off
call config.cmd

premake gmake
if errorlevel 0 (
    make target=%TARGET% config=%CONFIG% -j
)