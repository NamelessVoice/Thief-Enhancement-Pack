<?xml version="1.0" encoding="ISO-8859-1"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>Thief Enhancement Pack - Terrain progress</title>
<style type="text/css">
<!--
body
{
	background-color: black;
	color: white;
}

a
{
	color: red;
}

a:visited
{
	color: #CC3300;
}

a:hover
{
	color: #00FF00;
}

img
{
	border:none;
}

a img
{
	text-decoration: none;
	display: block;
}

table
{
	margin-left: auto;
	margin-right: auto;
}
	
-->
</style>
</head>
<body>
<?PHP
$hmain = "/";
$self = substr($_SERVER['PHP_SELF'], 1, strripos($_SERVER['PHP_SELF'],'/'));

$origDir = 'fam/original/';

$paramString="";

include("categories.inc");

if ( isset($_GET['category']) )
{
	$paramString .= "&category=" . $_GET['category'];
}


$newDir = 'fam/new/';
$pendingDir = 'fam/pending/';
$totalOrig = 0;
$totalNew = 0;
$origCount = array();
$newCount = array();
$TOTAL_NAME = "(Total)";

if ( isset($_GET['family']) && $_GET['family'] != "" ) {
	$family = $_GET['family'];
}
$familyImages = array();

for ($k = 0; $k < count($origDirs); ++$k)
{
	$origDir = $origDirs[$k];
	$origDirLen = strlen($origDir);

	$dirs = glob($origDir . "*", GLOB_ONLYDIR);
	if (count($dirs))
	{
		
		for($d = 0; $d < count($dirs); $d++)
		{
			$famName = substr($dirs[$d], $origDirLen);
			
			$origImg = glob($origDir . $famName . '/' . "*.gif");
			$newImg = array();
			
			for($i = 0; $i < count($origImg); $i++)
			{
				$searchTex = str_replace($origDir, $newDir, $origImg[$i]);
				
				$searchTexPng = str_replace(".gif", ".png", $searchTex);
				if ( file_exists($searchTexPng) )
				{
					array_push($newImg, $searchTexPng);
				} else if ( file_exists($searchTex) )
				{
					array_push($newImg, $searchTex);
				}
			}
	/*
			$newImg = array();
			$newImg = array_merge($newImg, glob($newDir . $famName . '/' . "*.jpg"));
			$newImg = array_merge($newImg, glob($newDir . $famName . '/' . "*.png"));
			$newImg = array_merge($newImg, glob($newDir . $famName . '/' . "*.gif")); // for animated gifs
	*/		
			if ( !isset($origCount[$TOTAL_NAME]) ) {
				$origCount[$TOTAL_NAME] = 0;
			}
				
			if ( !isset($newCount[$TOTAL_NAME]) ) {
				$newCount[$TOTAL_NAME] = 0;
			}
				
			$origCount[$TOTAL_NAME] += count($origImg);
			$newCount[$TOTAL_NAME] += count($newImg);
			
			if ( !isset($newCount[$famName]) ) { $newCount[$famName] = 0; }
			if ( !isset($origCount[$famName]) ) { $origCount[$famName] = 0; }
			
			$newCount[$famName] += count($newImg);
			$origCount[$famName] += count($origImg);
			
			if (!isset($familyImages[$famName]) )
			{
				$familyImages[$famName] = array();
			}
			for ($z = 0; $z < count($origImg); ++$z)
			{
				$trimLen = strlen($dirs[$d]) + 1;
				$texName = substr($origImg[$z], $trimLen);
			
				$familyImages[$famName][$texName] = $origImg[$z];
			}
		}

	}
}

ksort($origCount);

$v = $origCount[$TOTAL_NAME];
unset($origCount[$TOTAL_NAME]);
$origCount[$TOTAL_NAME] = $v;

print("<form action=\"index.php\"><table><tr><td>");
print("Family: ");
print("\n<select name=\"family\" onchange=\"submit()\">");
print("\n\t<option value=\"\">(Overview)</option>");

foreach ($origCount as $key => $value)
{
	if ( $key != "(Total)" )
	{
		print("\n\t<option value=\"$key\"");

		if ( isset($family) && $key == $family )
		{
			print(" selected=\"selected\"");
		}
		print(">$key ($newCount[$key] / $origCount[$key])</option>");
	}
}
print("</select>");
print("\n<select name=\"category\" onchange=\"submit()\">");
print("\n\t<option value=\"\">Default</option>");
foreach ($DIR_NAMES as $dkey => $dvalue)
{
	print("\n\t<option value=\"$dkey\"");

	if ( $dkey == $category )
	{
		print(" selected=\"selected\"");
	}
	print(">$dkey</option>");
}
print("\n\t<option value=\"all\"");
if ( $category == "all" )
{
	print(" selected=\"selected\"");
}
print(">All</option>");

