$slnNotFoundProjFound = $false
$failed = $false

If ( -Not (Test-Path "project.sln")) {
    Write-Host "Visual Studio Solution file not found."
    $failed = $true
    
    If ( -Not (Test-Path "src\project.vcxproj")) {
        Write-Host "Visual Studio Project file not found."
        $failed = $true
    }
    else {
        $slnNotFoundProjFound = $true
    }
}

$IncludeFiles = ("*.h","*.cpp","*.hpp")
$sourceCount = (Get-Childitem .\ -Recurse -File -Include $IncludeFiles | Measure-Object ).Count
If ($sourceCount -eq 0) {
    Write-Host "Source files not found."
    $failed = $true
}

If (Test-Path "Release") {
    Remove-Item ./Release -Recurse -Force
}
If (Test-Path "Debug") {
    Remove-Item ./Debug -Recurse -Force
}

if ($failed -eq $false) {
    $msbuild = msbuild.exe project.sln /t:Rebuild /p:Configuration=Release /p:Platform=x86
    $msbuild
}
ElseIf ($slnNotFoundProjFound -eq $true) {
    $msbuild = msbuild.exe src\project.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x86
    $msbuild
}

$buildInRelease = $true
$color = "Green"

If ( -Not (Test-Path "Release\project.exe")) {
    Write-Host "Build Error: did not compile in Release." 
    $buildInRelease = $false
    
    $msbuild = msbuild.exe Project1\project1.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x86 /p:ForceImportBeforeCppTargets=".grading\props\ReleaseConfig.props"
    $msbuild

    If (Test-Path "Release\project1.exe") {
        Write-Host "Did compile in Release with overridden targets/props (you probably forgot to link a library in Release)."
        $buildInRelease = $true
        $color = "Yellow"
    }
    Else {
        $failed = $true
        
        If ($slnNotFoundProjFound -eq $true) {
            $msbuild = msbuild.exe Project1\project1.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x86
        }
        Else {
            $msbuild = msbuild.exe project1.sln /t:Rebuild /p:Configuration=Debug /p:Platform=x86
        }

        $msbuild
        
        If (Test-Path "Debug\project1.exe") {
            Write-Host "Did compile in Debug." 
            $color = "Orange"
        }
        Else {
            Write-Host "Did not compile in Debug." 
            $color = "Red"
            
            $msbuild = msbuild.exe project1.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x86 /p:ForceImportBeforeCppTargets=".grading\props\ReleaseConfig.props"
            $msbuild
            If (Test-Path "Debug\project1.exe") {
                Write-Host "Did compile in Debug with overridden targets/props (you probably forgot to link a library in Debug)." 
            }
        }
    }
}

If ($buildInRelease -eq $false -or $failed -eq $true) {
    Write-Host "Build Failed in Release." -ForegroundColor Red
    Exit 1
}
else {
    Write-Host "Build Succeeded" -ForegroundColor $color
}
