<?php

require "piphpinclude.php";

$deviceID=$_GET['deviceID'];
$ldrID=$_GET['ldrID'];
$ldrValue=$_GET['ldrValue'];



$sql = "INSERT INTO ldrreadings(deviceID, ldrID, ldrValue) VALUES ('{$deviceID}', '{$ldrID}','{$ldrValue}')";


if (mysqli_query($con, $sql))
echo "New Record created successfully";


?>