print("</select>");
print("\n</td></tr></table></form>\n\n");


if ( isset($family) )
{
	print("<table border=\"1\">\n");
	print("<tr>");
	print("<th>Family</th>");
	print("<th>Name</th>");
	print("<th>Image</th>");
	print("<th>Replacement</th>");
	print("<th>Variants</th>");
	print("<th>Priority</th>");
	print("</tr>");
	$img = $familyImages[$family];
	if ( isset($img) )
	{	
		ksort($img);
	}
	$famName = $family; //substr($dirs[$d], $origDirLen);
	if (count($img))
	{
		foreach($img as $texName => $texPath)
//		for($i = 0; $i < count($img); $i++)
		{
				$texNameNoExtension = substr($texName, 0, strripos($texName, '.'));
				$newImageName = $newDir . $famName . '/' . $texNameNoExtension;
				$newImages = glob($newImageName . '.png');
				$newImages = array_merge($newImages, glob($newImageName . '.gif'));
				$newImages = array_merge($newImages, glob($newImageName . '.jpg'));
				$pendingImages = glob($pendingDir . '*/' . $famName . '/' . $texNameNoExtension . '.{gif,png,jpg,bmp}', GLOB_BRACE);

				print("<tr>\n");
				
				print("\n\t<td>" . $famName . '</td>');
				print("\n\t<td>" . $texName . '</td>');
				print("\n\t<td>\n\t\t");
				print("<a href=\"compareImage.php?family=" . $famName . "&amp;texture=" . $texNameNoExtension . $paramString ."\"><img src=\"$texPath\" alt=\"Image\" /></a>");
				print("\n\t</td>\n");
				print("\n\t<td>");
				
				if ( count($newImages) == 0 )
				{
					print("No.");
				} else {
					print("<a href=\"compareImage.php?family=" . $famName . "&amp;texture=" . $texNameNoExtension . $paramString ."\">Yes</a>");
				}

				print("\n\t</td>\n");
				print("\n\t<td>");
				if ( count($pendingImages) == 0 )
				{
					print("No.");
				} else {
					print("<a href=\"compareImage.php?family=" . $famName . "&amp;texture=" . $texNameNoExtension . $paramString ."\">Yes</a>");
				}
				print("</td>");
				
				$catName = "Error";
				foreach ($DIR_NAMES as $dkey => $dvalue)
				{
					if ( 0 === strpos($texPath, $dvalue) )
					{
						$catName = $dkey;
						break;
					}
				}
				print("\n\t<td>" . $catName . '</td>');
				print("</tr>\n");
		}
	}	

	print("\n</table>\n<p>&nbsp;</p>\n");
} else {
	print("<table border=\"1\">\n");
	print("<tr>");
	print("<th>Family</th>");
	print("<th>Percentage complete</th>");
	print("<th>Textures</th>");
	print("</tr>");
	foreach ($origCount as $key => $value)
	{
		print("\n<tr>");
		if ( $key == $TOTAL_NAME )
		{
			print("\n\t<td>$key</td>");
			print("\n\t<td style=\"width: 200px; background-color: #111111; padding: 0px; margin: 0px;\">");
		} else {
			print("\n\t<td><a href=\"index.php?family=$key$paramString\">$key</a></td>");
			print("\n\t<td style=\"width: 200px; background-color: #333333; padding: 0px; margin: 0px;\">");
		}
		$width = 0;
		if ( $newCount[$key] > 0 )
		{
//			if ( $key == $TOTAL_NAME )
//			{
//				//$width = round($newCount[$key] / $value * 100, 4);
//				$width = number_format($newCount[$key] / $value * 100, 1);
//			} else {
				$width = intval($newCount[$key] / $value * 100);
//			}
			print("<div style=\"background-color: green; width: " . intval($width*2) . "px\">$width%</div>");
		} else {
			print("$width%");
		}
		print("</td>");
		print("\n\t<td>");
		print($newCount[$key] . " / " . $value);
		print("</td>");
		print("\n</tr>");
	}
	print("</table>\n");
}
?>
</body>
</html>