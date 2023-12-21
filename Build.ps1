function Set-BinaryObjects {
    param (
        [Parameter(Mandatory = $true, Position=0)]
        $lib,
        [Parameter(Mandatory = $false)]
        $includes
    )
    
    $c_file = "$lib.c"
    $o_file = "bin\$lib.o"
    $a_file = "bin\lib$lib.a"
    $so_file = "bin\lib$lib.so"

    gcc -o $o_file -c $c_file
    ar -rcs $a_file $o_file
    gcc -fPIC -o $o_file -c $c_file

    if ($includes)
    {
        gcc -shared -o $so_file $includes $o_file
    }
    else
    {
        gcc -shared -o $so_file $o_file
    }
}

Set-Location src\helpers

if (!(Test-Path -Path 'bin')) {
    mkdir('bin')
}

Set-BinaryObjects -lib console 
Set-BinaryObjects -lib strhelpers

Set-Location ..\lib

if (!(Test-Path -Path 'bin')) {
    mkdir('bin')
}

Set-BinaryObjects -lib developers -includes ..\helpers\bin\console.o, ..\helpers\bin\strhelpers.o
Set-BinaryObjects -lib games -includes ..\helpers\bin\console.o, ..\helpers\bin\strhelpers.o, bin\developers.o

Set-Location ..

gcc -o ..\main main.c lib\bin\games.o lib\bin\developers.o helpers\bin\console.o helpers\bin\strhelpers.o

Set-Location ..