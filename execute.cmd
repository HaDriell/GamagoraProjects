@echo off
call config.cmd

if "%1" == "" (
    start "%TARGET%" /D %TARGET% ".\bin\%CONFIG%\%TARGET%.exe"
    exit
) else (
    start "%1" /D %1 ".\bin\%CONFIG%\%1.exe"
    exit
)

:help
    echo execute.cmd <project_name>
    exit