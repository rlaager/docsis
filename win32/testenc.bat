@echo off
for %%f in (..\examples\*.cfg) do docsis_d -e %%f ..\examples\keyfile %%f.bin
