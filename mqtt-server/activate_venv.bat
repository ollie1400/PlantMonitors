:start
IF EXIST "venv" (
call venv\Scripts\activate
) ELSE (
call python -m venv venv
GOTO start
)

call pip install -r requirements.txt