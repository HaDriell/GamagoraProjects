@echo off
call config.cmd
make %TARGET% config=%CONFIG% clean
