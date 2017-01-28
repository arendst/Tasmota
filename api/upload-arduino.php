<?php
// mkdir and chmod arduino folder to 777
//
//var_dump($_FILES);

$image = basename($_FILES["file"]["name"]);
$target_file = "arduino/".$image;
$hostname = $_SERVER['SERVER_NAME'];

if (move_uploaded_file($_FILES["file"]["tmp_name"], $target_file)) {
  echo "The file $image has been uploaded to OTA server $hostname. \n";
} else {
  echo "Sorry, there was an error uploading your file $image to OTA server $hostname. \n";
}
?>