setlocal enabledelayedexpansion
set i=1

for /r %%d in (*) do (
    if exist "%%d" (
        for %%f in ("%%d\*.jpg") do (
            ren "%%f" "!i!.jpg"
            set /a i+=1
        )
    )
)