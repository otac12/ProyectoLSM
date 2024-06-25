setlocal enabledelayedexpansion

for /d %%d in (*) do (
    set contador=1
    for %%f in ("%%d\*.jpg") do (
        ren "%%f" "!contador!.jpg"
        set /a contador+=1
    )
)