<?php

require 'config.php';

$query = "SELECT date, time, lat, lng FROM gps ORDER BY date DESC, time";

$result = mysqli_query($con, $query);

if($result === FALSE) { 
    printf("Error: %s\n", mysqli_error($con));
}  

$response = array();

while($row = mysqli_fetch_array($result)){
    array_push($response, array('date'=>$row[0],'time'=>$row[1],'latitude'=>$row[2],'longitude'=>$row[3]));
}

echo json_encode(array($response));

?>