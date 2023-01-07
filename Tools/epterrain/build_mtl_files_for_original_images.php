<?xml version="1.0" encoding="ISO-8859-1"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>Thief Enhancement Pack - Material generator script</title>
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
$newDir = 'fam/new/';
$origDirLen = strlen($origDir);

$family = $_GET['family'];

$dirs = glob($origDir . "*", GLOB_ONLYDIR);
if (count($dirs))
{
	for($d = 0; $d < count($dirs); $d++)
	{
		$famName = substr($dirs[$d], $origDirLen);
		
		$origImg = glob($origDir . $famName . '/' . "*.gif");
		$newImg = array();
		$newImg = array_merge($newImg, glob($newDir . $famName . '/' . "*.jpg"));
		$newImg = array_merge($newImg, glob($newDir . $famName . '/' . "*.png"));
		
		$main = $_SERVER['DOCUMENT_ROOT'] . '/';
		foreach ($origImg as $mg)
		{
			$img_dims = getimagesize($mg);
			$mg_mtl = $main . $self . "mtl/" . substr($mg, 0, strripos($mg, '.')) . ".mtl";
			$mtl_string = "terrain_scale $img_dims[0] $img_dims[1]";
			$main = $_SERVER['DOCUMENT_ROOT'] . '/';
			
			$myFile = "testFile.txt";
			$fh = fopen($mg_mtl, 'w') or die("can't open file");
			fwrite($fh, $mtl_string);
			fclose($fh);
			
			print("<p>Wrote material file for $mg</p>\n");
		}
	}
}
?>
</body>
</html>