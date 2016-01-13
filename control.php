<?php
session_start();
//////////////////////////////
// EDIT THESE TWO VARIABLES //
//////////////////////////////
$MySQLUsername = "USERNAME HERE";
$MySQLPassword = "PASSWORD HERE";
/////////////////////////////////
// DO NOT EDIT BELOW THIS LINE //
/////////////////////////////////
$MySQLHost = "localhost";
$MySQLDB = "gpio";
If (($MySQLUsername == "USERNAME HERE") || ($MySQLPassword == "PASSWORD HERE")){
  print 'ERROR - Please set up the script first';
	exit();
}
$dbConnection = mysql_connect($MySQLHost, $MySQLUsername, $MySQLPassword);
mysql_select_db($MySQLDB, $dbConnection);
If (isset($_POST['action'])){
	If ($_POST['action'] == "setPassword"){
		$password1 = $_POST['password1'];
		$password2 = $_POST['password2'];
		If ($password1 != $password2){
			header('Location: control.php');
		}
		$password = mysql_real_escape_string($_POST['password1']);
		If (strlen($password) > 28){
			mysql_close();
			header('location: control.php');
		}
		$resetQuery = "SELECT username, salt FROM users WHERE username = 'admin';";
		$resetResult = mysql_query($resetQuery);
		If (mysql_num_rows($resetResult) < 1){
			mysql_close();
			header('location: control.php');
		}
		$resetData = mysql_fetch_array($resetResult, MYSQL_ASSOC);
		$resetHash = hash('sha256', $salt . hash('sha256', $password));
		$hash = hash('sha256', $password);
		function createSalt(){
			$string = md5(uniqid(rand(), true));
			return substr($string, 0, 8);
		}
		$salt = createSalt();
		$hash = hash('sha256', $salt . $hash);
		mysql_query("UPDATE users SET salt='$salt' WHERE username='admin'");
		mysql_query("UPDATE users SET password='$hash' WHERE username='admin'");
		mysql_close();
		header('location: control.php');
	}
}
If ((isset($_POST['username'])) && (isset($_POST['password']))){
	$username = mysql_real_escape_string($_POST['username']);
	$password = mysql_real_escape_string($_POST['password']);
	$loginQuery = "SELECT UserID, password, salt FROM users WHERE username = '$username';";
	$loginResult = mysql_query($loginQuery);
	If (mysql_num_rows($loginResult) < 1){
		mysql_close();
		header('location: control.php?error=incorrectLogin');
	}
	$loginData = mysql_fetch_array($loginResult, MYSQL_ASSOC);
	$loginHash = hash('sha256', $loginData['salt'] . hash('sha256', $password));
	If ($loginHash != $loginData['password']){
		mysql_close();
		header('location: control.php?error=incorrectLogin');
	} else {
		session_regenerate_id();
		$_SESSION['username'] = "admin";
		$_SESSION['userID'] = "1";
		mysql_close();
		header('location: control.php');
	}
}
If ((!isset($_SESSION['username'])) || (!isset($_SESSION['userID']))){
	print '
	<html>
	<head>
	<title>Sprinkler Controller - Login</title>
	</head>
	<body>
	<table border="0" align="center">
	<form name="login" action="control.php" method="post">
	<tr>
	<td>Username: </td><td><input type="text" name="username"></td>
	</tr>
	<tr>
	<td>Password: </td><td><input type="password" name="password"></td>
	</tr>
	<tr>
	<td colspan="2" align="center"><input type="submit" value="Log In"></td>
	</tr>
	</form>
	</table>
	</body>
	</html>
	';
	die();
}
If (isset($_GET['action'])){
	If ($_GET['action'] == "logout"){
		$_SESSION = array();
		session_destroy();
		header('Location: control.php');
	} else If ($_GET['action'] == "setPassword"){
		print '
		<form name="changePassword" action="control.php" method="post">
		<input type="hidden" name="action" value="setPassword">
		<p>Enter New Password: <input type="password" name="password1">  Confirm: <input type="password" name="password2"><input type="submit" value="submit"></p>
		</form>
		';
	} else {
		$action = $_GET['action'];
		$pin = mysql_real_escape_string($_GET['pin']);
		if ($action == "turnOn"){
			$setting = "1";
			$manDur = mysql_real_escape_string($_GET['manualDuration']);
			mysql_query("UPDATE manual_start SET manualDuration='$manDur' WHERE pinNumber='$pin';");
			mysql_query("UPDATE manual_start SET pinStart='$setting' WHERE pinNumber='$pin';");
			mysql_close();
			header('Location: control.php');
		} else If ($action == "turnOff"){
			$setting = "0";
			mysql_query("UPDATE manual_start SET pinStart='$setting' WHERE pinNumber='$pin';");
			mysql_close();
			header('Location: control.php');
		} else IF ($action == "manualMode") {
                        $setting = "1";
                        mysql_query("UPDATE pinMode SET pinMode='$setting' WHERE pinNumber='$pin';");
			mysql_close();
			header('Location: control.php');
                } else IF ($action == "autoMode") {
                        $setting = "0";
                        mysql_query("UPDATE pinMode SET pinMode='$setting' WHERE pinNumber='$pin';");
			mysql_close();
			header('Location: control.php');
                }
                else IF ($action == "autoEdits") {
			$pin = mysql_real_escape_string($_GET['pin']);
                        $query = mysql_query("SELECT pinHours, pinMins, autoDuration FROM auto_start WHERE pinNumber='$pin';");
			$query1 = mysql_query("SELECT pinAutoMode FROM pinAutoMode WHERE pinNumber='$pin';");
                        $query2 = mysql_query("SELECT pinDays FROM pinDays WHERE pinNumber='$pin';");
			$descRow = mysql_fetch_assoc($query);
			$descRow1 = mysql_fetch_assoc($query1);
			$descRow2 = mysql_fetch_assoc($query2);
			$pinAutoMode = $descRow1['pinAutoMode'];
                        $pinMins = $descRow['pinMins'];
			$pinHours = $descRow['pinHours'];
			$pinDuration = $descRow['autoDuration'];
			$pinDays = $descRow2['pinDays'];
			$sunday = $monday = $tuesday = $wednesday = $thursday = $friday = $saturday = 0;
			if (($pinDays % 2) == 1) 
			{
				$sunday = 1;
				$pinDays = $pinDays-1;
			}
			while ($pinDays != 0){
				$x = 0;
				$y = $pinDays;
				while ($y != 1)
				{
					if ($y %2 == 1)
					{
						$y = 1;
						
					}
					else
					{
						$y = $y/2;
						$x++;
					}
				}
				$pinDays = $pinDays-(pow(2,$x));
				if ($x==1){$monday=1;}
				if ($x==2){$tuesday=1;}
                                if ($x==3){$wednesday=1;}
                                if ($x==4){$thursday=1;}
                                if ($x==5){$friday=1;}
                                if ($x==6){$saturday=1;}
			}
		    print '<html><head><title>Auto Settings for Pin ' . $pinDays . '</title></head><body>';
		    print '<form name="autoEdits" action="control.php" method="get"> ';
		    print '<table name="Times" border="1" cellpadding="3">
			<tr><th>Hour to Start (24 Hr Clock)</th><th>Minute to Start</th><th>Duration (in Minutes)</th>
			<tr><td><input type="text" name="hours" value="' .$pinHours . '"></td>
		        <td><input type="text" name="mins" value="' . $pinMins . '"></td>
			<td><input type="text" name="duration" value="' . $pinDuration . '"></td>
			</table>';
                    print '<table name="autoMode" border="1" cellpadding="3">';
		    print '<tr><th>Even Days</th><th>Odd Days</th><th>User Defined</th>';
			if ($pinAutoMode==0){ print ' <tr><td><input type="radio" name="daySelector" value="0" checked> Even</td>';}
			else { print ' <tr><td><input type="radio" name="daySelector" value="0"> Even</td>';}
                        if ($pinAutoMode==1){ print ' <td><input type="radio" name="daySelector" value="1" checked> Odd</td>';}
                        else { print ' <td><input type="radio" name="daySelector" value="1"> Odd</td>';}
                        if ($pinAutoMode==2){ print ' <td><input type="radio" name="daySelector" value="2" checked> User Defined</td>';}
                        else { print ' <td><input type="radio" name="daySelector" value="2"> User Defined</td></tr>';}
		   print ' </table>';
                    print '<table name="days" border="1" cellpadding="7">';
                    print '<tr><th>Sunday</th><th>Monday</th><th>Tuesday</th><th>Wednesday</th><th>Thursday</th><th>Friday</th><th>Saturday</th></tr>';
                        if ($sunday == 1) { print '<tr><td><input type="checkbox" name="formDays[]" value=0 checked/> Sunday</td>';}
			else { print '<tr><td><input type="checkbox" name="formDays[]" value=0 /> Sunday</td>';}
		    	if ($monday == 1) { print '<td><input type="checkbox" name="formDays[]" value=1 checked/> Monday</td>';}
                        else { print '<td><input type="checkbox" name="formDays[]" value=1 /> Monday</td>';}
			if ($tuesday == 1) { print '<td><input type="checkbox" name="formDays[]" value=2 checked/> Tuesday</td>';}
                        else { print '<td><input type="checkbox" name="formDays[]" value=2 /> Tuesday</td>';}
			if ($wednesday == 1) { print '<td><input type="checkbox" name="formDays[]" value=3 checked/> Wednesday</td>';}
                        else { print '<td><input type="checkbox" name="formDays[]" value=3 /> Wednesday</td>';}
			if ($thursday == 1) { print '<td><input type="checkbox" name="formDays[]" value=4 checked/> Thursday</td>';}
                        else { print '<td><input type="checkbox" name="formDays[]" value=4 /> Thursday</td>';}
			if ($friday == 1) { print '<td><input type="checkbox" name="formDays[]" value=5 checked/> Friday</td>';}
                        else { print '<td><input type="checkbox" name="formDays[]" value=5 /> Friday</td>';}
			if ($saturday == 1) { print '<td><input type="checkbox" name="formDays[]" value=6 checked/> Saturday</td>';}
                        else { print '<td><input type="checkbox" name="formDays[]" value=6 /> Saturday</td>';}
			print '</table>
                    <input type = "hidden" name = "action" value = "autoUpdate">
                    <input type = "hidden" name = "pin" value = "' . $pin . '">
                    <tr><td><input type = "submit" value = "Confirm"></td></tr>
                    </form>
                    </body></html>
                    ';
                    mysql_close();
                    
                    } else IF ($action =="autoUpdate"){
			$pin = mysql_real_escape_string($_GET['pin']);
			$hours = mysql_real_escape_string($_GET['hours']);
			$mins = mysql_real_escape_string($_GET['mins']);
			$duration = mysql_real_escape_string($_GET['duration']);
			$daytest = $_GET['daySelector'];
			$day= mysql_real_escape_string($daytest);
			$dayInt =0;
                        mysql_query("UPDATE auto_start SET pinHours='$hours' WHERE pinNumber='$pin';");
                        mysql_query("UPDATE auto_start SET pinMins='$mins' WHERE pinNumber='$pin';");
                        mysql_query("UPDATE auto_start SET autoDuration='$duration' WHERE pinNumber='$pin';");
			mysql_query("UPDATE pinAutoMode SET pinAutoMode='$day' WHERE pinNumber='$pin';");
			mysql_query("UPDATE auto_start SET needUpdate=1 WHERE pinNumber='$pin';");
			for($i=0;$i<7;$i++)
			{
				if (!empty($_GET['formDays']))
				{
					foreach($_GET['formDays'] as $chkval)
					{
						if($chkval == $i)
						{
							$dayInt = $dayInt + pow(2,$i);
						}
					}
				}
			}
			mysql_query("UPDATE pinDays SET pinDays='$dayInt' WHERE pinNumber='$pin';");
                        header('Location: control.php');
		    } else IF ($action =="edit"){
			$pin = mysql_real_escape_string($_GET['pin']);
			$query = mysql_query("SELECT pinDescription FROM pinDescription WHERE pinNumber='$pin';");
			$descRow = mysql_fetch_assoc($query);
			$description = $descRow['pinDescription'];
			print '
			<html><head><title>Update Pin ' . $pin . '</title></head><body>
			<table border="0">
			<form name="edit" action="control.php" method="get">
			<input type="hidden" name="action" value="update">
			<input type="hidden" name="pin" value="' . $pin . '">
			<tr>
			<td><p>Description: </p></td><td><input type="text" name="description" value="' . $description . '"></td><td><input type="submit" value="Confirm"></td>
			</tr>
			</form>
			</table>
			</body></html>
			';
			mysql_close();
		} else IF ($action =="update"){
			$pin = mysql_real_escape_string($_GET['pin']);
			$description = mysql_real_escape_string($_GET['description']);
			mysql_query("UPDATE pinDescription SET pinDescription='$description' WHERE pinNumber='$pin';");
			header('Location: control.php');
		} else {
			header('Location: control.php');
		}
	}
} else {
	print '
		<html>
		<head>
		<title>Main Sprinkler Page</title>
		</head>
		<font face="verdana">
		<p>Main Page   <a href="control.php?action=setPassword">Change Password</a></p>
		';
		$query = mysql_query("SELECT pinNumber, pinStatus FROM pinStatus;");
		$query2 = mysql_query("SELECT pinNumber, pinDescription FROM pinDescription;");
                $query3 = mysql_query("SELECT pinNumber, pinMode FROM pinMode;");
		$query4 = mysql_query("SELECT manualDuration FROM manual_start;");
		$totalGPIOCount = mysql_num_rows($query);
		$currentGPIOCount = 0;
		print '<table name="GPIO" border="1" cellpadding="6">';
		print '<tr><th>GPIO #</th><th>GPIO Description</th><th>Status</th><th>Mode</th><th>Action</th><th>Manual Duration (0 = Infinite)</th><th>Edit Description</th></tr>';
		while ($currentGPIOCount < $totalGPIOCount){
			$pinRow = mysql_fetch_assoc($query);
			$descRow = mysql_fetch_assoc($query2);
                        $modeRow = mysql_fetch_assoc($query3);
			$durRow = mysql_fetch_assoc($query4);
			$pinNumber = $pinRow['pinNumber'];
			$pinStatus = $pinRow['pinStatus'];
			$pinMode = $modeRow['pinMode'];
			$pinDescription = $descRow['pinDescription'];
			$manualDuration = $durRow['manualDuration'];
                        If ($pinMode == "0") {
                            $buttonValue1 = "Auto Mode";
                            $action1 = "manualMode";
			    $buttonValue = "Auto Settings";
			    $action = "autoEdits";
			    If ($pinStatus == "0") {
				$image = "off.jpg";
			    } else {
				$image = "on.jpg";
			    }
                         } else {
                            $buttonValue1 = "Manual Mode";
                            $action1 = "autoMode";
			    If ($pinStatus == "0"){
				$buttonValue = "Turn On";
				$action = "turnOn";
				$image = "off.jpg";
			    } else {
				$buttonValue = "Turn Off";
				$action = "turnOff";
				$image = "on.jpg";
			    }
			}
			print '<tr>';
			print '<td align="center">' . $pinNumber . '</td>
			<td>' . $pinDescription . '</td>
			<td align="center"><img src="' . $image . '" width="50"><td align="center" valign="middle"><form name="pin' . $pinNumber . 'edit" action="control.php" method="get"><input type="hidden" name="action" value="' . $action1 . '"><input type="hidden" name="pin" value="' . $pinNumber . '"><input type="submit" value="' . $buttonValue1 . '"></form></td></td>
			<td><form name="pin' . $pinNumber . 'edit" action="control.php" method="get"><input type="hidden" name="action" value="' . $action . '"><input type="hidden" name="pin" value="' . $pinNumber . '"><input type="submit" value="' . $buttonValue . '"></td>
			<td><input type="text" name="manualDuration" value="' . $manualDuration . '" method="get"></form></td>
			<td><form name="pin' . $pinNumber . '" action="control.php" method="get"><input type="hidden" name="action" value="edit"><input type="hidden" name="pin" value="' . $pinNumber . '"><input type="submit" value="Edit"></form></td>';
			print '</tr>';
			$currentGPIOCount ++;
		}
		print '</table>';
		mysql_close();
	print '
	<br><br>
	<a href="control.php?action=logout">Log out</a>
	</font>
	</html>
	';
}
?>