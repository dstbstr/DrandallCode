if(test-path 'C:\Program Files\Notepad++\notepad++.exe') {
	Set-Alias -name np -value 'C:\Program Files\Notepad++\notepad++.exe'
} else {
	Set-Alias -name np -value 'C:\Program Files (x86)\Notepad++\notepad++.exe'
}
Set-Alias -name touch -value New-Item

function ... {cd ..\..}
function .... {cd ..\..\..}
function ..... {cd ..\..\..\..}
function ...... {cd ..\..\..\..\..}

function repo {
	[CmdletBinding()]
	param(
		[Parameter(Position=0)]
		[string]$Name
	)
	
	$root = 'Z:\source\repos'
	
	if(-not $Name) {
		cd $root
		return
	}
	$level1 = ls $root -Directory -ErrorAction SilentlyContinue
	$level2 = foreach($d in $level1) {
		ls $d.FullName -Directory -ErrorAction SilentlyContinue
	}
	$dirs = @($level1 + $level2)
	
	$matches = $dirs | Where-Object {$_.Name -like "*$Name*" } | Sort-Object Name
	switch ($matches.Count) {
		0 { Write-Warning "Couldn't find $Name"; break; }
		1 { cd $matches[0].FullName; break; }
		default {
			$keys = @('0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z')
			$choices = for($i = 0; $i -lt $matches.Count; $i++) {
				$rel = $matches[$i].FullName.Substring($root.Length).TrimStart('\')
				[System.Management.Automation.Host.ChoiceDescription]::new("&$($keys[$i]) $($rel)")
			}
			$selection = $Host.UI.PromptForChoice("Multiple matches found", "Select the desired match", $choices, 0)
			if($selection -ge 0 -and $selection -lt $choices.Count) {
				$target = $matches[$selection]
				cd $target.FullName
			} else {
				Write-Host "cancelled"
			}
		}
	}
}

function profile { np $Profile }

[console]::backgroundcolor = "black"
[console]::foregroundcolor = "green"
