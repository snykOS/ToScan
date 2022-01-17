<?php

header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

session_start();

$_result_ = isset($_SESSION["result"]) ? $_SESSION["result"] : '';
$_last_error_ = isset($_SESSION["last_error"]) ? $_SESSION["last_error"] : '';
$_last_image_ = isset($_SESSION["last_image"]) ? $_SESSION["last_image"] : '';

$_SESSION["last_image"] = '';
$_SESSION["last_error"] = '';
$_SESSION["result"] = '';

//session_write_close ();

?>
<!DOCTYPE html>
<html>

<head>
	<meta charset="windows-1255">

	<title>PyRolls Online Evaluation</title>

	<link type="text/css" rel="stylesheet" href="css/wix-plugins.css">
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.4/jquery.min.js"></script>
	<script type="text/javascript" src="//static.parastorage.com/services/js-sdk/1.61.0/js/wix.min.js"></script>
	<script>
		$(document).ready(
			function() {
				$("#loading").hide();
				$("#result_section").hide();
				$("#errors_section").hide();

				$("#uform").submit(function(e) {

					$("#loading").show().fadeIn("slow");
					$("#result_section").hide();
					$("#errors_section").hide();
				});


				//check if called from wixx site and set in session
				<?php
				if (empty($_result_))
					print('$("#result_section").hide();');
				else
					print('$("#result_section").show().fadeIn("slow");');

				if (empty($_last_error_))
					print('$("#errors_section").hide();');
				else
					print('$("#errors_section").show().fadeIn("slow");');

				?>

			});
	</script>

</head>

<body>
	<?php
	// print "whaaat?" + $_result_ ;
	// print $_last_image_ ;
	//autofocus="autofocus"
	?>
	<div id="logo"></div>
	<div style="width:90%; margin:200px auto;"> <!-- "width:70%; margin-left: 10%; margin-top: 250px; position: center;" -->
		<div>
			<h2></h2>
		</div>
		<div>
			<div id="errors_section">
				<?php
				if (!empty($_last_error_))
					print($_last_error_);
				?>
			</div>
			<form id="uform" enctype="multipart/form-data" action="./payroll-recognize.php" method="POST">
				<div class="form_settings">

				<p>
						<label for="image">בחר קובץ</label> <input name="image" type="file" required="required" placeholder="Select Invoice image" />
					</p>
					<p>
						<input type="submit" id="usubmit" value="העלאת קובץ" class="submit" style="display: block;" />
					</p>
				</div>
			</form>
		</div>
<div id="loading">
			<div class="lds-ellipsis">
				<div></div>
				<div></div>
				<div></div>
				<div></div>
			</div>
		</div>
		<div id="result_section" class="container">
			<div class="inbl">
				<?php

				if (!empty($_result_))
					print($_result_);

				?>
			</div>
			<?php
			if (!empty($_last_image_)) {
				print "<img src='$_last_image_' class='inbl' width='80%'>";
			}
			?>

		</div>
	</div>

</body>

</html>