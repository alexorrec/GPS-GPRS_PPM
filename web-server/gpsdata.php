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


$sql = "INSERT INTO gps(lat,lng,alt,spd,date) 
	VALUES('".$lat."','".$lng."','".$alt."','".$spd."','".date("Y-m-d H:i:s")."')";

if($db->query($sql) === FALSE)
	{ echo "Error: " . $sql . "<br>" . $db->error; }

echo "<br>";
echo $db->insert_id;
