<?php

require "piphpinclude.php";

$deviceID=$_GET['deviceID'];
$ultraID=$_GET['ultraID'];
$ultraVal=$_GET['ultraVal'];



$sql = "INSERT INTO ultrareadings(deviceID, ultraID, ultraVal) VALUES ('{$deviceID}', '{$ultraID}','{$ultraVal}')";


if (mysqli_query($con, $sql))
echo "New Record created successfully";


?>