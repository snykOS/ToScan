<?php
class FSCAPI {
	
	// api commands
	const CMD_LOGIN 				= "login" ;
	const CMD_LOGOUT 				= "logout" ;
	const CMD_UPLOAD				= "upload" ;
	const CMD_GETIMAGE				= "getimage" ;
	const CMD_NEXTJOB				= "nextjob" ;
	const CMD_NEXTINPUT				= "nextinput" ;
	// for server that pulls jobs for all
	const CMD_NEXTJOBDATA			= "nextjobdata" ;
	const CMD_NEXTINPUTDATA			= "nextinputdata" ;
	const CMD_CANCEL				= "cancelcurrentaction" ;
	
	const CMD_UPLOADJOB				= "jobupload" ;
	
	//	action fields - fields within POST to provide action
	const AFLD_ACTION				= "action" ;
	const AFLD_GUID					= "guid" ;
	const AFLD_VENDORID				= "customer_id" ;
	const AFLD_ACCOUNT				= "account_id" ;
	const AFLD_IMAGE				= "scanned_image" ;
	const AFLD_USER					= "username" ;
	const AFLD_PASSWORD				= "password" ;
	
	const RESULT_OK					= 0x0000 ;
	const RESULT_LOGIN_FAIL			= 0x0001 ;
	const RESULT_UPLOAD_FAIL		= 0x0002 ;
	const RESULT_DELETE_FAIL		= 0x0003 ;
	const RESULT_UPDATE_FAIL		= 0x0004 ;
	const RESULT_GIMAGE_FAIL		= 0x0005 ;
	const RESULT_LIST_FAIL			= 0x0006 ;
	const RESULT_UNKNOWN			= 0xFFFF ; 
	
	const DB_UTYPE_INPUT			= 0 ;
	const DB_UTYPE_JOB				= 0x0001 ;
	

	public static $expense_post_map = array( 
			FSCAPI::RFLD_EXPENSE_ID				=> "`expense`.`id`" , // unique number from MySQL
			FSCAPI::RFLD_PHONE					=> "`expense`.`rcpt_phone`" ,
			FSCAPI::RFLD_VENDOR_NAME			=> "`expense`.`rcpt_vendor`" ,
			FSCAPI::RFLD_VENDOR_NAME2			=> "`expense`.`rcpt_vendor`" ,
			FSCAPI::RFLD_VENDOR_ID				=> "`expense`.`rcpt_vendor_id`" ,
//			FSCAPI::RFLD_EXPENSE				=> "`expense`.`exp_type_text`" , // temporary, should be expense_type_id 	
			FSCAPI::RFLD_TOTAL					=> "`expense`.`rcpt_total`" ,
			FSCAPI::RFLD_TAX					=> "`expense`.`rcpt_tax`" ,
			FSCAPI::RFLD_DATE					=> "`expense`.`rcpt_date`" ,
	//		FSCAPI::RFLD_TELEPHONE				=> "Telephone" , // who is this ?
			FSCAPI::RFLD_INV_NUMBER	 			=> "`expense`.`rcpt_number`" ,
			FSCAPI::RFLD_COUNTRY				=> "`expense`.`rcpt_country_code`" ,
		    FSCAPI::RFLD_CURRENCY				=> "`expense`.`currency_id`",
			FSCAPI::RFLD_USER_ID				=> "`expense`.`user_id`",
			FSCAPI::RFLD_COMMENT				=> "`expense`.`rcpt_comment`",
			FSCAPI::RFLD_EXP_TYPE_ID			=> "`expense`.`exp_type_id`" ) ;
	
	public static $update_fields_list = array(
			"rcpt_vendor",
			"rcpt_vendor_id",
			"rcpt_date",
			"rcpt_number",
			"rcpt_tax",
			"rcpt_total",
			"rcpt_comment",
			"rcpt_country_code",
			"exp_type_id",
			"currency_id"
			) ;

}

