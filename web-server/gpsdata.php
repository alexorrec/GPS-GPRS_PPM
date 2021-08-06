<?php 

require 'config.php';

$lat = $_GET['lat'];
$lng = $_GET['lng'];
$alt = $_GET['alt'];
$spd = $_GET['spd'];

echo $lat;
echo "<br>";
echo $lng;
echo "<br>";
echo $alt;
echo "<br>";
echo $spd;


$query = "INSERT INTO gps(lat,lng,alt,spd,date)
	VALUES('".$lat."','".$lng."','".$alt."','".$spd."','".date("Y-m-d")."','".time("H:i:s")."')";

if($con->query($query) === FALSE)
	{ echo "Error: " . $query . "<br>" . $con->error; }

echo "<br>";
echo $con->insert_id;
