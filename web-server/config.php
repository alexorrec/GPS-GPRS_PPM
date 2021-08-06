<?php 
define('DB_HOST', 'localhost'); 
define('DB_USERNAME', 'id17297422_gps'); 
define('DB_PASSWORD', 'wlWtmhp$c(-KMJd4'); 
define('DB_NAME', 'id17297422_gps_db');

date_default_timezone_set('Europe/Rome');

//Connessione al DB
$con = new mysqli(DB_HOST, DB_USERNAME, DB_PASSWORD, DB_NAME); 
 
//Se non funziona
if ($con->connect_errno) { 
    echo "Connection to database is failed: ".$con->connect_error;
    exit();
}