@echo off

for /r %%F in (*.ipynb) do (
    echo Clean output of file %%F
    nbstripout %%F
    echo Output of file %%F is cleaned
)