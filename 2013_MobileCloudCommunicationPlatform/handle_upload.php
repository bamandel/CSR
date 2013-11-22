<?php
//credentials in Google drive
$user ="";
$pass = "";
$hostname = "";
$dbselect="";

$fileUpload = true; //allows files from android to be uploaded to server
$dbInserts = true; //allows files to be inserted into the database

$link = mysqli_connect($hostname,$user,$pass,$dbselect); //connect to database
	if(!$link){
		die('Connect Error ('.mysqli_connect_errno().')'.mysqli_connect_error());
	}

if ($fileUpload){
	$target_path  = "fallDetection/"; //where to upload the file
	$target_path = $target_path . basename( $_FILES['uploadedfile']['name']);
	if(move_uploaded_file($_FILES['uploadedfile']['tmp_name'], $target_path)) {
	 echo "The file ".  basename( $_FILES['uploadedfile']['name']).
	 " has been uploaded";
	}
	else{
		echo "There was an error uploading the file, please try again!";
	}
}
else{
	echo "File Upload is Off<br/>";
}

if ($dbInserts){
	$handle = fopen("fallDetection/".$_FILES['uploadedfile']['name'], "r"); //open file for reading
	if($handle){ //if file successfully opened.
		$dataEvent = fgetcsv($handle, 1000, ","); //input dataEvent info in array
		$user = fgetcsv($handle, 1000, ","); //input user info in array
		
		/***   INSERT QUERIES   ***/
		$checkName = mysqli_query($link, "SELECT * FROM User WHERE name = '".$user[5]."';");
		if(mysqli_num_rows($checkName) == 0){ //if name does not exist
			$userInsert = 	"INSERT INTO `User` ".
							"(`UserID`, `Name`, `Age`, `Height`, `Weight`, `Sex`, `Activity_Level`, `Email`, `Alarm_Number`, `DeviceID`, `PositionID`, `BMI`)".
							"VALUES (NULL, '".$user[5]."', '".$user[0]."', '".$user[4]."', '".$user[2]."', NULL, NULL, NULL, NULL, NULL, NULL, '".$user[1]."');";
			mysqli_query( $link, $userInsert ); //inserts user info
			$nameID = mysqli_insert_id( $link );
		}
		else{
			$nameID=mysqli_fetch_row($checkName)[0]; //stores the first UserID of the name given from previous query
		}
		
		$dataEventInsert = 	"INSERT INTO `DataEvent` ".
							"(`DataEventID`, `User_ID`, `PositionStart`, `PositionEnd`, `ADLActual`, `ADLClassify`, `CaptureRate`, `CollectionStartTime`, `CollectionEndTime`)".
							"VALUES (NULL, '$nameID', NULL, NULL, '".$dataEvent[0]."', '".$dataEvent[1]."' , '".$dataEvent[4]."', '".$dataEvent[2]."', '".$dataEvent[3]."');";
		mysqli_query($link, $dataEventInsert); //inserts data event info
		
		$dataEventID = mysqli_insert_id($link); //temp variable for inserting in the loop
		// this loops through each line of your csv, putting the values into array elements
		while(($dataItem = fgetcsv($handle, 1000, ",")) !== FALSE){
			$rss = sqrt($dataItem[1] * $dataItem[1] + $dataItem[2] * $dataItem[2] + $dataItem[3] * $dataItem[3]);
			$dataItemInsert = 	"INSERT INTO `DataItem` ".
								"(`DataItemID`, `Timestamp`, `DataEventID`, `Gx`, `Gy`, `Gz`, `RSS`)".
								"VALUES (NULL, '".$dataItem[0]."', '$dataEventID', '".$dataItem[1]."', '".$dataItem[2]."', '".$dataItem[3]."', '$rss');";
			mysqli_query($link, $dataItemInsert);
		}
	}
	else {
		echo "Can't open file.";
	}
	fclose($handle);
}
else{
	echo "Database Insert is Off";
}
	mysqli_close($link);
?>