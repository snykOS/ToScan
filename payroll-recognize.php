<?php
header("Cache-Control: no-cache, must-revalidate");

error_reporting(E_ALL & ~E_NOTICE);
set_time_limit(60);
require_once "./lib/common.inc";

$log_file_name = './Logs/payroll-log-' . date("Ymd") . ".log";
Analog::handler(\Analog\Handler\File::init($log_file_name));


session_start();
//print_r($_SESSION);die("ssn");
$_captcha_ = empty($_SESSION['captcha']) ? "77666" : $_SESSION['captcha'];
$_home_url_ = empty($_SESSION["homeurl"]) ? "./index.php" : $_SESSION["homeurl"];
$_result_ = '';

$fields_captions = array(
	"IDNumber" => "תעודת זהות",
	"TikNikuyim" => "תיק ניכויים",
	"Year" => "שנה",
	"Month" => "חודש",
	"ProcessingTime" => "זמן פענוח בשניות",
	"Maasik" => "מעסיק",
	"Sachar" => "שכר",
	"MasMitztaber" => "מס מצטבר",
	"BasisKerenHishtalmut" => "בסיס קרן השתלמות",
	"Layout" => "תוכנה פיננסית",
	"NikuyHova" => "ניכוי חובה",
	"MasMitztaber1" => "מס מצטבר",
	"NikuyReshut" => "ניכוי רשות",
	"Neto0" => "נטו",
	"Neto" => "נטו",
	"MasMitztaber1" => "",
	"BrutoForMas" => "שכר חייב מס"
);

/*
Charactell, 12:32 PM

G GETF(NikuyHova, "");
   GETF(NikuyReshut, "");
   GETF(Neto0, "");
   GETF(Neto, "");
*/

function validateFormInput(&$errorMsg)
{
	$valuesOK = true;


	$uploadedFilesErr = $_FILES['image']['error'];
	if ($uploadedFilesErr > 0) {
		$valuesOK = false;
		$errorMsg .= "<br> $uploadedFilesErr - Uploaded file is missing.<br>";
	} else {
		$ext = pathinfo($_FILES['image']['name'], PATHINFO_EXTENSION);

		if (!in_array(strtolower($ext), array('pdf', 'jpg', 'jpeg', 'jpeg', 'bmp', 'png'))) {
			$valuesOK = false;
			$errorMsg .= 'Must choose image file.<br>';
		}
	}

	return $valuesOK;
}

function grab_copy($source, $account_id)
{

	try {

		$prefix = Config::GRAB_PATH . date("Ymd") . "/";
		if (!file_exists($prefix))
			mkdir($prefix);
		$dest = $prefix . $account_id . "-" . date("YmdHisu") . "." . pathinfo($source, PATHINFO_EXTENSION);
		//die( $dest ) ;
		@copy($source, $dest);
	} catch (Exception $e) {
	}
}

function convert_xml_to_table($file_content)
{

	$result = "";
	global $fields_captions;
	//die( htmlentities($file_content)) ;

	try {
		$xml = simplexml_load_string($file_content);
		//print_r( $xml ) ; die ;
		$i = 1;
		foreach ($xml->children() as $node) {
			$field_name = $node->getName();
			$field_caption = array_key_exists($field_name, $fields_captions) ? $fields_captions[$field_name] : $field_name;
			$result .= "<tr" . (($i++ % 2) ? " class=\"table_caption\"" : "") . "><td>"
				. $field_caption . "</td><td style=\"font-weight: 500\">" . $node->attributes()->value . "</td></tr>";
		}
	} catch (Exception $err) {
		die("can't parse xml string");
	}
	$result = "<table>$result</table>";
	return $result;
}


