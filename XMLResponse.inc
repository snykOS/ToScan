<?php
class XMLResponse {

	const VER								= "1.0" ;
	const DEFAULT_RESPONSE_NAME 			= "unknown" ;
		
	const NODE_ROOT 						= "wsresponse" ;
	const ATTR_ROOT_VER						= "version" ;
	const ATTR_ROOT_REQUEST					= "response_on" ;
	
	const NODE_RESPONSE_SUM					= "result" ;
	const ATTR_RESPONSE_CODE				= "code" ;
	const ATTR_RESPONSE_MESSAGE				= "message" ;
	
	const NODE_UPLOAD						= "uploadinfo" ;
	const ATTR_UPLOAD_RECID					= "record_id" ;
	const ATTR_UPLOAD_UTYPE					= "utype" ;
	
	const NODE_LOGIN						= "login" ;
	const ATTR_LOGIN_GUID					= "guid" ; 
	const ATTR_LOGIN_ACCOUNT				= "accountid" ;
	const ATTR_LOGIN_CUSTOMERID				= "customerid" ;
	const ATTR_LOGIN_EXPIRED				= "expired" ;
	const ATTR_LOGIN_PAGES					= "npages" ;
	
	const NODE_EXPENSES						= "expenses" ;
	const NODE_EXPENSE						= "expense" ;
	const ATTR_EXPENSE_ID					= "id" ;
	const ATTR_EXPENSE_COMPLETED			= "completed" ;
	
	const NODE_EXPENSE_FIELDS				= "exfields" ;
	const NODE_EXPENSE_FIELD				= "field" ;
	const ATTR_EXPENSE_FIELD_NAME			= "name" ;
	const ATTR_EXPENSE_FIELD_VALUE			= "value" ;

	const NODE_GETIMAGE						= "image" ;
	const ATTR_IMAGE_URL					= "url" ;
	
	const NODE_DELETE_EXPENSE				= "deleted" ;
	const ATTR_DELETE_EXPENSE_ACFCTD		= "affected_records" ;
	const ATTR_DELETE_EXPENSE_IDS			= "affected_ids" ;

	const NODE_UPDATE_EXPENSE				= "update" ;
	const ATTR_UPDATE_EXPENSE_ACFCTD		= "affected_records" ;
	const ATTR_UPDATE_EXPENSE_IDS			= "affected_ids" ;
		
	const NODE_STAT							= "statistics" ;
	const NODE_STAT_PENDING					= "pending" ;
	const NODE_STAT_READY					= "ready" ;
	const NODE_STAT_REPORT					= "reports" ;
	const ATTR_STAT_COUNT					= "count" ;
	

	private static function openXMLResponse( XMLWriter $xml, $res_code, $message, $response_on = XMLResponse::DEFAULT_RESPONSE_NAME ) {

		$xml->openURI("php://output");
		$xml->startDocument( "1.0", "utf-8" );
		$xml->setIndent(true);
			
		$xml->startElement(XMLResponse::NODE_ROOT);
		$xml->writeAttribute( XMLResponse::ATTR_ROOT_VER, XMLResponse::VER ) ;
		$xml->writeAttribute( XMLResponse::ATTR_ROOT_REQUEST, $response_on ) ;
		
		$xml->startElement(XMLResponse::NODE_RESPONSE_SUM);
		$xml->writeAttribute(XMLResponse::ATTR_RESPONSE_CODE, $res_code);
		$xml->writeAttribute(XMLResponse::ATTR_RESPONSE_MESSAGE, $message);
		$xml->endElement();
	}
	
	private static function closeXMLResponse( XMLWriter $xml ) {
	
		$xml->endElement();
		//$xml->endDocument();
		// get XML as string
		ob_start() ;
		$xml->flush();
		return ob_get_clean() ;
	}
	
	public static function cancel( $res_code, $record_id, $message ="" ) {

		$xml = new XMLWriter();
		$message .= " - request canceled, session: $record_id" ;	
		XMLResponse::openXMLResponse($xml, $res_code, $message, FSCAPI::CMD_CANCEL) ;
		return XMLResponse::closeXMLResponse($xml) ;
	}
	
	public static function upload( $res_code, $record_id, $type = 0, $message ="" ) {

		$xml = new XMLWriter();
			
		XMLResponse::openXMLResponse($xml, $res_code, $message, FSCAPI::CMD_UPLOAD) ;
		if( $res_code == FSCAPI::RESULT_OK ) {

			$xml->startElement(XMLResponse::NODE_UPLOAD);
			$xml->writeAttribute(XMLResponse::ATTR_UPLOAD_RECID, $record_id );
			$xml->writeAttribute(XMLResponse::ATTR_UPLOAD_UTYPE, $type );
			$xml->endElement();
				
		} else {
			
		}
		
		return XMLResponse::closeXMLResponse($xml) ;
	}
	
	

	private static function GUID()
	{
		if (function_exists('com_create_guid') === true)
		{
			return trim(com_create_guid(), '{}');
		}
	
		return sprintf('%04X%04X-%04X-%04X-%04X-%04X%04X%04X', mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(16384, 20479), mt_rand(32768, 49151), mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(0, 65535));
	}
	
	public static function login( $res_code, $user_name, $user_password ) {

		$xml = new XMLWriter();
			
		XMLResponse::openXMLResponse($xml, $res_code, $message, FSCAPI::CMD_LOGIN) ;
		if( $res_code == FSCAPI::RESULT_OK ) {
		
			$xml->startElement(XMLResponse::NODE_LOGIN);
			$xml->writeAttribute(XMLResponse::ATTR_LOGIN_GUID, XMLResponse::GUID() );
			$xml->writeAttribute(XMLResponse::ATTR_LOGIN_ACCOUNT, '6390000' );
			$xml->writeAttribute(XMLResponse::ATTR_LOGIN_CUSTOMERID, '540189727' );
			$xml->writeAttribute(XMLResponse::ATTR_LOGIN_EXPIRED, '20160101');
			$xml->writeAttribute(XMLResponse::ATTR_LOGIN_PAGES, '1000' );
			$xml->endElement();
		}
		
		return XMLResponse::closeXMLResponse($xml) ;
		
	}
	
	public static function empty_response() {
		
		$xml = new XMLWriter();
			
		XMLResponse::openXMLResponse($xml, 0x001, 'response is empty' ) ;
		return XMLResponse::closeXMLResponse($xml) ;		
	}
	
}