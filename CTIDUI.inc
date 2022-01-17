<?php
class CTIDUI {
	static public function outputHTMLBegin($title="KloudOCR IDs Demo") {
		ob_start ();
		?>
<!DOCTYPE HTML>
<html>
<head>
<title><?php print $title?></title>
<meta http-equiv="content-type" content="text/html; charset=UTF-8" />
<link rel="stylesheet" type="text/css" href="css/style.css" />
<script type="text/javascript"
	src="https://ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js"></script>
<script type="text/javascript" src="./js/ids.js"></script>
</head>
<body>
	<div id="main">
		<div id="site_content">
			<?php
		return ob_get_clean ();
	}
	static public function outputHTMLEnd() {
		ob_start ();
		// <a href="http://www.charactell.com/formstorm_builds/IDEvalSetup113.exe" style="color: #0F55EB"> <b>Download KloudOCR Evaluation </b><small>( Windows only )</small></a>
		?>
			
		
		</div>
	</div>
</body>

</html>
<?php
		return ob_get_clean ();
	}
	static public function outputNewForm() {
		$action = CTIDConfig::ACTION_PAGE . (empty ( $_SERVER ['QUERY_STRING'] ) ? "" : "?" . $_SERVER ['QUERY_STRING']);
		
		$accid = "";
		if (isset ( $_COOKIE ["accid"] ))
			$accid = $_COOKIE ["accid"];
		
		ob_start ();
		?>
<h1>KloudOCR</h1>
<h3>ID Server test page</h3>

<form enctype="multipart/form-data" action="<?php print $action?>"
	method="POST">
	<div class="form_settings">
		<p>
			<span>Account ID:</span> <input class="id_upload" type="text"
				name="accountField" value="<?php print $accid;?>"
				required="required" autocomplete="on" autofocus="autofocus">
		</p>

		<p>
			<span>Station ID: </span> <input class="id_upload" type="text"
				name="stationField" value="12345">
		</p>

		<p>
			<span>Image Type:</span> <input class="radio" type="radio"
				id="radio_ids" name="itype" value="1" checked="checked"> ID
		
		
		<fieldset id="fldset_ids">
			<p>
				Country:<br> <input class="checkbox" type="checkbox" name="country"
					value="1" checked="checked" disabled="disabled"
					style="padding-left: 10px">US
			</p>
			<p>
				ID Type:<br> <span class="field_group"> <input class="checkbox"
					type="checkbox" name="idf" value="1" checked="checked"> DL Front <input
					class="checkbox" type="checkbox" name="idb" value="1"
					checked="checked"> DL Back <br> <!-- <input class="checkbox"
					type="checkbox" name="psp" value="1" checked="checked"> Passport <input
					class="checkbox" type="checkbox" name="ins" value="1"
					checked="checked"> Insurance Card -->
				</span>



			</p>
		</fieldset>
		</p>

		<p>



			<span>&nbsp;</span><input class="radio" id="radio_vin" type="radio"
				name="itype" value="2"> VIN

		</p>

		<p>



			<span>Choose ID photo:</span> <input class="id_upload" name="image"
				type="file" required="required" />

		</p>

		<p style="padding-top: 15px">



			<span>&nbsp;</span> <input class="submit" type="submit"
				name="contact_submitted" value="Upload" />



		</p>
	</div>

</form>
<?php
		return ob_get_clean ();
	}
	static public function outputNewFormSingTel() {
		$action = "singtel.php" . (empty ( $_SERVER ['QUERY_STRING'] ) ? "" : "?" . $_SERVER ['QUERY_STRING']);
		ob_start ();
		?>
<div>
<img src="./images/singtel-logo.gif" alt="Singtel logo">
</div>		

<h4>Test Page</h4>

<form enctype="multipart/form-data" action="<?php print $action?>"
	method="POST">
	<div class="form_settings">
		<p>
			<span>Choose ID photo:</span> <input class="id_upload" name="image"
				type="file" required="required" />
		</p>

		<p style="padding-top: 15px">
			<span>&nbsp;</span> <input class="submit" type="submit"
				name="contact_submitted" value="Upload" />
		</p>
	</div>

</form>
<?php
		return ob_get_clean ();
	}
	static public function parse_xml_r($xml_file, $action_page = CTIDConfig::ACTION_PAGE, $header = "KloudOCR", $extra="" ) {
		 
		$action = $action_page . (empty ( $_SERVER ['QUERY_STRING'] ) ? "" : "?" . $_SERVER ['QUERY_STRING']);
		ob_start ();
		?>
<br clear="all">
<div>
	<a href="./<?php print $action ;?>"> Upload Another </a>
</div>

<?php
		$footer = ob_get_clean ();
		
		if (! file_exists ( $xml_file )) {
			
			return "ERROR: Result File is Missing" . $footer;
		}
		
		try {
			if (($xml = simplexml_load_file ( $xml_file )) != FALSE) {
				
				ob_start ();
				?>
<div>
<?php if( !empty($extra)) {
	print $extra ;
} else {
		
	print ("<h1>$header</h1>" ) ;
}
?>	
	<h4>Recognition Result</h4>
	<table>
<?php
				foreach ( $xml->children () as $node ) {
					
					$style = strcasecmp ( "ErrorMessage", $node->getName () ) != 0 ? "" : " style=\"color: red\"";
					print "<tr><td>" . $node->getName () . "</td><td><b$style>" . $node ["value"] . "</b></td></tr>";
				}
				?> 

	</table>
</div>
<?php
				$res_table = ob_get_clean ();
				return $res_table . $footer;
			}
			
			return "ERROR: Cannot load result file" . $footer;
		} catch ( Exception $err ) {
			
			print "ERROR:" . $err->getMessage () . $footer;
		}
	}
}