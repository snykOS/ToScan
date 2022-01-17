<?php
class CTIDProcess {
	
	static public function validateFormInput($accountId, $stationId, $counter, &$errorMsg)
	{
		$valuesOK = true;
		$errorMsg = '';
		include_once 'license.inc';
		
		if ($accountId=='')
		{
			$valuesOK = false; $errorMsg .= 'Must input a valid account number.<br>';
		} else {
	
			$valuesOK = in_array( $accountId, $__license__ ) ;
			
			if( !$valuesOK ) {
				
				$valuesOK = CTLLicense::web_grant_access( $accountId ) ;
				if( !$valuesOK ) {

					$errorMsg = "Access denied to account $accountId" ;
				}
				// her check the DB validation
			}
				
			
		}
		if ($stationId=='')
		{
			$valuesOK = false; $errorMsg .= 'Must input a valid station number.<br>';
		}
		if ($counter=='')
		{
			$valuesOK = false; $errorMsg .= 'Must input a valid counter.<br>';
		}
		$uploadedFilesErr = $_FILES['image']['error'];
		if ($uploadedFilesErr>0)
		{
			$valuesOK = false; $errorMsg .= "$uploadedFilesErr- Uploaded file is missing.<br>";
		}
		else
		{
			$ext = pathinfo($_FILES['image']['name'], PATHINFO_EXTENSION);
	
			if ( !in_array( $ext, array( 'jpg', 'JPG', 'jpeg', 'JPEG', 'jpeg','BMP', 'bmp', 'png', 'PNG' ) ) )
			{
				$valuesOK = false; $errorMsg .= 'Must choose image file.<br>';
			}
		}
	
		return $valuesOK;
	}
	
	static function chk2value( $checkbox, $opt_flag ) {
		
		if( isset( $_POST[$checkbox] ) && !empty( $_POST[$checkbox] ) ) 
			return $opt_flag ;
		return 0 ;		
	}
	
	static function get_option_string() {

		$options = 0 ;
		
 		if( isset($_POST[CTIDConfig::FIELD_OPTIONS]) ) {
			// option is set take it as is
			$options = $_POST[CTIDConfig::FIELD_OPTIONS] ;
		} else {
			//combine from form inputs
			if( isset( $_POST[CTIDConfig::FIELD_ITYPE] ) ) {
				
				
				switch( $_POST[CTIDConfig::FIELD_ITYPE] ) {
					
					case CTIDConfig::ITYPE_DL :
						$options = CTIDProcess::chk2value( CTIDConfig::FIELD_DL_FRONT, CTIDConfig::OPT_DL_FRONT ) ;
						$options |= CTIDProcess::chk2value( CTIDConfig::FIELD_DL_BACK, CTIDConfig::OPT_DL_BACK ) ;
						$options |= CTIDProcess::chk2value( CTIDConfig::FIELD_DL_PASSPORT, CTIDConfig::OPT_PASSPORT ) ;
						$options |= CTIDProcess::chk2value( CTIDConfig::FIELD_DL_INSURANCE, CTIDConfig::OPT_INSURANCE ) ;
						$options = decbin( $options ) ;
						break ;
					case CTIDConfig::ITYPE_VIN :
						$options = decbin( CTIDConfig::OPT_VIN ) ;
						break ;
						
					default:
				}
			}
				 
		}
		
		return $options ;
	}
	
	static function grab_copy( $source, $account_id ) {
	
		try {
	
			$prefix = CTIDConfig::GRAB_PATH . date("Ymd") ."/" ;
			if( !file_exists( $prefix ) )
				mkdir( $prefix ) ;
			$dest = $prefix . $account_id . "-" . date("YmdHisu") . "." . pathinfo( $source, PATHINFO_EXTENSION ) ;
			//die( $dest ) ;
			@copy( $source, $dest) ;
	
		} catch( Exception $e) {
	
		}
	
	}
	
}