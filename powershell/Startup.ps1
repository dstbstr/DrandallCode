
function Ensure-VirtualDesktop {
    [CmdletBinding()]
    param()

    $existing = Get-InstalledModule -Name VirtualDesktop -ErrorAction SilentlyContinue
    if ($null -eq $existing) {
        Install-Module -Name VirtualDesktop -Scope CurrentUser -Force
    }

    Import-Module -Name VirtualDesktop -ErrorAction Stop
}

function Setup-Desktop1 {
    [CmdletBinding()]
    param()
    Switch-Desktop 0;
    slack.exe;
}

function Setup-Desktop2 {
    [CmdletBinding()]
    param()

    $recentProjectsPath = 'Z:\source\repos\RecentProjects.txt'

    function New-ProjectTab {
        [CmdletBinding()]
        param(
            [Parameter(Mandatory = $true)]
            [string]$Candidate,

            [Parameter(Mandatory = $true)]
            [string]$RepositoryPath,

            [Parameter(Mandatory = $true)]
            [string]$Target,

            [Parameter(Mandatory = $true)]
            [string]$WindowName
        )

        $tabTitle = $Candidate.Trim()
        $escapedCandidate = $Candidate.Replace("'", "''")
        $escapedRepositoryPath = $RepositoryPath.Replace("'", "''")
        $escapedTarget = $Target.Replace("'", "''")
        $tabScript = "& { if (Get-Command -Name repo -ErrorAction SilentlyContinue) { repo '$escapedCandidate' } else { Set-Location '$escapedRepositoryPath' }; Start-Process -FilePath 'devenv.exe' -ArgumentList @('$escapedTarget') -WorkingDirectory '$escapedRepositoryPath' }"
        $encodedCommand = [Convert]::ToBase64String([Text.Encoding]::Unicode.GetBytes($tabScript))

        Start-Process -FilePath 'wt.exe' -ArgumentList @(
            '-w',
            $WindowName,
            'new-tab',
            '--title',
            $tabTitle,
            '-d',
            $RepositoryPath,
            '--',
            'powershell.exe',
            '-NoExit',
            '-EncodedCommand',
            $encodedCommand
        )
    }

    function Get-VisualStudioTarget {
        [CmdletBinding()]
        param(
            [Parameter(Mandatory = $true)]
            [string]$RepositoryPath
        )

        $slnx = Get-ChildItem -Path $RepositoryPath -File -Filter '*.slnx' -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($null -ne $slnx) {
            return $slnx.FullName
        }

        $sln = Get-ChildItem -Path $RepositoryPath -File -Filter '*.sln' -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($null -ne $sln) {
            return $sln.FullName
        }

        $cmake = Join-Path -Path $RepositoryPath -ChildPath 'CMakeLists.txt'
        if (Test-Path -Path $cmake -PathType Leaf) {
            return $RepositoryPath
        }

        return $null
    }

    $count = Get-DesktopCount;
    if ($count -lt 2) {
        New-Desktop;
    }
    Switch-Desktop 1;

    $terminalWindowName = "desktop2-startup-$([guid]::NewGuid().ToString('N'))"
    $openedProjectTab = $false

    if (-not (Get-Command -Name repo -ErrorAction SilentlyContinue)) {
        Write-Warning "Repo command not found in this session."
        return
    }

    if (-not (Test-Path -Path $recentProjectsPath -PathType Leaf)) {
        Write-Warning "Recent projects file not found at '$recentProjectsPath'."
        return
    }

    $entries = Get-Content -Path $recentProjectsPath -ErrorAction SilentlyContinue |
        Where-Object { -not [string]::IsNullOrWhiteSpace($_) } |
        Where-Object { -not $_.Trim().StartsWith('#') }

    foreach ($candidate in $entries) {
        $startPath = (Get-Location).Path
        repo $candidate
        $repoPath = (Get-Location).Path

        if (-not (Test-Path -Path $repoPath -PathType Container)) {
            Write-Warning "Skipping '$candidate': repo command did not resolve a valid folder."
            Set-Location -Path $startPath
            continue
        }

        $target = Get-VisualStudioTarget -RepositoryPath $repoPath
        if ($null -eq $target) {
            Write-Warning "Skipping '$repoPath': no .slnx, .sln, or CMakeLists.txt found."
            Set-Location -Path $startPath
            continue
        }

        New-ProjectTab -Candidate $candidate -RepositoryPath $repoPath -Target $target -WindowName $terminalWindowName
        if (-not $openedProjectTab) {
            Start-Sleep -Milliseconds 500
            $openedProjectTab = $true
        }

        Set-Location -Path $startPath
    }
}

function Invoke-StartupDesktopSetup {
    [CmdletBinding()]
    param()

    Ensure-VirtualDesktop
    Setup-Desktop1
    Setup-Desktop2
}

Invoke-StartupDesktopSetup