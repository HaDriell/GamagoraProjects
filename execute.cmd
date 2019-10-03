@echo off
call config.cmd

if "%1" == "" (
    goto help
) else (
    start "%1" /D %1 ".\bin\%CONFIG%\%1.exe"
    exit
)

:help
    echo execute.cmd <project_name>
    exit