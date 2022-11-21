<?php
//include( 'index.html' );
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "IoTLab6";

$con = mysqli_connect($servername, $username, $password,$dbname);

$tankID=$_GET['deviceID'];

$sql3= "SELECT ultraID, ultraVal, Utimetaken FROM UltraReadings WHERE deviceID = '{$tankID}' ORDER BY Utimetaken DESC LIMIT 10";




$q3 = mysqli_query($con,$sql3);
$data= array();

while ($row=mysqli_fetch_object($q3))
{
    $data[]=$row;

}


echo json_encode($data);



?>

