<?php
use Analog\Analog;

class CTUDB
{

    // put your code here
    static $connection;

    static $last_errors;

    static public function connect()
    {
        Analog::log("CTDBSQLSRV:connect: BEGIN");
        try {
            if (! isset(CTUDB::$connection)) {
                CTUDB::$connection = false;
                $config = parse_ini_file('../../../../private/ctudb.conf.ini');
                // var_dump($config); die;
                // MYSQL: mysql:host=localhost;dbname=test
                $connection_string = "mysql:host=" . $config["host"] . ";";
                if (! empty($config["port"]))
                    $connection_string .= "port=" . $config["port"] . ";";
                $connection_string .= "dbname=" . $config["dbname"] . ";";

                $password = $config['password'];
                // die($connection_string . " " . $password) ;
                CTUDB::$connection = new \PDO($connection_string, $config['user'], $password);
                // for debug mode
                CTUDB::$connection->setAttribute(\PDO::ATTR_ERRMODE, \PDO::ERRMODE_WARNING);
            }
        } catch (PDOException $e) {

            Analog::log("CTDBSQLSRV:connect:" . 'Connection failed: ' . $e->getMessage());
            return FALSE;
        }

        // If connection was not successful, handle the error
        if (CTUDB::$connection === false) {
            Analog::log("CTDBSQLSRV:connect: END - NOK");
            return FALSE;
        }
        Analog::log("CTDBSQLSRV:connect: END - OK");
        return TRUE;
    }

    static public function disconnect()
    {
        CTUDB::$connection = null;
    }

    static public function query($sql)
    {
        try {

            if (TRUE == CTUDB::connect()) {
                $stmt = CTUDB::$connection->prepare($sql);
                $stmt->execute();
                if ($stmt->errorCode() == 0) {
                    $sql_result = $stmt->fetchAll(\PDO::FETCH_ASSOC);
                } else {
                    $errors = $stmt->errorInfo();
                    echo ($errors[2]);
                }
                return $sql_result;
            }
        } catch (PDOException $e) {
            echo 'quering failed: ' . $e->getMessage();
            return FALSE;
        } finally {
            // CTUDB::disconnect();
        }
    }

    public static function convertToParamArray($name, $value, $type = PDO::PARAM_STR)
    {
        $result = array();
        $result["name"] = $name;
        if ($type == PDO::PARAM_INT) {
            $result["value"] = intval($value);
        } else {
            $result["value"] = $value;
        }
        $result["type"] = $type;

        return $result;
    }

    private static function bindArrayParam(PDOStatement $stmt, $sp_param_pairs)
    {
        foreach ($sp_param_pairs as $param) {
            $stmt->bindValue(":" . $param["name"], $param["value"], $param["type"]);
        }
    }

    private static function sqlResults(PDOStatement $stmt, $return_type)
    {
        if ($stmt->errorCode() == 0) {
            if ("fetch" == $return_type) {
                $data = $stmt->fetchALL(\PDO::FETCH_ASSOC);
                // var_dump($data); die;
                if (empty($data)) {
                    // no records found, incorrect or deleted job_id
                    // die('foo');
                    CTUDB::$last_errors = 'no records found';
                    $sql_result = false;
                    Analog::log(__FUNCTION__ . "SQL: returned no values");
                } else {
                    $sql_result = $data;
                }
            } else {
                // if not fetch, regular result
                $affected_records = $stmt->rowCount();
                $sql_result = array(
                    "status" => "success",
                    "affected_records" => $affected_records
                );
            }
        } else {
            $errors = $stmt->errorInfo();
            Analog::log(__FUNCTION__ . 'error exec SQL:' . print_r($errors, true));
            $sql_result = false;
            CTUDB::$last_errors = print_r($errors, true);
        }

        return $sql_result;
    }

    public static function execStoredProcedure($sp_name, array $sp_param_pairs, $return_type = "fetch")
    {
        CTUDB::$last_errors = "";
        Analog::log(__FUNCTION__ . ' - BEGIN');
        // var_dump($cmn) ;die;
        $sql = "";
        // each memeber has to be array of name, value, type
        foreach ($sp_param_pairs as $param) {

            $sql .= ", :" . $param["name"];
        }
        $sql = "CALL $sp_name( " . trim($sql, ", ") . ")";

        try {
            if (TRUE == CTUDB::connect()) {
                $stmt = CTUDB::$connection->prepare($sql);
                CTUDB::bindArrayParam($stmt, $sp_param_pairs);
                $stmt->execute();
                return CTUDB::sqlResults($stmt, $return_type);
            }
        } catch (PDOException $e) {
            Analog::log(__FUNCTION__ . 'exception: ' . $e->getMessage());
            CTUDB::$last_errors = $e->getMessage();

            return FALSE;
        } finally {
            CTUDB::disconnect();
            Analog::log(__FUNCTION__ . ' END');
        }
        return false;
    }
}

