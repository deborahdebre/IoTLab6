<?php
//include( 'index.html' );
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "IoTLab6";

$con = mysqli_connect($servername, $username, $password,$dbname);

$tankID=$_GET['deviceID'];

$sql2= "SELECT ldrID,ldrValue, timetaken FROM LDRReadings WHERE deviceID = '{$tankID}' ORDER BY timetaken DESC LIMIT 10";


$q2 = mysqli_query($con,$sql2);

$data= array();

while ($row=mysqli_fetch_object($q2))
{
    $data[]=$row;

}

echo json_encode($data);




?>


