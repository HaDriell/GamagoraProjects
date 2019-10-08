@echo off
call config.cmd

premake gmake
if errorlevel 0 (
    make %TARGET% config=%CONFIG% -j
)