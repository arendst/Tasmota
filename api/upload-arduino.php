<?php
// mkdir and chmod arduino folder to 777
//
//var_dump($_FILES);

/**
 * GZIPs a file on disk (appending .gz to the name)
 *
 * From http://stackoverflow.com/questions/6073397/how-do-you-create-a-gz-file-using-php
 * Based on function by Kioob at:
 * http://www.php.net/manual/en/function.gzwrite.php#34955
 *
 * @param string $source Path to file that should be compressed
 * @param integer $level GZIP compression level (default: 9)
 * @return string New filename (with .gz appended) if success, or false if operation fails
 */
function gzCompressFile($source, $level = 9){
  $dest = $source . '.gz';
  $mode = 'wb' . $level;
  $error = false;
  if ($fp_out = gzopen($dest, $mode)) {
      if ($fp_in = fopen($source,'rb')) {
          while (!feof($fp_in))
              gzwrite($fp_out, fread($fp_in, 1024 * 512));
          fclose($fp_in);
      } else {
          $error = true;
      }
      gzclose($fp_out);
  } else {
      $error = true;
  }
  if ($error)
      return false;
  else
      return $dest;
}

$image = basename($_FILES["file"]["name"]);
$target_file = "arduino/".$image;
$hostname = $_SERVER['SERVER_NAME'];

if (move_uploaded_file($_FILES["file"]["tmp_name"], $target_file)) {
  gzCompressFile($target_file);
  echo "The files $image and $image.gz have been uploaded to OTA server $hostname. \n";
} else {
  echo "Sorry, there was an error uploading your file $image to OTA server $hostname. \n";
}
?>