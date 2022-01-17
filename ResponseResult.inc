<?php

/**
 *
 * @author Vitaly
 *        
 */
class ResponseResult
{

    public static function errorResponse($message, $code = 403)
    {
        http_response_code($code);
        return json_encode(array(
            "code" => $code,
            "message" => $message
        ));
    }

    public static function okResponse($data)
    {
    	http_response_code(200);
        return json_encode($data);
    }

    /**
     */
    public function __construct()
    {

        // TODO - Insert your code here
    }

    /**
     */
    function __destruct()
    {

        // TODO - Insert your code here
    }
}

