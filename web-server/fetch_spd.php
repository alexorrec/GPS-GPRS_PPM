<?php

require 'config.php';

$query = "SELECT date, MAX(spd), MIN(spd), AVG(spd) FROM gps GROUP BY date";

$result = mysqli_query($con, $query);

if($result === FALSE) { 
    printf("Error: %s\n", mysqli_error($con));
}  

$response = array();

while($row = mysqli_fetch_array($result)){
    array_push($response, array('date'=>$row[0],'maxspeed'=>$row[1],'minspeed'=>$row[2],'avgspeed'=>$row[3]));
}

echo json_encode(array($response));

?>