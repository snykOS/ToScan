<?php
	class CTIDConfig {
		
		const ACTION_PAGE 			= "kocr.php" ;
		//path
		const GRAB_PATH 			= "C:/new_samples/" ;
		const BASE_WORK_PATH		= "C:/IDFiles/" ;
		//script parameters
		const SCRIPT_MAX_DELAY		= 90 ;
		const GLOBAL_MAX_EXEC_TIME	= 240 ;
		
		
		const FIELD_OPTIONS 		= 'counterField' ;
		const FIELD_ITYPE	 		= 'itype' ;
		const FIELD_DL_FRONT		= 'idf' ;
		const FIELD_DL_BACK			= 'idb' ;
		const FIELD_DL_PASSPORT		= 'psp' ;
		const FIELD_DL_INSURANCE	= 'ins' ;
		const FIELD_COUNTRY			= 'country' ;
		
		const ITYPE_DL				= 1 ;
		const ITYPE_VIN				= 2 ;
		
		const OPT_DL_FRONT			= 1 ;
		const OPT_DL_BACK			= 2;
		const OPT_VIN				= 4 ;
		const OPT_PASSPORT			= 8 ;
		const OPT_INSURANCE			= 16 ;
		
		public static function get_client_ip() {
			$ipaddress = '';
			if (isset($_SERVER['HTTP_CLIENT_IP']))
				$ipaddress = $_SERVER['HTTP_CLIENT_IP'];
			else if(isset($_SERVER['HTTP_X_FORWARDED_FOR']))
				$ipaddress = $_SERVER['HTTP_X_FORWARDED_FOR'];
			else if(isset($_SERVER['HTTP_X_FORWARDED']))
				$ipaddress = $_SERVER['HTTP_X_FORWARDED'];
			else if(isset($_SERVER['HTTP_FORWARDED_FOR']))
				$ipaddress = $_SERVER['HTTP_FORWARDED_FOR'];
			else if(isset($_SERVER['HTTP_FORWARDED']))
				$ipaddress = $_SERVER['HTTP_FORWARDED'];
			else if(isset($_SERVER['REMOTE_ADDR']))
				$ipaddress = $_SERVER['REMOTE_ADDR'];
			else
				$ipaddress = 'UNKNOWN';
			return $ipaddress;
		}
		
		static public function action_url() {
		
			;
		}
		
	}