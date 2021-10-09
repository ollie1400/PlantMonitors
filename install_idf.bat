set PROJECT_DIR=%CD%
set IDF_PATH=C:\Users\Ollie\Documents\Programs\esp-idf-v4.3.1

:: IDF doesn't work well with pyenv so add specific python version to path
set PPPATH=C:\Users\Ollie\.pyenv\pyenv-win\versions\3.8.1
set PATH=%PPPATH%;%PATH%
echo %PATH%
cd %IDF_PATH%
call install.bat

call cd %PROJECT_DIR%