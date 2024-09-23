param (
    [int]$test = 0
)

$exitCode = 0

If (-Not (Test-Path "Release\project.exe")) {
    Write-Host "Failed: " -NoNewline -ForegroundColor Red
    Write-Host "Release\project.exe was not found."
    $exitCode = 1
}
Else {
    Copy-Item src\project2_scenarios.dll -Destination .\Release -Force
    Copy-Item src\project2_scenarios.lib -Destination .\Release -Force
    
    Get-ChildItem -Include *.dmp, *.mdmp, *.dump, *.mdump -Recurse | ForEach-Object { Remove-Item -Path $_.FullName -Force }
    

    $location = Get-Location
    $outputFile = "$location\output-$test.txt"

    Set-Location Release

    Write-Host "Testing Scenario $test : " -ForegroundColor Cyan
    
    & .\project.exe $test | Tee-Object -FilePath $outputFile -Append
    
    Set-Location ..

    $feedback = ""
    
    $dumpWritten = $false
    Get-ChildItem -Include *.dmp, *.mdmp, *.dump, *.mdump -Recurse | ForEach-Object {
        $dumpWritten = $true
        
        $size = (Get-Item $_.FullName).length

        If ($size -gt 0kb) {
            $feedback += "Found $($_.FullName). "
        }
        else {
            $dumpWritten = $false
            $feedback += "Crash dump created for Scenario $test has file size 0. "
            $exitCode = 1
        }
    }

    Write-Host "Test $test " -ForegroundColor Cyan -NoNewline
    if ($test -eq 0) {
        if ($dumpWritten -eq $false) {
            Write-Host "PASSED" -ForegroundColor Green
        }
        else {
            Write-Host "FAILED: " -NoNewline -ForegroundColor Red
            Write-Host "Scenario 0 should not create a crash dump."
            $exitCode = 1
        }
    }
    elseif ($dumpWritten -eq $false) {
        Write-Host "FAILED: " -NoNewline -ForegroundColor Red
        Write-Host "No crash dump was found for Scenario $test with known extensions: *.dmp, *.mdmp, *.dump, *.mdump.  Searched: $(Get-Location)"
        $exitCode = 1
    }
    else {
        Write-Host "PASSED" -ForegroundColor Green
    }

    Write-Host $feedback

    Start-Sleep -Seconds 1

    Get-ChildItem -Include *.dmp, *.mdmp, *.dump, *.mdump -Recurse | ForEach-Object { Remove-Item -Path $_.FullName -Force }

    Start-Sleep -Seconds 1
}

exit $exitCode
