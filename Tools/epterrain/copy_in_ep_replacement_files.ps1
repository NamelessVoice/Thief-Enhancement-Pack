# Requires ImageMagick.  Set path appropriately.
$imageMagick = "C:\Program Files\ImageMagick\magick.exe"

$newFamPath = '../../Enhancement Pack/ep/fam'
$newT1FamPath = '../../Enhancement Pack/ep/Thief1/fam'
$newObjPath = '../../Enhancement Pack/ep/obj/txt16'
$newMeshPath = '../../Enhancement Pack/ep/mesh/txt16'
$originalFolders = @('fam/important', 'fam/original', 'fam/unused')

Foreach($folder in $originalFolders) {
	$folderFullPath = Resolve-Path $folder
	
	Get-ChildItem -Path $folder –Recurse -File |
	Foreach-Object {
		$Family = $_.DirectoryName.Replace("$folderFullPath\", "")
		$Texture = $_.BaseName

		if ($Family -eq 'obj') {
			# Do object things
			$searchPath = Resolve-Path $newObjPath
		} elseif ($Family -eq 'mesh') {
			$searchPath = Resolve-Path $newMeshPath
		} elseif ($Family.EndsWith('-t1')) {
			$FamilyT1 = $Family.Replace('-t1', '')
			$searchPath = Resolve-Path $newT1FamPath/$FamilyT1
		} else {
			$searchPath = Resolve-Path $newFamPath/$Family
		}
		
		New-Item -Path "fam/new" -Name "$Family" -ItemType Directory -Force | Out-Null
		$newPath = Resolve-Path "fam/new/$Family"
		
		Write-Output "$Family - $Texture"
		
		Get-ChildItem -Path $searchPath -filter "$Texture.*" -File |
		Foreach-Object {
#			Write-Output $_.FullName
			if (($_.Extension -eq '.dds') -or ($_.Extension -eq '.png')) {
				$newFile = $_.FullName
				
				$frames = Get-ChildItem -Path $searchPath -filter "$Texture`_*.*" -File | Sort-Object { [regex]::Replace($_.Name, '\d+', { $args[0].Value.PadLeft(20) }) }
				
				if ($frames) {
					# An animation!
					
					$inFiles = @($newFile)
					$frames | Foreach-Object {
						$inFiles += $_.FullName
					}

					$outputPath = "$newPath\$Texture.gif"

					# Animation rate is hard-coded at 70ms for the moment
					$animationRate = 7

					& $imageMagick -delay $animationRate $inFiles -alpha off  "$outputPath"
				} else {
				
					$outputPath = "$newPath\$Texture.png"
					# Write-Output "DDS file -> write to `"$outputPath`""
					& $imageMagick convert "$newFile" -alpha off "$outputPath"
				}
			} elseif ($_.Extension -eq '.gif') {
				Copy-Item $_.FullName -Destination $newPath
			} elseif ($_.Extension -eq '.mtl') {
				Copy-Item $_.FullName -Destination $newPath
			}
		}
	}
}