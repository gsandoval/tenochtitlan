@echo off

msbuild tenochtitlan.sln /t:tenochtitlan /p:Configuration=Debug /p:Platform="Win32" /clp:NoSummary;NoItemAndPropertyList;Verbosity=minimal /nologo