$asHTML = true;
$_result_ = '';

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
	$errorMsg = '';

	// if ( !isset( $_REQUEST['captcha'] ) || ($_captcha_ != $_REQUEST['captcha'] && $_captcha_ != "77666")  )
	// {
	// 	$errorMsg = "Incorrect Capture value, please try again" ;
	// }

	validateFormInput($errorMsg);
	// 	print_r($_REQUEST);
	// 	print_r($_captcha_);
	// die($errorMsg);	
	if (!empty($errorMsg)) {
		$_SESSION["last_error"] = $errorMsg;
		header("Location: $_home_url_");
		die;
	}

	$accountId = 'webeval';
	$stationId = $_captcha_;
	$options = "11-1";

	if (!empty($_POST["idtype"])) {
		if ($_POST["idtype"] == 1)
			$options = "11-1";
		elseif ($_POST["idtype"] == 2)
			$options = "100-1";
	}

	$mainPath = Config::BASE_WORK_PATH;
	$double_sided = isset($_FILES['image_back']["name"]) && !empty($_FILES['image_back']["name"]);
	//die( print_r($double_sided, true)) ;	
	$ext = pathinfo($_FILES['image']['name'], PATHINFO_EXTENSION);
	$fileName = "webeval-" . date("YmdHisu") .  "_100000000000000000-1";
	//00014148225_05032019_000000000016269438_01_TS_10_TS_000002_100000000000000000-1
	if ($double_sided) {
		$inputFileName = $mainPath . 'Input/' . $fileName . "_f.$ext";
		$inputFileNameBack = $mainPath . 'Input/' . $fileName . "_b.$ext";
		$outputFileName = $mainPath . 'Output/' . $fileName . '_b.xml';
	} else {
		$inputFileName = $mainPath . 'Input/' . $fileName . ".$ext";
		$outputFileName = $mainPath . 'Output/' . $fileName . '.xml';
	}


	$prefix = Config::GRAB_PATH . date("Ymd") . "/";
	$dest = $prefix . $accountId . "-" . date("YmdHisu") . "_100000000000000000-1." . pathinfo($inputFileName, PATHINFO_EXTENSION);

	$ip = Config::get_client_ip();
	Analog::debug("SESSION_START: {IP:$ip}{Account:$accountId}{File:$dest}");

	// Make sure no such answer already exists:
	@unlink($outputFileName);

	$_img_file_content_ = file_get_contents($_FILES['image']['tmp_name']);

	if (move_uploaded_file($_FILES['image']['tmp_name'], $inputFileName)) {
		if ($double_sided && move_uploaded_file($_FILES['image_back']['tmp_name'], $inputFileNameBack)) {

			grab_copy($inputFileName, $accountId . "_f");
			grab_copy($inputFileNameBack, $accountId . "_b");
		} else {

			//File was uploaded okay. Great, now we wait for Ofer to do his magic:
			grab_copy($inputFileName, $accountId);
		}

		$secondsToCheck = Config::SCRIPT_MAX_DELAY;
		$checkCounter = 0;
		$found = false;
		set_time_limit(120);

		while ((!$found) && ($checkCounter < $secondsToCheck)) {

			if (file_exists($outputFileName)) {
				//echo 'File Found!!!'; die;
				$found = true;

				//Config::save_session_ip($outputFileName) ;
				if (!$asHTML) {

					$len = filesize($outputFileName);
					header('Content-type: application/xml');
					header("Pragma: public");
					header("Content-Length: " . $len);
					header("Cache-control: private");
					ob_clean();
					flush();
					@readfile($outputFileName);
				} else {

					$fileContent = file_get_contents($outputFileName);
					//die('cont' . $fileContent);
					$_result_ = convert_xml_to_table($fileContent);

					$type = pathinfo($inputFileName, PATHINFO_EXTENSION);
					$base64 = 'data:image/' . $type . ';base64,' . base64_encode($_img_file_content_);
					$_SESSION["last_image"] = $base64; // store in session file content
				}
				if (!@unlink($outputFileName)) {
					// echo 'could not remove file ';
				}
				Analog::debug("SESSION_END:{IP:$ip} - OK");
				break;
			} else {
				$checkCounter++;
				sleep(1);
			}
		}
		if (empty($_result_)) {

			$_SESSION["last_error"] = 'File could not be processed in ' . Config::SCRIPT_MAX_DELAY . ' - terminated';
			Analog::debug("SESSION_END:{IP:$ip} - File could not be processed - TimeOut");
		}
	} else {

		$_SESSION["last_error"] = "There was an error uploading the file, please try again!";
		Analog::debug("SESSION_END:{IP:$ip} - There was an error uploading the file");
	}
}


$_SESSION["result"] = $_result_;
session_write_close();
//

header("Location: $_home_url_");
