<?php
header("Cache-Control: no-cache, must-revalidate") ;
define( "ROOT_FOLDER", "d:/xamp64/htdocs/fsecloud/payroll/" ) ;

class Config
{

	const ACTION_PAGE 			= "index.php";
	//path
	const GRAB_PATH 			= "D:/PayRoll/temp/";
	const BASE_WORK_PATH		= "D:/PayRoll/";
	//script parameters
	const SCRIPT_MAX_DELAY		= 90;

	public static function get_client_ip()
	{
		$ipaddress = '';
		if ($_SERVER['HTTP_CLIENT_IP'])
			$ipaddress = $_SERVER['HTTP_CLIENT_IP'];
		else if ($_SERVER['HTTP_X_FORWARDED_FOR'])
			$ipaddress = $_SERVER['HTTP_X_FORWARDED_FOR'];
		else if ($_SERVER['HTTP_X_FORWARDED'])
			$ipaddress = $_SERVER['HTTP_X_FORWARDED'];
		else if ($_SERVER['HTTP_FORWARDED_FOR'])
			$ipaddress = $_SERVER['HTTP_FORWARDED_FOR'];
		else if ($_SERVER['HTTP_FORWARDED'])
			$ipaddress = $_SERVER['HTTP_FORWARDED'];
		else if ($_SERVER['REMOTE_ADDR'])
			$ipaddress = $_SERVER['REMOTE_ADDR'];
		else
			$ipaddress = 'UNKNOWN';
		return $ipaddress;
	}

	public static function save_session_ip($file_name)
	{

		try {
			if (preg_match('"\_b."', $file_name)) {

				$file_name_pic = preg_replace('"\_b.(xml|jpg)$"', '.pic.ip', $file_name);
				$file_name_sig = preg_replace('"\_b.(xml|jpg)$"', '.sig.ip', $file_name);
				$file_name_ful = preg_replace('"\_b.(xml|jpg)$"', '.ful.ip', $file_name);
			} else {
				$file_name_pic = preg_replace('"\.(xml|jpg)$"', '.pic.ip', $file_name);
				$file_name_sig = preg_replace('"\.(xml|jpg)$"', '.sig.ip', $file_name);
				$file_name_ful = preg_replace('"\.(xml|jpg)$"', '.ful.ip', $file_name);
			}

			$ip = Config::get_client_ip();

			file_put_contents($file_name_pic, $ip);
			file_put_contents($file_name_sig, $ip);
			file_put_contents($file_name_ful, $ip);
		} catch (Exception $exp) {
		}
	}
}

use Analog\Analog;
function old_autoload( $class )
{

	// process class
	$filename = ROOT_FOLDER . "lib/$class.inc" ;
	
	if( file_exists( $filename ) ) {

		require_once(  $filename ) ;
		return ;

	}
	
	if( file_exists( __DIR__ . DIRECTORY_SEPARATOR . "$class" . '.php') )
		include __DIR__ . DIRECTORY_SEPARATOR . "$class" . '.php';
	
	
}

spl_autoload_register('old_autoload');

