<?php
use Analog\Analog;

class CTUUser
{

    private static function getUserByPublicKey($pukey, $document_type, $with_pusage = 0)
    {
        CTUDB::$last_errors = "";
        Analog::log(__FUNCTION__ . ' - BEGIN');
        /*
         * CALL `charactell_license`.`sp_user_details`(<{IN pukey varchar(32)}>, <{IN with_pusage tinyint}>);
         *
         */
        try {
            $params = array();
            $params[] = CTUDB::convertToParamArray("pukey", $pukey, PDO::PARAM_STR);
            $params[] = CTUDB::convertToParamArray("doc_type", $document_type, PDO::PARAM_INT);
            $params[] = CTUDB::convertToParamArray("with_pusage", $with_pusage, PDO::PARAM_INT);

            $results = CTUDB::execStoredProcedure("sp_user_details", $params);

            // check the results
            if (false == $results || empty($results) || empty($results[0]) || ! isset($results[0]["id"])) {
                CTUDB::$last_errors = "no records returned";
                return false;
            }
            return $results;
        } finally {
            Analog::log(__FUNCTION__ . ' END');
        }

        return false;
    }

    private static function getPageLeftByPlan($user, $document_type, $umonth = false)
    {
        CTUDB::$last_errors = "";
        Analog::log(__FUNCTION__ . ' - BEGIN');
        /*
         * CALL `charactell_license`.`sp_pages_left_by_plan`(<{user_id int}>, <{usage_type int}>, <{doc_type int}>, <{curr_month varchar(6)}>);
         *
         *
         */
        try {
            $params = array();
            $params[] = CTUDB::convertToParamArray("user_id", $user["id"], PDO::PARAM_INT);
            $params[] = CTUDB::convertToParamArray("usage_type", $user["usage_type"], PDO::PARAM_INT);
            $params[] = CTUDB::convertToParamArray("doc_type", $document_type, PDO::PARAM_INT);
            if (! empty($umonth)) {
                $params[] = CTUDB::convertToParamArray("curr_month", $umonth, PDO::PARAM_INT);
            } else {
                $params[] = CTUDB::convertToParamArray("curr_month", NULL, PDO::PARAM_NULL);
            }

            // var_dump($params);

            $results = CTUDB::execStoredProcedure("sp_pages_left_by_plan", $params);
            // var_dump($results);
            // check the results
            if (false == $results || empty($results) || empty($results[0]) || ! isset($results[0]["user_id"])) {
                CTUDB::$last_errors = "no records returned";
                return false;
            }
            return $results;
        } finally {
            Analog::log(__FUNCTION__ . ' END');
        }

        return false;
    }

    /**
     *
     * @param array $user
     * @param string $ip
     * @param
     *            JSON string $request
     * @return boolean
     */
    private static function updateUserPageUsage($user, $ip, $request)
    {
        CTUDB::$last_errors = "";
        Analog::log(__FUNCTION__ . ' - BEGIN');
        /*
         * CALL `charactell_license`.`sp_add_log_entry`
         * (<{IN client_ip varchar(45)}>,
         * <{IN user_id int}>,
         * <{IN act_date datetime}>,
         * <{IN npages int}>, <{IN ndocs int}>,
         * <{IN doctype int}>);
         *
         *
         */
        // var_dump($request);
        // die();
        try {
            $params = array();
            $params[] = CTUDB::convertToParamArray("client_ip", $ip, PDO::PARAM_STR);
            $params[] = CTUDB::convertToParamArray("user_id", $user["id"], PDO::PARAM_INT);
            $params[] = CTUDB::convertToParamArray("act_date", NULL, PDO::PARAM_NULL);
            $params[] = CTUDB::convertToParamArray("npages", $request["npages"], PDO::PARAM_INT);
            $params[] = CTUDB::convertToParamArray("ndocs", $request["ndocs"], PDO::PARAM_INT);
            $params[] = CTUDB::convertToParamArray("doc_type", $user["doc_type"], PDO::PARAM_INT);

            $results = CTUDB::execStoredProcedure("sp_add_log_entry", $params);

            // check the results
            if (false == $results || empty($results) || empty($results[0]) || ! isset($results[0]["id"])) {
                CTUDB::$last_errors = "no records returned";
                return false;
            }
            return $results;
        } finally {
            Analog::log(__FUNCTION__ . ' END');
        }

        return false;
    }

    private static function filterUserFieldResponse($user)
    {
        $filtered = array();
        $filter = array(
            "private_key",
            "public_key",
            "last_updated",
            "creation_date",
            "id"
        );

        // var_dump($user);
        // die();
        foreach ($user as $key => $value) {
            if (in_array($key, $filter))
                continue;
            $filtered[$key] = $value;
        }

        return $filtered;
    }

    public static function loadUserDetails($pukey, $document_type, $with_pusage = 0)
    {
        $user = CTUUser::getUserByPublicKey($pukey, $document_type, $with_pusage);
        if ($user !== false) {

            return $user[0];
        }

        return false;
    }

    public static function updateUsageTable($user, $request_ip, $request)
    {
        $req_array = json_decode($request, true);
        $result = CTUUser::updateUserPageUsage($user, $request_ip, $req_array);
        if (false !== $result) {
            return $result[0];
        }

        return false;
    }

    public static function isValidRequest($user, $request)
    {
        // calculate checksum with user's private key and compare to signature
        Analog::log(__FUNCTION__ . ' - BEGIN');
        try {
            $data = json_decode($request, true);
            if (! isset($data) || ! array_key_exists("signature", $data)) {
                return false;
            }

            $signature = $data["signature"];
            unset($data["signature"]);

            $raw_data = json_encode($data) . $user["private_key"];

            if (sha1($raw_data, false) === $signature) {
                // the request is valid
                // TBD - validate time stamp ( 10-30 seconds? )
                return true;
            }
        } finally {
            Analog::log(__FUNCTION__ . ' - END');
        }

        return false;
    }

    public static function filteredSignedResponse($response, $pkey = null)
    {
        $filtered = CTUUser::filterUserFieldResponse($response);
        return CTUUser::injectSignature(isset($pkey) ? $pkey : $response["private_key"], $filtered);
    }

    public static function injectSignature($pkey, $data, $b64encode = false)
    {

        // data is JSON array
        // calculate signature and inject into data
        // inject signature into data
        // convert to base64

        // calculate checksum with user's private key and compare to signature
        $raw_data = json_encode($data) . $pkey;

        $signature = sha1($raw_data, false);
        $data["signature"] = $signature;
        if ($b64encode)
            return base64_encode(json_encode($data));
        else
            return $data;
    }

    public static function canProceed($user, $document_type, $umonth = false, $usage_type_known = false)
    {
        // use the usage type from the user record (for now)
        $result = CTUUser::getPageLeftByPlan($user, $document_type);

        if (false !== $result && $result[0]["pages_left"] > 0 || $result[0]["docs_left" > 0]) {
            return $result[0];
        }

        return false;
    }

    public static function detailedCantProceedMessage($result)
    {
        if (false !== $result) {
            return "maximum pages used: limit-" . $result["page_limit"] . " used-" . $result["total_pages"];
        }
        return "usage plan not found ";
    }
}

