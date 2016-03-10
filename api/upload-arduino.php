<?php
// mkdir and chmod arduino folder to 777
//
//var_dump($_FILES);

$image = basename($_FILES["file"]["name"]);
$target_file = "arduino/".$image;

if (move_uploaded_file($_FILES["file"]["tmp_name"], $target_file)) {
  echo "The file $image has been uploaded to the OTA server. \n";
} else {
  echo "Sorry, there was an error uploading your file. \n";
}
?>