<?php
$DIR_NAMES = array();
$DIR_NAMES["high"] = 'fam/important/';
$DIR_NAMES["low"] = 'fam/original/';
$DIR_NAMES["unused"] = 'fam/unused/';


$origDirs = array();
$category = "default";
if ( isset($_GET['category']) && $_GET['category'] != "")
{
	$category = $_GET['category'];
}

if ( $category == "default" || $category == "low" || $category == "all" )
{
	array_push($origDirs, $DIR_NAMES['low']);
}

if ( $category == "unused"  || $category == "all" )
{
	array_push($origDirs, $DIR_NAMES['unused']);
}

if ( $category == "default" || $category == "high" || $category == "all" )
{
	array_push($origDirs, $DIR_NAMES['high']);
}
?>