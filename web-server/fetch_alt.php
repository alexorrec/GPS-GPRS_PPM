<?php

require 'config.php';

$query = "SELECT CAST(CONCAT(date, ' ', time) as datetime) as datetime, alt 
            FROM gps 
            ORDER BY datetime desc";

$result = mysqli_query($con, $query);

if($result === FALSE) { 
    printf("Error: %s\n", mysqli_error($con));
}  

$response = array();

while($row = mysqli_fetch_array($result)){
    array_push($response, array('date'=>$row[0],'altitude'=>$row[1]));
}

echo json_encode(array($response));

?>