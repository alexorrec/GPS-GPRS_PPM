<?php 
define('DB_HOST', 'localhost'); 
define('DB_USERNAME', 'id17297422_gps'); 
define('DB_PASSWORD', 'wlWtmhp$c(-KMJd4'); 
define('DB_NAME', 'id17297422_gps_db');

date_default_timezone_set('Europe/Rome');

//Connessione al DB
$db = new mysqli(DB_HOST, DB_USERNAME, DB_PASSWORD, DB_NAME); 
 
//Se non funziona
if ($db->connect_errno) { 
    echo "Connection to database is failed: ".$db->connect_error;
    exit();
}