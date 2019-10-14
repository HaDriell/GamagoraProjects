rem This file is used to configure all the underlying build commands
@echo off
rem Target is the make target, in case you want some specific stuff
set TARGET=UnitTest

rem Configuration (either Debug or Release)
rem set CONFIG=debug
set CONFIG=release

echo ****Configuration Start
echo     Target: %TARGET%
echo     Config: %CONFIG%
echo ****Configuration End
