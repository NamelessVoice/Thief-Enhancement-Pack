<?xml version="1.0" encoding="ISO-8859-1"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>Image comparison</title>
<style type="text/css">
<!--
body
{
	background-color: black;
	color: white;
	text-align: center;
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

#compareView
{
	margin-left: auto;
	margin-right: auto;
}

#compareView img
{
	display: block;
}

#variants li
{
	display: inline-block;
	margin: 0 6px;
	padding: 4px;
	width: 100px;
	background-color: #303040;
}

table,tbody, tr, th, td {
	margin: 0;
	padding: 0;
	border: 0;
	outline: 0;
	font-weight: inherit;
	font-style: inherit;
	font-size: 100&#37;;
	font-family: inherit;
	vertical-align: baseline;
}

#compareView img:hover
{
	opacity: 0.0;
}
	
-->
</style>
<script type="text/javascript">
<!--
rising = 0;
alpha = 1.00;

var fadeTimer = null;

function onLoad()
{
	toggleFade();
}

function fade()
{
	if ( rising == 0 )
	{
		alpha -= 0.05;
		if ( alpha <= -0.50 )
			rising = 1;
	} else {
		alpha += 0.05;
		if ( alpha >= 1.5 )
			rising = 0;
	}
	document.getElementById("newImage").style.opacity = alpha;
}

function toggleFade()
{
	if ( fadeTimer == null ) {
		fadeTimer = setInterval("fade()", 100);
	} else {
		fadeTimer = clearInterval(fadeTimer);
		document.getElementById("newImage").style.opacity = null;
	}
}

function setNewImage(newImage)
{
	document.getElementById("newImage").src = newImage;
}
-->
</script>
</head>
<body onload="onLoad()">
<?PHP

$paramString="";

include("categories.inc");

if ( isset($_GET['category']) )
{
	$paramString .= "&category=" . $_GET['category'];
}
$newDir = 'fam/new/';

$pendingDir = 'fam/pending/';

$famName = $_GET['family'];
$texName = $_GET['texture'];
if ( isset($_GET['url']) ) {
	$url = $_GET['url'];
}
if ( isset($_GET['scale']) ) {
	$scale = $_GET['scale'];
}
$oldImage = "error.gif";

foreach ($DIR_NAMES as $dkey => $dvalue)
{
	$tempImage = $dvalue . $famName . '/' . $texName . '.gif';
	if ( file_exists($tempImage) )
	{
		$oldImage = $tempImage;
		break;
	}
}

if ( isset($url) )
{
	$newImage = $url;
} else {
	if ( $texName )
	{
		$newImages = array();
		$newImages = array_merge($newImages, glob($newDir . $famName . '/' . $texName . ".png"));
		$newImages = array_merge($newImages, glob($newDir . $famName . '/' . $texName . ".jpg"));
		$newImages = array_merge($newImages, glob($newDir . $famName . '/' . $texName . ".gif"));
		if ( isset($newImages[0]) ) {
			$newImage = $newImages[0];
		}
	}
}

if ( !isset($newImage) ) {
	$newImage = $oldImage;
}

print("<p>Family: " . $famName . "</p>");
print("<p>Texture: " . $texName . "</p>");

// Load material file
$main = $_SERVER['DOCUMENT_ROOT'] . '/';
$self = substr($_SERVER['PHP_SELF'], 1, strripos($_SERVER['PHP_SELF'],'/'));
$materialFile = $main . $self . $newDir . $famName . '/' . $texName . ".mtl";

if ( file_exists($materialFile) )
{
	$settingsFileData = file($materialFile);
	foreach ($settingsFileData as $settingsLine)
	{
		$parts = explode(' ', $settingsLine, 3);
		if ($parts[0] == 'tile_factor') {
			$scale_width = $parts[1];
			if ($parts[2]) {
				$scale_height = $parts[2];
			} else {
				$scale_height = $parts[1];
			}
		}
	}
}

if ( isset($scale_width) )
{
	$bgSize = (100 / $scale_width) . '% ' . (100 / $scale_height) . '%';
} else {
	$bgSize = '100%';
}

print("<table style=\"margin-left: auto; margin-right: auto;\"><tr><td style=\"background-image:url(" . $oldImage . ");background-size: $bgSize\" id=\"compareView\"><img id=\"newImage\" src=\"" . $newImage . "\" alt=\"$texName\" /></td></tr></table>");

print("<p><a href=\"javascript:toggleFade()\">Toggle fading</a></p>");

print("<noscript><p>Fading will not work with JavaScript disabled.  Mouse over the image to see the original instead.</p></noscript>");

print("<form action=\"compareImage.php\"><p>");
print("Compare with URL: ");
print("<input type=\"hidden\" name=\"family\" value=\"$famName\"/>");
print("<input type=\"hidden\" name=\"texture\" value=\"$texName\"/>");
print("<input size=\"50\" type=\"text\" name=\"url\" ");
if ( isset($url) )
{
	print("value=\"$url\"");
}
print("></input>");
print("<input type=\"submit\" value=\"Compare\" />");
print("</p></form>");


$pendingSets = glob($pendingDir . '*', GLOB_ONLYDIR);

$pending = array();
foreach ($pendingSets as $pendingSet)
{
	$setName = basename($pendingSet);
	$setImages = array();
	$setImages = array_merge($setImages, glob($pendingSet . '/' . $famName . '/' . $texName . ".png"));
	$setImages = array_merge($setImages, glob($pendingSet . '/' . $famName . '/' . $texName . ".jpg"));
	$setImages = array_merge($setImages, glob($pendingSet . '/' . $famName . '/' . $texName . ".gif"));
	if ( isset($setImages[0]) ) {
		array_push($pending, $setName);
		array_push($pending, $setImages[0]);
	}
}

if (count($pending) > 0)
{
	print('<div id="variants">');
	print("<p>Variants:</p>");
	print("<ul>");
	if ($newImage != $oldImage)
	{
		print("<li><a href=\"javascript:setNewImage('$newImage')\">EP2</a></li>");
	} else {
		print("<li><a href=\"javascript:setNewImage('$oldImage')\">Original</a></li>");
	}
	for ($i = 0; $i < count($pending); $i += 2)
	{
		$setName = $pending[$i];
		$imagePath = $pending[$i+1];
		print("<li><a href=\"javascript:setNewImage('$imagePath')\">$setName</a></li>");
	}
	print("</ul>");
	print('</div>');
}



print("<p><a href=\"index.php?family=$famName$paramString\">Return to index</a></p>");

?>
</body>
</html>