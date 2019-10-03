@echo off
call config.cmd
make target=%TARGET% config=%CONFIG% clean
