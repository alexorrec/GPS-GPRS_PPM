<?php
require 'config.php';

$query = "SELECT lat, lng, time, date FROM gps WHERE id = (SELECT MAX(ID) FROM gps)";

$result = mysqli_query($con, $query);

if($result === FALSE) { 
    printf("Error: %s\n", mysqli_error($con));
}  

//$response = array();

while($row = mysqli_fetch_array($result)){
    $response = array('latitude'=>$row[0],'longitude'=>$row[1],'time'=>$row[2],'date'=>$row[3]);
}

echo json_encode(array($response));
?>