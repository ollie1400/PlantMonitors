set PROJECT_DIR=%CD%
if "%IDF_PATH%" == "" (
    @echo Please set the IDF_PATH environment variable to point to the ESP-IDF installation
    exit 1
)

:: IDF doesn't work well with pyenv so add specific python version to path
set PPPATH=%userprofile%\.pyenv\pyenv-win\versions\3.8.0-amd64
set PATH=%PPPATH%;%PATH%

cd %IDF_PATH%
call export.bat
call cd %PROJECT_DIR%

call idf.py build